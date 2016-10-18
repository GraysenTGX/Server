#pragma once

#include <WinSock2.h>
#include <boost/thread.hpp>

#define LPFN_ACCEPTEX LPVOID


namespace tmtgx{

	class CIOCPServerbase
	{
	public:
		CIOCPServerbase(void);
		virtual ~CIOCPServerbase(void);

	public:
		DWORD GetCPUNum();
		BOOL SocketEnvInitialize();
		BOOL CreateNewCompletionPort(DWORD WorkThreadNum);
		BOOL Associate2CompletionHandle(HANDLE &completionPort, HANDLE &newDevice, DWORD &hCmpletionKey);
		LPFN_ACCEPTEX GetAcceptEXFuncAddress();
		HANDLE CreateNewHandle();
		HANDLE GetIOCPortHandle();
		BOOL run();
		BOOL stop();


	public:
		virtual void RecvDataProc(HANDLE &device, HANDLE &IOComloetionKey);
		virtual void SendDataProc(HANDLE &device, HANDLE &IOComloetionKey);

	private:
		HANDLE IOCPort;
		LPFN_ACCEPTEX m_AcceptEXAddress;
		boost::thread *WorkerThreadHandle;

	};

}
