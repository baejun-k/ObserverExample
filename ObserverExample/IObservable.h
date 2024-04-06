#ifndef __INTERFACE_OBSERABLE_HEADER__
#define __INTERFACE_OBSERABLE_HEADER__


#include <functional>


template<typename Ty> class IObserver;


template<typename DataType, typename ObserverType = IObserver<DataType>*>
class IObservable
{
protected:
	IObservable() {}

public:
	virtual ~IObservable() {}

	virtual std::function<void()> Subscribe(ObserverType observer) = 0;
};


#endif // !__INTERFACE_OBSERABLE_HEADER__