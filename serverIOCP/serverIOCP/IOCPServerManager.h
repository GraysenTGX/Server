#ifndef _IOCP_SERVER_MANAGER_H_
#define _IOCP_SERVER_MANAGER_H_

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

#include "IOCPServer.h"

namespace tmtgx {

	class IOCPServerManager
	{
	public:
		IOCPServerManager(void);
		~IOCPServerManager(void);

	public:
		bool						Start();
		bool						Stop();
		bool						CreateWorkerThreads();


	private:
		std::vector<boost::shared_ptr<boost::thread*>>		m_WorkerThreadVector;
		IOCPServer					m_iocpserver;
		

	};
}
#endif;

