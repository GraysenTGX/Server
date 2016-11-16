#ifndef _IOCPSERVER_BASE_H_
#define	_IOCPSERVER_BASE_H_

#include <WinSock2.h>
#include <string.h>
#include <MSWSock.h>  //for acceptex
#include <stack>
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
		OP_NULL	 =  -1,
		OP_READ	 =	1,
		OP_WRITE =	2,
		OP_ACCEPT =	3
	};

	typedef struct _PER_SOCKET_HANDLE_DATA		//bind to IOCP PORT with socket handle together
	{
		SOCKET			m_cursocket;
		sockaddr_in		m_localaddr;
		sockaddr_in		m_remoteaddr;
		int				m_postSendEventNum;
		int				m_postRecvEventNum;
		BOOL			m_isclosed;
	}Per_Socket_Handle_Data,*pPer_Socket_Handle_Data;

	
	typedef struct _PER_IO_DATA
	{
		OVERLAPPED		m_poverlap;				//this variable must put at the first place of _PER_IO_DATA
		SOCKET			m_socket;					//current option's socket
		WSABUF			m_wsabuf;					//wsa buffer
		char			m_buffer[BUFFER_SIZE];		//data buffer
		int				m_op_type;					//option

		_PER_IO_DATA()
		{
			m_op_type      = OP_NULL;
			m_wsabuf.buf   = m_buffer;
			m_wsabuf.len   = BUFFER_SIZE;
			m_socket	   = INVALID_SOCKET;
			ZeroMemory(m_buffer, BUFFER_SIZE);
			ZeroMemory(&m_poverlap, sizeof(OVERLAPPED));
		}

		~_PER_IO_DATA()
		{
			if(m_socket != INVALID_SOCKET) {
				closesocket(m_socket);
				m_socket = INVALID_SOCKET;
			}
		}
	}Per_IO_Data,*pPer_IO_Data;

	class CClientSocketPool
	{
	public:
		CClientSocketPool();
		~CClientSocketPool();
		typedef						boost::shared_ptr<SOCKET> SHARED_SOCKET;

	public:
		BOOL						Initialize() {return m_curpoolsize;}
		BOOL						Push(SHARED_SOCKET m_clientsock_ptr);
		SHARED_SOCKET				Pop();
		BOOL						Isfull() {return m_maxpoolsize == m_curpoolsize;}
		BOOL						Isempty() {return !m_curpoolsize;}

	private:
		void						CreateClientSocketPool(); /*manage the socket by stack init the socket pool at initialize*/

	private:
		std::stack<SHARED_SOCKET>	m_socketpool; /*real pool*/
		boost::mutex				m_mutex; /*for thread safety*/
		size_t						m_maxpoolsize; /*read from config*/
		size_t						m_curpoolsize;

	};

	class CIOCPServerbase
	{
	public:
		CIOCPServerbase(void);
		virtual						~CIOCPServerbase(void);

	public:
		bool						Initialize();
		void						UnInitialize();		
		BOOL						CreateNewCompletionPort(DWORD WorkThreadNum);
		BOOL						Associate2CompletionHandle(HANDLE &completionPort, HANDLE &newDevice, DWORD &hCmpletionKey);
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
		Per_Socket_Handle_Data*		CreateNewHandle4Socket();
		Per_IO_Data*				CreateNewIOData();
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