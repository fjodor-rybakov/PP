#include "BankClient.h"
#include "Bank.h"


CBankClient::CBankClient(CBank *bank, unsigned id)
	: m_bank(bank)
	, m_id(id)
{
	CreateThread(NULL, 0, ThreadFunction, this, 0, NULL);
}


unsigned CBankClient::GetId()
{
	return m_id;
}

DWORD WINAPI CBankClient::ThreadFunction(LPVOID lpParam)
{
	CBankClient *client = (CBankClient*)lpParam;
	// TODO: srand(client->m_id);
	srand(client->m_id);
	while (true)
	{
		Sleep(GetSleepDuration(client));
		client->m_bank->UpdateClientBalance(*client, GetBalanceChangeValue());
	}
	return 0;
}


unsigned CBankClient::GetSleepDuration(CBankClient *client)
{
	// TODO: check correctness of running application with no sleep, even in CBank

	// 1000 .. 2000
	return 1000 + (rand() % 2001);
}


int CBankClient::GetBalanceChangeValue()
{
	// -100 .. 100
	return rand() % 201 - 100;
}