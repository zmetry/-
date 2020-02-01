#pragma once
#include<iostream>

const int Max = 100;

using namespace std;

#ifndef _BANK_H_
#define _BANK_H_

class User;
class BankWindow;

template <class T>
class Queqe
{
private:
	struct List
	{
		int id;
		int isWait;
		int ArrayTime;
		int ServerTime;
		char type;
		int member;
		List *next;
	};
	List *list;    //队伍第一个为空
	List *front;    //队头
	List *rear;      //当前位置
public:
	Queqe();
	~Queqe();
	bool Empty();         //空返回1，否则返回0
	bool Full();         //满了返回1，否则返回0
	int TeamCount();       //读有多少个元素
	void TeamFront(T &x);  //读取队头元素
	void TeamNum(T &x, int n);
	void TeamSetIsWait(int n,int _iswait);
	void TeamSetTime(int n,int _time);
	void TeamIn(T &U); //插入数据
	void TeamOut(T &x); //弹出数据
	void TeamShow();     //展示队列

};

template<class T>
void Queqe<T>::TeamSetTime(int n, int _time)
{
	List *temp = front;
	for (int i = 0; i < n; i++)
		temp = temp->next;
	temp->ArrayTime = _time;
}

template <class T>
void Queqe<T>::TeamSetIsWait(int n, int _iswait)
{
	List *temp = front;
	for (int i = 0; i < n; i++)
		temp = temp->next;
	temp->isWait = _iswait;
}

template <class T>
void Queqe<T>::TeamNum(T &x, int n)
{
	List *temp = front;
	for (int i = 0; i < n; i++)
		temp = temp->next;
	x.SetUser(temp->id, temp->isWait, temp->ArrayTime, temp->ServerTime, temp->type);
}

template <class T>
Queqe<T>::Queqe()
{
	List *current = NULL, *temp = NULL;
	list = (List *)malloc(sizeof(List));   //头指针，记载总数
	list->next = NULL;
	list->member = 0;
	front = list;
	rear = NULL;
	temp = list;
	for (int i = 0; i < Max; i++)
	{
		current = (List*)malloc(sizeof(List));
		if (front == list)
			front = current;
		current->next = front;
		current->member = -1;
		temp->next = current;
		temp = temp->next;
	}
}

template <class T>
bool Queqe<T>::Full()
{
	if (rear == front)
		return true;
	else
		return false;
}

template <class T>
bool Queqe<T>::Empty()
{
	if (rear == NULL)
		return true;
	else
		return false;
}

template <class T>
void Queqe<T>::TeamIn(T &U)
{
	List *current = NULL, *prev = NULL;
	if (Full())
		cout << "该队列已满。" << endl;
	else
	{
		if (rear == NULL)
		{
			front->id = U.getid();
			front->isWait = U.getiswait();
			front->ArrayTime = U.getarraytime();
			front->ServerTime = U.getservertime();
			front->type = U.gettype();
			front->member = 1;
			rear = front->next;
		}
		else
		{
			rear->id = U.getid();
			rear->isWait = U.getiswait();
			rear->ArrayTime = U.getarraytime();
			rear->ServerTime = U.getservertime();
			rear->type = U.gettype();
			rear->member = 1;
			rear = rear->next;
		}
		list->member++;
	}
}

template <class T>
void Queqe<T>::TeamOut(T &x)
{
	if (Empty())
		cout << "该队列为空。" << endl;
	else
	{
		List *current = NULL, *prev = NULL;
		List *temp = front;
		x.SetUser(temp->id, temp->isWait, temp->ArrayTime, temp->ServerTime, temp->type);
		list->next = temp->next;
		front = list->next;
		if (front->member != -1)
		{
			rear = front;
			while (rear->next->member != -1 && rear->next->next != front)
				rear = rear->next;
		}
		else
			rear = NULL;

		list->member--;
		free(temp);
		current = (List*)malloc(sizeof(List));
		current->member = -1;
		current->next = front;
		prev = front;
		for (int i = 0; i < Max - 2; i++)
			prev = prev->next;
		prev->next = current;
		if (rear)
			rear = rear->next;
	}
}

template <class T>
int Queqe<T>::TeamCount()
{
	return list->member;
}

template <class T>
void Queqe<T>::TeamFront(T &x)
{
	if (Empty())
		cout << "队列为空。" << endl;
	else
		x.SetUser(front->id, front->isWait, front->ArrayTime, front->ServerTime, front->type);
}

template <class T>
void Queqe<T>::TeamShow()
{
	if (Empty())
		cout << "队列为空。" << endl;
	else
	{
		List *temp = NULL;
		temp = front;
		do
		{
			cout << temp << " ";
			temp = temp->next;
		} while (temp != rear);
	}
}

