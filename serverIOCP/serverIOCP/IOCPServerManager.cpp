#include "stdafx.h"
#include "IOCPServerManager.h"


tmtgx::IOCPServerManager::IOCPServerManager(void)
{
}


tmtgx::IOCPServerManager::~IOCPServerManager(void)
{
}


bool tmtgx::IOCPServerManager::CreateWorkerThreads()
{
	for(int i = 0; i < 2*m_CPUnum; i++){
		m_WorkerThreadHandle[i] = boost::thread(boost::bind(&tmtgx::CIOCPServerbase::WorkerThreadFunc, this, m_WorkerThreadHandle[i].get_id()));
	}
	return true;
}

