#pragma once
#include <Windows.h>

class CBank;

class CBankClient
{
public:
	unsigned GetId();

private:
	CBankClient(CBank *bank, unsigned id);
	static unsigned GetSleepDuration(CBankClient *client);
	static int GetBalanceChangeValue();
	static DWORD WINAPI ThreadFunction(LPVOID lpParam);

	friend CBank;

	CBank *m_bank;
	unsigned m_id;
};