template <class T>
Queqe<T>::~Queqe()
{
	List *temp = list;
	for (int i = 0; i < Max + 1; i++)
	{
		temp = list->next;
		free(list);
		list = temp;
	}
}

class User
{
protected:
	int id;
	int isWait;      //1为在等待，0为不在
	int ArrayTime;
	int ServerTime;
	char type;
public:
	int getiswait() {
		return isWait;
	}
	int getarraytime() {
		return ArrayTime;
	}
	int getservertime() {
		return ServerTime;
	}
	int getid() {
		return id;
	}
	char gettype() {
		return type;
	};
	virtual User& getServer() {
		isWait = 0;
		return *this;
	};
	void SetisWait(int n){
		isWait = n;
	}
	void SetUser(int _id, int _isWait, int _ArrayTime, int _ServerTime, char _type) {
		id = _id;
		isWait = _isWait;
		ArrayTime = _ArrayTime;
		ServerTime = _ServerTime;
		type = _type;
	}
	User();
	~User() {};
};

class BankWindow
{
protected:
	bool isBusy;
	int id;
	User client;
	char type;
public:
	BankWindow();
	void SetWindow(bool _isBusy, int _id, User _client, char _type) {
		isBusy = _isBusy;
		id = _id;
		client = _client;
		type = _type;
	}
	BankWindow(bool _isbusy, int _id, char _type) {
		isBusy = _isbusy;
		id = _id;
		type = _type;
	}
	int getleavetime() {
		return client.getarraytime() + client.getservertime();
	}
	int getid() {
		return id;
	}
	void SetClient(User &_client) {
		client = _client;
		client.SetisWait(0);
		getmessage();
	}
	void SetisBusy(int n) {
		isBusy = n;
	}
	void getmessage() {
		id = client.getid();
		type = client.gettype();
		isBusy = true;
	}
	bool isbusy() {
		return isBusy;
	}
	virtual void HandleUser() {
	};
};

class NormalUser : public User
{
public:
	NormalUser() {

	}
	NormalUser(int _id, int _isWait, int _ArrayTime, int _ServerTime, char _type) {
		id = _id;
		isWait = _isWait;
		ArrayTime = _ArrayTime;
		ServerTime = _ServerTime;
		type = _type;
	}
	~NormalUser() {};
};

class VIPUser : public User
{
public:
	VIPUser() {

	}
	VIPUser(int _id, int _isWait, int _ArrayTime, int _ServerTime, char _type) {
		id = _id;
		isWait = _isWait;
		ArrayTime = _ArrayTime;
		ServerTime = _ServerTime;
		type = _type;
	}
};

class OfficialUser : public User
{
public:
	OfficialUser() {

	}
	OfficialUser(int _id, int _isWait, int _ArrayTime, int _ServerTime, char _type) {
		id = _id;
		isWait = _isWait;
		ArrayTime = _ArrayTime;
		ServerTime = _ServerTime;
		type = _type;
	}
};

class NormalBankWindow : public BankWindow
{
public:
	NormalBankWindow(){

	}
	NormalBankWindow(bool _isBusy, int _id, User _client, char _type)
	{
		isBusy = _isBusy;
		id = _id;
		client = _client;
		type = _type;
	}
	void HandleUser();
};

class VIPBankWindow : public BankWindow
{
public:
	VIPBankWindow() {

	}
	VIPBankWindow(bool _isBusy, int _id, User _client, char _type)
	{
		isBusy = _isBusy;
		id = _id;
		client = _client;
		type = _type;
	}
	char gettype() {
		return type;
	}
	int getid() {
		return id;
	}
	void HandleUser();
};

class OfficialBankWindow : public BankWindow
{
public:
	OfficialBankWindow() {

	}
	OfficialBankWindow(bool _isBusy, int _id, User _client, char _type)
	{
		isBusy = _isBusy;
		id = _id;
		client = _client;
		type = _type;
	}
	char gettype() {
		return type;
	}
	int getid() {
		return id;
	}
	void HandleUser();
};

class Simulater
{
private:
	int normalwaitcount;
	int vipwaitcount;
	int officialwaitcount;
	int time;      //计时装置
	int count;
	NormalBankWindow normalwindow[3];
	VIPBankWindow vipwindow;
	OfficialBankWindow officialwindow;
	Queqe<User> NormalQue;
	Queqe<User> VIPQue;
	Queqe<User> OfficialQue;
public:
	Simulater();                 //初始化函数
	void CallNumber();          //叫号函数
	void CustomerEnter();
	void SimulaterOn();
	void waitcount();
	void CustomerLeave();
};


#endif // !_BANK_H_


