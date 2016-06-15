// FirstMultiThreads.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>

using namespace std;

void threadFunc()
{
	cout << "Welcome to Multithreading" << endl;
}

void threadFunc2(string name)
{
	cout << "Welcome to Multithreading: " <<name<< endl;
}

class A
{
public:
	A(int i) : id(i){}
	void operator()(string str) const
	{
		cout << "functor A " << str << endl;
	}
private:
	int id;
};

class B
{
public:
	B(int k) : val(k) {}
	void operator()(int i) {
		std::cout << i + val << std::endl;
	}
private:
	int val;
};

class MyFunctor
{
public:
	MyFunctor(){ id = 0; }
	MyFunctor(int i): id(i){}
	void operator()() {
		std::cout << "functor\n";
	}
private:
	int id;
};

class MyFunctorParam
{
public:
	void operator()(vector<int>& v)
	{
		for_each(v.begin(), v.end(), [](int n){cout << n << endl;});
	}
};

class MyMemClass
{
public:
	void publicFunction()
	{
		cout << "public function of myFunctor class is called" << endl;
	}
	void publicFunction2(string& s)
	{
		s = "Haha!";
		cout << s << endl;
	}

};

void Functors()
{
	A(2014)("Hello");

	vector<int> v = { 1, 2, 3, 4, 5 };
	int val = 10;
	for_each(v.begin(), v.end(), B(val));
}

void CreateThreadwithFunction()
{
	//pass a function to thread, no param
	//thread funcTest1(threadFunc);
	
	//with param
	thread funcTest1(threadFunc2,"Bryan");
	//detach funcTest1 from main thread
	//funcTest1.detach();
	//check if thread is joinable
	if (funcTest1.joinable())
	{
		//main is blocked until funcTest1 is not finished
		funcTest1.join();
	}
}

void CreateThreadwithFunctor()
{
	//No param
	//MyFunctor fnctor;
	//thread t((MyFunctor()));
	//thread t(fnctor);

	MyFunctorParam fnctorParam;
	vector<int> v = { 1, 2, 3, 4, 5 };
	thread t(fnctorParam, v);

	cout << "main thread\n";
	t.join();
}

void CreateThreadwithMemberFunction()
{
	MyMemClass myFunc;
	//initializing thread with member function of MyFunctor class, no param
	//thread functorTest(&MyMemClass::publicFunction, &myFunc);
	//with param
	string s1 = "Xixi!";
	//thread functorTest(&MyMemClass::publicFunction2, &myFunc, s1);
	thread functorTest(&MyMemClass::publicFunction2, &myFunc, ref(s1));  //can acctually pass ref
	//thread functorTest(&MyMemClass::publicFunction2, &myFunc, move(s1));  //move s1, main() will no longer has this varaible
	if (functorTest.joinable())
		functorTest.join();
	cout << s1 << endl;
}



int _tmain(int argc, _TCHAR* argv[])
{
	

	CreateThreadwithMemberFunction();
	//CreateThreadwithFunctor();
	//CreateThreadwithFunction();
	//Functors();
	return 0;
}

