#include"Bank.h"
#include<iostream>
#include<iomanip>
using namespace std;

void NormalBankWindow::HandleUser()
{
	SetClient(client.getServer());
}

void VIPBankWindow::HandleUser()
{
	SetClient(client.getServer());
}

void OfficialBankWindow::HandleUser()
{
	SetClient(client.getServer());
}

BankWindow::BankWindow()
{
	isBusy = 0;
	id = 0;
	type = '无';
}

User::User()
{
	id = 0;
	isWait = 0;
	ArrayTime = 0;
	ServerTime = 0;
	type = '无';
}

Simulater::Simulater()
{
	count = 0;
	time = 0;
	vipwaitcount = 0;
	normalwaitcount = 0;
	officialwaitcount = 0;
}

void Simulater::CallNumber()
{
	NormalUser nu, skt;
	VIPUser vu;
	OfficialUser ou;
	int count = 0;
	//普通窗口判断
	for (int i = 0; i < 3; i++)
	{
		if (!normalwindow[i].isbusy())
		{
			if (!NormalQue.Empty())
			{			
				NormalQue.TeamOut(nu);
				while (!nu.getiswait())
				{
					NormalQue.TeamNum(nu, count++);
					if (count == NormalQue.TeamCount() + 1)
						goto loop;
				}
				normalwindow[i].SetClient(nu);
				count = 0;
				cout << "请" << nu.getid() << "号到第" << i + 1 << "号普通窗口办理业务。" << endl;
				normalwindow->HandleUser();
				if (!NormalQue.Empty())
				{
					NormalQue.TeamFront(skt);
					if (skt.getid() == nu.getid())
						NormalQue.TeamOut(nu);
				}
					
			}		
		}
	}

	//VIP窗口判断,当普通人在时有VIP来，普通重置为0
	if (!vipwindow.isbusy())
	{
		if (!VIPQue.Empty())
		{
			VIPQue.TeamOut(vu);
			vipwindow.SetClient(vu);
			cout << "请" << vu.getid() << "号到VIP窗口办理业务。" << endl;
			vipwindow.HandleUser();
		}
		else
		{
			if (!NormalQue.Empty())
			{
				NormalQue.TeamFront(nu);
				while (!nu.getiswait())
				{
					NormalQue.TeamNum(nu, count++);
					if (count == NormalQue.TeamCount())
						goto loop;
				}	
				NormalQue.TeamSetIsWait(count, 0);
				count = 0;
				vipwindow.SetClient(nu);
				cout << "请" << nu.getid() << "号到VIP窗口办理业务。" << endl;
				vipwindow.HandleUser();
			}
		}
	}
	else
	{
		if (vipwindow.gettype() == 'N' && !VIPQue.Empty())
		{
			NormalQue.TeamFront(nu);
			while (nu.getid() != vipwindow.getid())
				NormalQue.TeamNum(nu, ++count);
			NormalQue.TeamSetIsWait(count, 1);
			count = 0;
			VIPQue.TeamOut(vu);
			vipwindow.SetClient(vu);
			cout << "请" << vu.getid() << "号到VIP窗口办理业务。" << endl;
			vipwindow.HandleUser();
		}
	}

	//公务员窗口判断
	if (!officialwindow.isbusy())
	{
		if (!OfficialQue.Empty())
		{
			OfficialQue.TeamOut(ou);
			officialwindow.SetClient(ou);
			cout << "请" << ou.getid() << "号到对公用户窗口办理业务。" << endl;
			officialwindow.HandleUser();
		}
		else
		{
			if (!NormalQue.Empty())
			{
				NormalQue.TeamFront(nu);
				while (!nu.getiswait())
				{
					NormalQue.TeamNum(nu, ++count);
					if (count == NormalQue.TeamCount())
						goto loop;
				}
				NormalQue.TeamSetIsWait(count, 0);
				count = 0;
				officialwindow.SetClient(nu);
				cout << "请" << nu.getid() << "号到对公用户窗口办理业务。" << endl;
				officialwindow.HandleUser();
			}	
		}
	}
	else
	{
		if (officialwindow.gettype() == 'N' && !OfficialQue.Empty())
		{
			NormalQue.TeamFront(nu);
			while (nu.getid() != officialwindow.getid())
				NormalQue.TeamNum(nu, ++count);
			NormalQue.TeamSetIsWait(count, 1);
			count = 0;
			OfficialQue.TeamFront(ou);
			officialwindow.SetClient(ou);
			cout << "请" << ou.getid() << "号到对公用户窗口办理业务。" << endl;
			officialwindow.HandleUser();
		}
	}

	//增加在队列中数字的时间
loop: 
	for (int i = 0; i < NormalQue.TeamCount(); i++)
	{
		NormalQue.TeamNum(nu, i);
		if (nu.getiswait() == true)
			NormalQue.TeamSetTime(i, time);
	}
	for (int i = 0; i < VIPQue.TeamCount(); i++)
	{
		VIPQue.TeamNum(vu, i);
		if (vu.getiswait() == true)
			VIPQue.TeamSetTime(i, time);
	}
	for (int i = 0; i < OfficialQue.TeamCount(); i++)
	{
		OfficialQue.TeamNum(ou, i);
		if (vu.getiswait() == true)
			VIPQue.TeamSetTime(i, time);
	}


	cout << endl;
}

