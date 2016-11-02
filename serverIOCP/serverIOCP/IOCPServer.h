#ifndef _IOCP_SERVER_H_
#define _IOCP_SERVER_H_

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
		void	RecvDataProc(_PER_SOCKET_HANDLE_DATA &per_handle_data, _PER_IO_DATA &per_io_data);
		void	SendDataProc(_PER_SOCKET_HANDLE_DATA &per_handle_data, _PER_IO_DATA &per_io_data);
	};

}
#endif;

