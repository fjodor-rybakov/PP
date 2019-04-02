#include "Bank.h"

using namespace std;

CBank::CBank(int primitive)
{
	m_clients = std::vector<CBankClient>();
	m_totalBalance = 0;
	m_primitive = SynchronousPrimitive(primitive);
	InitializeCriticalSection(&csUpdateBalance);
}


CBankClient* CBank::CreateClient()
{
	unsigned clientId = unsigned(m_clients.size());
	CBankClient* client = new CBankClient(this, clientId);
	m_clients.push_back(*client);
	return client;
}


int CBank::GetClientBalanceById(int clientId) {
	return clientAccountBalance.at(clientId);
}


void CBank::SetClientBalanceById(int clientId, int value) {
	auto it = clientAccountBalance.find(clientId);
	if (it != clientAccountBalance.end()) {
		it->second += value;
	}
	else {
		clientAccountBalance.emplace(clientId, value);
	}
}

void CBank::UpdateClientBalance(CBankClient &client, int value)
{
	EnableSynchronous();
	int totalBalance = GetTotalBalance();
	std::cout << "Client " << client.GetId() << " initiates reading total balance. Total = " << totalBalance << "." << std::endl;
	
	SomeLongOperations();
	totalBalance += value;
	SetClientBalanceById(client.GetId(), value);

	std::cout
		<< "Client " << client.GetId() << " updates his balance with " << value
		<< " and initiates setting total balance to " << totalBalance
		<< ". Must be: " << GetTotalBalance() + value << "." << std::endl;

	// Check correctness of transaction through actual total balance
	if (totalBalance != GetTotalBalance() + value) {
		std::cout << "! ERROR !" << std::endl;
	}

	SetTotalBalance(totalBalance);
	DisableSynchronous();
}

vector<CBankClient> CBank::GetTotalClients() {
	return m_clients;
}


int CBank::GetTotalBalance()
{
	return m_totalBalance;
}


void CBank::SetTotalBalance(int value)
{
	m_totalBalance = value;
}

void CBank::SomeLongOperations()
{
	// TODO
	Sleep(1000);
}

SynchronousPrimitive CBank::GetPrimitive() {
	return m_primitive;
}

void CBank::EnableSynchronous() {
	if (CBank::GetPrimitive() == SynchronousPrimitive::CriticalSection) {
		EnterCriticalSection(&csUpdateBalance);
	}
	else if (CBank::GetPrimitive() == SynchronousPrimitive::Mutex) {
		mutexUpdateBalance.lock();
	}
}

void CBank::DisableSynchronous() {
	if (CBank::GetPrimitive() == SynchronousPrimitive::CriticalSection) {
		LeaveCriticalSection(&csUpdateBalance);
	}
	else if (CBank::GetPrimitive() == SynchronousPrimitive::Mutex) {
		mutexUpdateBalance.unlock();
	}
}

CBank::~CBank() {
	DeleteCriticalSection(&csUpdateBalance);
}