void Simulater::CustomerEnter()
{
	NormalUser nu;
	VIPUser vu;
	OfficialUser ou;
	char ch;
	int number = 0;
	cout << "是否有顾客进入（Y/N）:";
	cin >> ch;
	if (ch == 'Y')
	{
		cout << "请输入顾客几名：";
		cin >> number;
		for (int i = 0; i < number; i++)
		{
	loop:	cout << "请输入第" << i + 1 << "位顾客信息(N表示普通,V表示VIP,O表示公用)：";
			cin >> ch;
			switch (ch)
			{
			case 'N':
				nu.SetUser(++count, 1, time, 4, 'N');
				NormalQue.TeamIn(nu);
				break;
			case 'V':
				vu.SetUser(++count, 1, time, 4, 'V');
				VIPQue.TeamIn(vu);
				break;
			case 'O':
				ou.SetUser(++count, 1, time, 4, 'O');
				OfficialQue.TeamIn(ou);
				break;
			default:
				cout << "请按规则输入。" << endl;
				goto loop;
			}
		}
	}
	time += 2;
}

void Simulater::SimulaterOn()
{
	int width = 20;
	int width_1 = 15;
	while (1)
	{
		//窗口定义
		cout << "第" << time << "秒" << endl;
		cout << endl;
		CustomerLeave();
		CallNumber();
		for (int i = 0; i < 3; i++)
		{
			
			cout << std::right << setw(width) << "普通窗口" << i + 1;
		}
		cout << std::right << setw(width) << "VIP窗口";
		cout << std::right << setw(width) << "对公用窗口";
		cout << endl;
		for (int i = 0; i < 3; i++)
		{
			if (normalwindow[i].isbusy())
			{
				cout << std::right << setw(width_1 - i * 2) << "正在为" << normalwindow[i].getid() << "号服务。";
			}
			else
				cout << std::right << setw(width) << "提供服务";
		}
		if (vipwindow.isbusy())
			cout << std::right << setw(width_1 - 2) << "正在为" << vipwindow.getid() << "号服务。";
		else
			cout << std::right << setw(width ) << "提供服务";

		if (officialwindow.isbusy())
			cout << std::right << setw(width_1 - 4) << "正在为" << officialwindow.getid() << "号服务。";
		else
			cout << std::right << setw(width) << "提供服务";

		cout << '\n' << '\n' << endl;
		
		waitcount();
		cout << "普通用户等待人数为" << normalwaitcount << ",VIP用户等待人数为" << vipwaitcount << ",对公用户等待人数为" << officialwaitcount << endl;
		cout << endl;
		CustomerEnter();
		system("pause");
		system("cls");
	}
}

void Simulater::CustomerLeave()
{
	for (int i = 0; i < 3; i++)
		if (normalwindow[i].getleavetime() == time)
			normalwindow[i].SetisBusy(0);
	if (vipwindow.getleavetime() == time)
		vipwindow.SetisBusy(0);
	if (officialwindow.getleavetime() == time)
		officialwindow.SetisBusy(0);
}

void Simulater::waitcount()
{
	normalwaitcount = 0;
	vipwaitcount = 0;
	officialwaitcount = 0;
	NormalUser nu;
	VIPUser vu;
	OfficialUser ou;
	for (int i = 0; i < NormalQue.TeamCount(); i++)
	{
		NormalQue.TeamNum(nu, i);
		if (nu.getiswait() == 1)
			normalwaitcount++;
	}
	for (int i = 0; i < VIPQue.TeamCount(); i++)
	{
		VIPQue.TeamNum(vu, i);
		if (vu.getiswait() == 1)
			vipwaitcount++;
	}
	for (int i = 0; i < OfficialQue.TeamCount(); i++)
	{
		OfficialQue.TeamNum(ou, i);
		if (ou.getiswait() == 1)
			officialwaitcount++;
	}

}
