#include "stdafx.h"
#include "Bank.h"
#include "BankClient.h"
#include "iostream"
#include "string"

using namespace std;

void Help() {
	cerr << "Incorrect set params: PP2.exe <count clients> <number synchronous primitive>" << endl;
	cerr << "Count clients more that 1" << endl;
	cout << "SynchronousPrimitiveOption: CriticalSection = 0, Mutex = 1" << endl;
}

int main(int argc, char *argv[])
{
	int clientCount = 2, primitive = 0;

	if (argc == 2) {
		primitive = atoi(argv[1]);
		if (primitive != 0 || primitive != 1) {
			Help();
			return 1;
		}
	}
	else if (argc == 3) {
		clientCount = atoi(argv[1]);
		primitive = atoi(argv[2]);
		if (primitive != 0 || primitive != 1 || clientCount < 1) {
			Help();
			return 1;
		}
	}
	else {
		Help();
		return 1;
	}

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