#pragma warning(disable : 4996)


#include "Clock.h"
#include "ClockObserver.h"
#include <memory>


int main()
{
	Clock observableClock;
	std::shared_ptr<ClockObserver> clockObserver0 = std::make_shared<ClockObserver>("0");
	std::shared_ptr<ClockObserver> clockObserver1 = std::make_shared<ClockObserver>("1");
	std::shared_ptr<ClockObserver> clockObserver2 = std::make_shared<ClockObserver>("2");

#if ENABLE_THREAD_SAFE
	std::function<void()> unsubscriber0 = observableClock.Subscribe(clockObserver0);
#else
	std::function<void()> unsubscriber0 = observableClock.Subscribe(clockObserver0.get());
#endif
	observableClock.Updated();

#if ENABLE_THREAD_SAFE
	std::function<void()> unsubscriber1 = observableClock.Subscribe(clockObserver1);
#else
	std::function<void()> unsubscriber1 = observableClock.Subscribe(clockObserver1.get());
#endif
	observableClock.Updated();

#if ENABLE_THREAD_SAFE
	std::function<void()> unsubscriber2 = observableClock.Subscribe(clockObserver2);
#else
	std::function<void()> unsubscriber2 = observableClock.Subscribe(clockObserver2.get());
#endif
	observableClock.Updated();

	unsubscriber0();
	observableClock.Updated();

	unsubscriber2();
	observableClock.Updated();

	observableClock.Completed();


	return 0;
}