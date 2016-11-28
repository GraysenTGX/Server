#include "stdafx.h"
#include "MemoryPool.h"


namespace tmtgx {
	template<class T>
	MemoryPool<T>::MemoryPool()
	{
		m_maxpoolsize = 0;
		m_curpoolsize = 0;
	};

	template<class T>
	MemoryPool<T>::~MemoryPool()
	{
		while(m_socketpool.size()) {
			m_socketpool.pop();
		}
	};

	template<class T>
	void MemoryPool<T>::Initialize()
	{

	};

	template<class T>
	BOOL MemoryPool<T>::ThrowData2pool(SP_DATATYPE data)
	{
		boost::mutex::scoped_lock lock(m_mutex);
		m_datapool.push_front(data);
		m_curpoolsize ++;
		if (m_curpoolsize > m_poollimitedsize) {
			_CollectUnuseMem();
		}
	};

	template<class T>
	MemoryPool<T>::SP_DATATYPE MemoryPool<T>::GetDatafrompool()
	{
		boost::mutex::scoped_lock lock(m_mutex);
		if (_MempoolIsempty()) {
			if (_Reallocatemem4pool()) {
				SP_DATATYPE temp = m_datapool.front();
				m_datapool.pop_front();
				m_curpoolsize --;
				return temp;
			} else {
				return NULL;
			}
		} else {
			SP_DATATYPE temp = m_datapool.front();
			m_datapool.pop_front();
			m_curpoolsize --;
			return temp;
		}
	};

	template<class T>
	void MemoryPool<T>::_CollectUnuseMem() 
	{
		boost::mutex::scoped_lock lock(m_mutex);
		for (int i = m_initpoolsize; i < m_curpoolsize; ++i) {
			m_datapool.pop_back();
		}
	};

	template<class T>
	BOOL MemoryPool<T>::_Reallocatemem4pool() /* the reallocate size is the multi of m_initpoolsize*/ 
	{
		boost::mutex::scoped_lock lock(m_mutex);
		for (int i = 0; i< m_initpoolsize; ++i) {
			m_datapool.push_back(new T);
		}
		return !m_datapool.empty();
	};
};
