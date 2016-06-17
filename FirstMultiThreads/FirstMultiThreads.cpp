// FirstMultiThreads.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>

using namespace std;

void threadFunc()
{
	cout << "Welcome to Multithreading" << endl;
}

void threadFunc2(string name)
{
	this_thread::sleep_for(chrono::seconds(5));
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
	cout << "Thread ID: " << funcTest1.get_id() << endl;
	thread t2 = move(funcTest1);   //thread move, not reference, requires 
	//detach funcTest1 from main thread
	//funcTest1.detach();
	//check if thread is joinable
	/*std::cout << "Number of threads = "
		<< std::thread::hardware_concurrency() << std::endl;*/
	//if (funcTest1.joinable())
	//{
	//	//main is blocked until funcTest1 is not finished
	//	funcTest1.join();
	//}
	if (t2.joinable())    
	{
		//main is blocked until funcTest1 is not finished
		t2.join();
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
	cout <<"Thread ID: "<< functorTest.get_id()<<endl;
	if (functorTest.joinable())
		functorTest.join();
	cout << s1 << endl;
}



void LambdaThreadFunction()
{
	thread lambdaFunc([]()
		{
			cout << "Thread function with lambda function!" << endl;
		}
	);
	lambdaFunc.join();

	//vetor container stores threads, lamda func with param
	vector<thread> workers2;
	for (int i = 0; i < 5;i++)
	{
		workers2.push_back(thread([](int j)
		{
			cout << "Thread function j = " << j << endl;
		}, i));
	}
	cout << "main thread\n";

	for_each(workers2.begin(), workers2.end(), [](thread &t)
	{
		t.join();
	});

	//vetor container stores threads, lambda func without param, the lambda func capture i from outside
	vector<thread> workers;
	for (int i = 0; i < 5; i++)
	{
		workers.push_back(thread([i]()
		{
			cout << "Thread function i="<<i<< endl;
		}));
	}
	cout << "main thread\n";/**/

	for_each(workers.begin(),workers.end(),[](thread &t)
	{
		t.join();
	});

	return;
}

void MoveVariable()
{
	string a = "abc";
	string b="dgef";
	//b = a;
	b = move(a);
}

vector<int> vec;
mutex mLock;
void pushVec()
{
	mLock.lock();
	for (int i = 0; i < 5;i++)
	{
		cout << "push " << i << endl;
		vec.push_back(i);
	}
	mLock.unlock();
}

void popVec()
{
	mLock.lock();
	for (int i = 0; i < 5;i++)
	{
		int val = vec.back();
		vec.pop_back();
		cout << "Pop " << val << endl;
	}
	mLock.unlock();
}

int _tmain(int argc, _TCHAR* argv[])
{
	thread PushT(pushVec);
	_sleep(500);
	thread PopT(popVec);
	

	if (PushT.joinable())
		PushT.join();
	if (PopT.joinable())
		PopT.join();


	//MoveVariable();
	//LambdaThreadFunction();
	//CreateThreadwithMemberFunction();
	//CreateThreadwithFunctor();
	//CreateThreadwithFunction();
	//Functors();
	/*std::cout << "Number of threads = "
		<< std::thread::hardware_concurrency() << std::endl;*/
	return 0;
}

