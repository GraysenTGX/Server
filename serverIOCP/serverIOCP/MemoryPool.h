
/////////////////////////////////////////////////////////////////////////////////////////////////
//Note:
//	persockethandlememorypool is a cache for _PER_SOCKET_HANDLE_DATA and _PER_IO_DATA ,  i allocat    
//	a count of memeory to hold the data struct at the initialize , so the IOCP Server do not need
//	new a data struct when need, it can get the data struct from the pool directly , when finish 
//	the action server can throw the data struct to the pool for reuse.
//	the memeory pool provide two INSTANCE one for _PER_SOCKET_HANDLE_DATA and one for _PER_IO_DATA
//	and provide interface just 
//	(1)MempoolGetData() (2)MempoolThrowData2pool() (3)MempoolIsempty()
//	(4)MempoolReallocate() (5)MempoolUnInitialize() (6)MempoolInitialize()
//	and the mempool reallocate size is read from config
//
//	there have a signal when push data to pool,if the pool size is too large will invoke collect unuse
//	memory, in case the pool eat all the memory
/////////////////////////////////////////////////////////////////////////////////////////////////
//Author:
//	Graysen Tong
//
//Date:
//	2016.11.22
/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <list>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

namespace tmtgx {
	
	template <class T>
	class MemoryPool
	{
	public:
		MemoryPool();
		~MemoryPool();
		typedef						boost::shared_ptr<T> SP_DATATYPE;

	public:
		BOOL						Initialize();
		void						UnInitialize();
		SP_DATATYPE					GetDatafrompool();
		BOOL						ThrowData2pool(SP_DATATYPE data);

	private:
		BOOL						_Isempty() {return !m_curpoolsize;}
		BOOL						_Reallocatemem4pool();
		void						_CollectUnuseMem(); /*if the pool too large after reallocate ,it will collect the unuse mem*/

	private:
		size_t						m_initpoolsize; /*read from config*/
		size_t						m_curpoolsize;
		size_t						m_poollimitedsize; /* if pool size larger than this value will trigger collect*/
		std::list<SP_DATATYPE>		m_datapool; /*real pool*/
		boost::mutex				m_mutex; /*for thread safety*/
		boost::condition_variable	m_condition; /* for collect unuse memory*/

	};

}