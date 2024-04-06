#ifndef __CLOCK_OBSERVER_HEADER__
#define __CLOCK_OBSERVER_HEADER__


#include "IObserver.h"
#include <chrono>
#include <stdlib.h>
#include <string>


namespace chr = std::chrono;


class ClockObserver : public IObserver<chr::duration<double>>
{
public:
	ClockObserver(const char* name) 
		: IObserver<chr::duration<double>>()
		, m_name(name)
	{}
	~ClockObserver()
	{}

	virtual void OnNext(const chr::duration<double>& value) override
	{
		fprintf(stdout, "%s : %lf(sec).\n", m_name.c_str(), value.count());
	}

	virtual void OnCompleted() override
	{
		fprintf(stderr, "%s : Completed.\n", m_name.c_str());
	}

	virtual void OnError(std::exception const* pError) override
	{
		if (nullptr != pError)
		{
			fprintf(stderr, "%s : %s.\n", m_name.c_str(), pError->what());
		}
	}
private:
	std::string m_name;

};




#endif // !__CLOCK_OBSERVER_HEADER__