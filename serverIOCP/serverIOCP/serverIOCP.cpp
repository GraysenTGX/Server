// serverIOCP.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include "IOCPServerbase.h"

#include <stdlib.h>
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	tmtgx::CIOCPServerbase serverbase;
	int m = serverbase.GetCPUNum();
	cout<<"cpu num:"<<m<<endl;
	serverbase.CreateWorkerThreads();
	int i = 0;
	while(i<10)
	{
		cout<<serverbase.getteststr()<<endl;
		i++;
	}
	system("pause");
	return 0;
}

