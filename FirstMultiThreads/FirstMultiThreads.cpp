// FirstMultiThreads.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <thread>

using namespace std;

void threadFunc()
{
	cout << "Welcome to Multithreading" << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//thread funcTest1(threadFunc);
	return 0;
}

