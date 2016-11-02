#include "stdafx.h"
#include "IOCPServerManager.h"

boost::mutex	   tmtgx::IOCPServerManager::m_mutex;
tmtgx::IOCPServerManager* tmtgx::IOCPServerManager::m_iocpservermanager_ptr = NULL;
tmtgx::IOCPServerManager::IOCPServerManager(void)
{
	m_worker_started = false;
}


tmtgx::IOCPServerManager::~IOCPServerManager(void)
{
}

bool tmtgx::IOCPServerManager::Initialize()
{
	// init iocp server work env and create iocp handle 
}

DWORD tmtgx::IOCPServerManager::GetCPUNum()
{
	_SYSTEM_INFO sys_info;
	::GetSystemInfo(&sys_info);
	m_CPUnum = sys_info.dwNumberOfProcessors;
	return m_CPUnum;
}

bool tmtgx::IOCPServerManager::Start()
{
	if(m_worker_started) {
		return true;
	}
	m_worker_started = CreateWorkerThreads();
	return m_worker_started;
}

void tmtgx::IOCPServerManager::Stop()
{
	std::vector<boost::shared_ptr<boost::thread>>::iterator p_thread = m_WorkerThreadVector.begin();
	for (; p_thread != m_WorkerThreadVector.end(); ++p_thread) {
		(*p_thread)->interrupt();
		if( (*p_thread)->joinable() ) {
			(*p_thread)->join();
		}
	}
}


tmtgx::IOCPServerManager* tmtgx::IOCPServerManager::GetInstance()
{
	boost::mutex::scoped_lock(m_mutex);
	if(NULL == m_iocpservermanager_ptr) {
		m_iocpservermanager_ptr = new IOCPServerManager();
	}
	return m_iocpservermanager_ptr;
}

bool tmtgx::IOCPServerManager::CreateWorkerThreads()
{
	try {
		for(int i = 0; i < 2*m_CPUnum; i++){
			m_WorkerThreadVector.push_back(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&m_iocpserver.LongTimeWorker),&m_iocpserver)));
		}
		return true;
	}
	catch(...)
	{
		return false;
	}
}

