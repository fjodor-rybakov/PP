#include "stdafx.h"
#include "Bank.h"
#include "BankClient.h"
#include "iostream"
#include "string"

using namespace std;

int main(int argc, char *argv[])
{
	int clientCount = 2, primitive = 0;

	if (argc != 3) {
		cerr << "Incorrect set params: PP2.exe <count clients> <number synchronous primitive>" << endl;
		return 1;
	}
	clientCount = atoi(argv[1]);
	primitive = atoi(argv[2]);
	CBank* bank = new CBank(primitive);

	for (int i = 0; i < clientCount; i++) {
		CBankClient* client1 = bank->CreateClient();
	}

	// TODO: WaitForMultipleObjects
	string command = "";
	while (cin >> command)
	{
		if (command == "exit" || command == "quit") {
			break;
		}
	}

	int balanceClient = 0;

	for (CBankClient client : bank->GetTotalClients()) {
		auto clientBalance = bank->GetClientBalanceById(client.GetId());
		cout << "Client Id: " << client.GetId() << ", Balance:" << clientBalance << endl;
		balanceClient += clientBalance;
	}

	cout << "Total balance clients: " << balanceClient << endl;
	cout << "Bank balance: " << bank->GetTotalBalance() << endl;

    return 0;
}