#ifndef _IOCPSERVER_BASE_H_
#define	_IOCPSERVER_BASE_H_

#include <WinSock2.h>
#include <string.h>
#include <MSWSock.h>  //for acceptex
#include <stack>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "PerIOEvent.h"
#include "PerSocketEvent.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")

namespace tmtgx{
	
	class CIOCPServerbase
	{
	public:
		CIOCPServerbase(void);
		virtual						~CIOCPServerbase(void);

	public:
		bool						Initialize();
		void						UnInitialize();		
		BOOL						CreateNewCompletionPort(DWORD WorkThreadNum);
		BOOL						Associate2CompletionHandle(HANDLE completionPort, boost::shared_ptr<_PER_SOCKET_HANDLE_DATA>);
		HANDLE						*GetIOCPortHandle(){return &m_IOCPort;}
		SOCKET						*GetListenSocket(){return &m_ListenSocket;}
		BOOL						PostAcceptEvent2IOCPHandle(boost::shared_ptr<tmtgx::_PER_IO_DATA>);
		BOOL						PostRecvEvent2IOCPHandle(boost::shared_ptr<tmtgx::_PER_IO_DATA>);
		BOOL						PostSendEvent2IOCPHandle(boost::shared_ptr<tmtgx::_PER_IO_DATA>);

	public:
		virtual						void RecvDataProc(boost::shared_ptr<tmtgx::_PER_SOCKET_HANDLE_DATA> per_handle_data, 
														boost::shared_ptr<tmtgx::_PER_IO_DATA> per_io_data);
		virtual						void SendDataProc(boost::shared_ptr<tmtgx::_PER_SOCKET_HANDLE_DATA> per_handle_data, 
														boost::shared_ptr<tmtgx::_PER_IO_DATA> per_io_data);

	private:
		BOOL						ListenSocketInit(std::string ip, unsigned short listenPort);
		BOOL						GetAcceptEXFuncAddress();
		BOOL						SocketEnvInitialize();

	protected:
		LPFN_ACCEPTEX				m_AcceptEXAddress;

	private:
		HANDLE						m_IOCPort;
		bool						m_is_inited;
		SOCKET						m_ListenSocket;
	};

}
#endif;