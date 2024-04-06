#ifndef __INTERFACE_OBSERVER_HEADER__
#define __INTERFACE_OBSERVER_HEADER__


#include <exception>


template<typename Ty>
class IObserver
{
protected:
	IObserver() {}

public:
	virtual ~IObserver() {}

	virtual void OnNext(const Ty& value) = 0;
	virtual void OnCompleted() = 0;
	virtual void OnError(std::exception const* error) = 0;
};


#endif // !__INTERFACE_OBSERVER_HEADER__