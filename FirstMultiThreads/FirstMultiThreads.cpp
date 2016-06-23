// FirstMultiThreads.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>
#include <list>

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

void makeCall()
{
	mLock.lock();
	cout << "Hello my friend, this is thread: " << this_thread::get_id() << endl;
	mLock.unlock();
}

void MutexVector()
{
	thread PushT(pushVec);
	this_thread::sleep_for(chrono::milliseconds(500));
	thread PopT(popVec);
	

	if (PushT.joinable())
		PushT.join();
	if (PopT.joinable())
		PopT.join();
}

void MutexCoutResource()
{
	thread person1(makeCall);
	thread person2(makeCall);
	thread person3(makeCall);
	mLock.lock();
	cout << "2nd Hello my friend, this is thread: " << person1.get_id() << endl;
	cout << "2nd Hello my friend, this is thread: " << person2.get_id() << endl;
	cout << "2nd Hello my friend, this is thread: " << person3.get_id() << endl;
	mLock.unlock();
	if (person1.joinable())
		person1.join();
	if (person2.joinable())
		person2.join();
	if (person3.joinable())
		person3.join();
}

int g_i = 0;
list<int> intervalList;

void addToList(int max, int interval)
{
	lock_guard<mutex> guard(mLock);
	g_i++;
	for (int i = 0; i < max; i++)
		if ((i%interval) == 0)
			intervalList.push_back(i);
	// mLock is automatically released when lock goes out of scope
}

void printList()
{
	lock_guard<mutex> guard2(mLock);
	for (auto itr = intervalList.begin(), enditr = intervalList.end(); itr != enditr; itr++)
		cout << *itr << ", ";
	// mLock is automatically released when lock goes out of scope
}

void LockGuard()
{
	int max = 50;
	thread t1(addToList, max, 2);
	thread t2(addToList, max, 10);
	thread t3(printList);

	if (t1.joinable())
		t1.join();
	if (t2.joinable())
		t2.join();
	if (t3.joinable())
		t3.join();
}

class MyStack
{
public:
	MyStack() {};
	~MyStack() {};
	void pop();
	int top(){ return data.back(); }
	void push(int n);
	int getSize(){ return data.size(); }
private:
	vector<int> data;
};

void MyStack::pop()
{
	lock_guard<mutex> guard(mLock);
	data.pop_back();
}

void MyStack::push(int n)
{
	lock_guard<mutex> guard(mLock);
	data.push_back(n);
}

void ProcessMyStack(int val, string s)
{
	lock_guard<mutex> guard(mLock);
	cout << s << " : " << val << endl;
}

void ProcessMyStackThread(MyStack& st, string s)
{
	int val = st.top();
	st.pop();
	ProcessMyStack(val, s);
	//this_thread::sleep_for(chrono::milliseconds(10));
}

void ProcessMyStackThread2(MyStack& st, string s)
{
	int val = st.top();
	st.pop();
	this_thread::sleep_for(chrono::milliseconds(10));
	ProcessMyStack(val, s);
}


void RaceConditions1()
{
	//Two threads access the same stack cause race condition
	MyStack st;
	for (int i = 0; i < 30; i++)  st.push(i);

	while (true) {
		if (st.getSize() > 0) {
			thread t1(&ProcessMyStackThread, ref(st), string("thread1"));
			t1.join();
		}
		else
			break;
		if (st.getSize() > 0) {
			thread t2(&ProcessMyStackThread, ref(st), string("thread2"));
			t2.join();
		}
		else
			break;
	}
}

mutex myMutex;

void shared_cout(int i)
{
	lock_guard<mutex> guard(mLock);
	cout << i << " " << endl;
}

void Print10Num(int n)
{
	for (int i = 10 * (n - 1); i < 10 * n;i++)
	{
		shared_cout(i);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	thread t1(Print10Num, 1);  //0-9
	thread t2(Print10Num, 2);  //10-19
	thread t3(Print10Num, 3);  //20-29
	thread t4(Print10Num, 4);  //30-39
	thread t5(Print10Num, 5);  //40-49

	for (int i = 0; i > -50; i--)
		shared_cout(i);    //(0,-49)

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();


	//RaceConditions1();
	//LockGuard();
	//MutexCoutResource();
	//MutexVector();
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

