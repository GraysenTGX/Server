#ifndef _IOCP_SERVER_MANAGER_H_
#define _IOCP_SERVER_MANAGER_H_

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <boost/thread/mutex.hpp>

#include "IOCPServer.h"

namespace tmtgx {

	class IOCPServerManager
	{
	public:
		~IOCPServerManager(void);

	public:
		bool						Start();
		void						Stop();
		bool						Initialize();
		bool						CreateWorkerThreads();
		static	IOCPServerManager * GetInstance();

	private:
		DWORD						GetCPUNum();
		IOCPServerManager(void); // For Singleleton

	public:
		DWORD						m_CPUnum;

	private:
		std::vector<boost::shared_ptr<boost::thread>>		m_WorkerThreadVector;
		IOCPServer					m_iocpserver;
		static boost::mutex			m_mutex;
		static IOCPServerManager*	m_iocpservermanager_ptr;
		bool						m_worker_started;

	};
}
#endif;

