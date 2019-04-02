#pragma once
#include <iostream>
#include <vector>
#include <map>

#include "BankClient.h"

class CBank
{
public:
	CBank();
	CBankClient* CreateClient();
	void UpdateClientBalance(CBankClient& client, int value);
	std::vector<CBankClient>GetTotalClients();
	int GetTotalBalance();
	int GetClientBalanceById(int clientId);
	void SetClientBalanceById(int clientId, int value);

private:
	std::vector<CBankClient> m_clients;
	int m_totalBalance;
	std::map<int, int> clientAccountBalance;

	void SetTotalBalance(int value);
	void SomeLongOperations();
};