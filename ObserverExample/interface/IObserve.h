#ifndef __IOBSERVABLE_H_JUN__
#define __IOBSERVABLE_H_JUN__

#include <memory>

class IUnsubscriber 
{
public:
	virtual ~IUnsubscriber() {}
	virtual void Unsubscribe() = 0;
};

template<typename Ty>
class IObservable;

template<typename Ty>
class IObserver 
{
public:
	virtual ~IObserver() {}
	virtual void Subscribe(IObservable<Ty> * const provider) = 0;
	virtual void Unsubscribe() = 0;
    virtual void OnCompleted() = 0;
    virtual void OnError(std::exception *error) = 0;
	virtual void OnNext(const Ty &value) = 0;
};

template<typename Ty>
class IObservable 
{
public:
	virtual ~IObservable() {}
	virtual std::shared_ptr<IUnsubscriber> Subscribe(IObserver<Ty> * const observer) = 0;
};

#endif // !__IOBSERVABLE_H_JUN__
