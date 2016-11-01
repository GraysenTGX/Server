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

	};

}
#endif;

