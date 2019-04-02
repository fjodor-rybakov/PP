#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <mutex>
#include "BankClient.h"

enum SynchronousPrimitive {
	CriticalSection,
	Mutex
};


using namespace std;

class CBank
{
public:
	CBank(int primitive);
	~CBank();

	CBankClient* CreateClient();
	void UpdateClientBalance(CBankClient& client, int value);
	vector<CBankClient>GetTotalClients();

	int GetTotalBalance();
	int GetClientBalanceById(int clientId);
	void SetClientBalanceById(int clientId, int value);

	SynchronousPrimitive GetPrimitive();

private:
	vector<CBankClient> m_clients;
	int m_totalBalance;
	map<int, int> clientAccountBalance;

	SynchronousPrimitive m_primitive;

	void EnableSynchronous();
	void DisableSynchronous();

	CRITICAL_SECTION csUpdateBalance;
	mutex mutexUpdateBalance;

	void SetTotalBalance(int value);
	void SomeLongOperations();
};