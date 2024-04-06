#pragma warning(disable : 4996)


#include "Clock.h"
#include "ClockObserver.h"
#include <memory>


int main()
{
	std::shared_ptr<Clock> pClock = std::make_shared<Clock>();
	std::shared_ptr<ClockObserver> clockObserver0 = std::make_shared<ClockObserver>("0");
	std::shared_ptr<ClockObserver> clockObserver1 = std::make_shared<ClockObserver>("1");
	std::shared_ptr<ClockObserver> clockObserver2 = std::make_shared<ClockObserver>("2");

#if ENABLE_THREAD_SAFE
	std::function<void()> unsubscriber0 = pClock->Subscribe(clockObserver0);
#else
	std::function<void()> unsubscriber0 = pClock->Subscribe(clockObserver0.get());
#endif
	pClock->Updated();

#if ENABLE_THREAD_SAFE
	std::function<void()> unsubscriber1 = pClock->Subscribe(clockObserver1);
#else
	std::function<void()> unsubscriber1 = pClock->Subscribe(clockObserver1.get());
#endif
	pClock->Updated();

#if ENABLE_THREAD_SAFE
	std::function<void()> unsubscriber2 = pClock->Subscribe(clockObserver2);
#else
	std::function<void()> unsubscriber2 = pClock->Subscribe(clockObserver2.get());
#endif
	pClock->Updated();

	unsubscriber0();
	pClock->Updated();

	unsubscriber2();
	pClock->Updated();

	pClock->Completed();

	return 0;
}