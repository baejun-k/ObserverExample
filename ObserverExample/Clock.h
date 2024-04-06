#ifndef __CLOCK_HEADER__
#define __CLOCK_HEADER__


#include "ObservableBase.h"
#include "ThreadSafeObservableBase.h"
#include <chrono>
#include <stdlib.h>


#define ENABLE_THREAD_SAFE 1


namespace chr = std::chrono;


#if ENABLE_THREAD_SAFE
using ObservableType = ThreadSafeObservableBase<chr::duration<double>>;
#else
using ObservableType = ObservableBase<chr::duration<double>>;
#endif


class Clock : public ObservableType
{
public:
	Clock()
		: ObservableType()
		, m_startTime(chr::system_clock::now())
		, m_sec(chr::duration<double>(0))
	{}
	virtual ~Clock()
	{}

	void Updated()
	{
		printf("Updated Start======================\n");
		m_sec = chr::system_clock::now() - m_startTime;

		this->Notify(m_sec);

		printf("Updated End========================\n\n");
	}

	double GetDuration()
	{
		return m_sec.count();
	}

private:
	chr::system_clock::time_point m_startTime;
	chr::duration<double> m_sec;

};


#endif // !__CLOCK_HEADER__