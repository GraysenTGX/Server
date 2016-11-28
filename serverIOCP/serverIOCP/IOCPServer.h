#ifndef _IOCP_SERVER_H_
#define _IOCP_SERVER_H_

#include "MemoryPool.h"
#include "PerIOEvent.h"
#include "PerSocketEvent.h"
#include "iocpserverbase.h"

namespace tmtgx {

	class IOCPServer :
		public CIOCPServerbase
	{
	public:
		IOCPServer(void);
		~IOCPServer(void);

	public:
		void	LongTimeWorker();
		virtual void	RecvDataProc(boost::shared_ptr<_PER_SOCKET_HANDLE_DATA>, boost::shared_ptr<_PER_IO_DATA>);
		virtual void	SendDataProc(boost::shared_ptr<_PER_SOCKET_HANDLE_DATA>, boost::shared_ptr<_PER_IO_DATA>);

	private:
		tmtgx::MemoryPool<_PER_SOCKET_HANDLE_DATA> m_persockethandlepool; /* memeory pool */
		tmtgx::MemoryPool<_PER_IO_DATA>			   m_periodatapool;
	};

}
#endif;

