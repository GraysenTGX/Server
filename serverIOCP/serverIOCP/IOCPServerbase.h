#ifndef _IOCPSERVER_BASE_H_
#define	_IOCPSERVER_BASE_H_

#include <WinSock2.h>
#include <string.h>
#include <MSWSock.h>  //for acceptex
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#define LPFN_ACCEPTEX LPVOID
#define BUFFER_SIZE 4*1024

namespace tmtgx{

	enum op
	{
		OP_READ	 =	1,
		OP_WRITE =	2,
		OP_ACCEPT =	3
	};

	typedef struct _PER_SOCKET_HANDLE_DATA		//bind to IOCP PORT with socket handle together
	{
		SOCKET			cursocket;
		sockaddr_in		localaddr;
		sockaddr_in		remoteaddr;
		int				m_postSendEventNum;
		int				m_postRecvEventNum;
		BOOL			isclosed;
	}Per_Socket_Handle_Data,*pPer_Socket_Handle_Data;

	
	typedef struct _PER_IO_DATA
	{
		OVERLAPPED*		poverlap;					//this variable must put at the first place of _PER_IO_DATA
		char			buffer[BUFFER_SIZE];		//data buffer
		int				op_type;					//option
	}Per_IO_Data,*pPer_IO_Data;


	class CIOCPServerbase
	{
	public:
		CIOCPServerbase(void);
		virtual						~CIOCPServerbase(void);

	public:
		bool						Initialize();
		void						UnInitilize();		
		DWORD						GetCPUNum();
		BOOL						CreateNewCompletionPort(DWORD WorkThreadNum);
		BOOL						Associate2CompletionHandle(HANDLE &completionPort, HANDLE &newDevice, DWORD &hCmpletionKey);
		HANDLE						CreateNewClientSocketHandle();
		HANDLE						*GetIOCPortHandle(){return &m_IOCPort;}
		SOCKET						*GetListenSocket(){return &m_ListenSocket;}
		BOOL						PostAcceptEvent2IOCPHandle();
		BOOL						PostRecvEvent2IOCPHandle(SOCKET &clientsocket);
		BOOL						PostSendEvent2IOCPHandle(SOCKET &clientsocket);

	public:
		virtual						void RecvDataProc(_PER_SOCKET_HANDLE_DATA &per_handle_data, 
														_PER_IO_DATA &per_io_data);
		virtual						void SendDataProc(_PER_SOCKET_HANDLE_DATA &per_handle_data, 
														_PER_IO_DATA &per_io_data);

	private:
		BOOL						ListenSocketInit(std::string ip, unsigned short listenPort);
		BOOL						GetAcceptEXFuncAddress();
		void						WorkerThreadFunc(boost::thread::id id);
		Per_Socket_Handle_Data*		CreateNewHandle4Socket();
		Per_IO_Data*				CreateNewIOData();
		BOOL						SocketEnvInitialize();

	
	private:
		HANDLE						m_IOCPort;
		DWORD						m_CPUnum;
		bool						m_is_inited;
		SOCKET						m_ListenSocket;
		LPFN_ACCEPTEX				m_AcceptEXAddress;

	};

}
#endif;