#ifndef __OBSERABLE_BASE_HEADER__
#define __OBSERABLE_BASE_HEADER__


#include "IObservable.h"
#include "IObserver.h"
#include <list>


template<typename Ty>
class ObservableBase : public IObservable<Ty>
{
private:
	using ObserverPtr = IObserver<Ty>*;
	using ObserverPtrList = std::list<ObserverPtr>;
	using ObserverPtrListIter = typename ObserverPtrList::iterator;

private:
	class UnSubscribe
	{
	private:
		ObserverPtrList& m_observers;
		ObserverPtrListIter m_observer;

	public:
		UnSubscribe(ObserverPtrList& observers, ObserverPtrListIter& observer)
			: m_observers(observers)
			, m_observer(observer)
		{}
		~UnSubscribe()
		{}

		void operator()()
		{
			m_observers.remove(*m_observer);
		}
	};

protected:
	ObservableBase() 
		: IObservable<Ty>() 
		, m_observers()
	{}

public:
	virtual ~ObservableBase() {}

	virtual std::function<void()> Subscribe(IObserver<Ty>* observer) override
	{
		ObserverPtrListIter iter = std::find(m_observers.begin(), m_observers.end(), observer);
		if (m_observers.end() == iter)
		{
			iter = m_observers.insert(m_observers.end(), observer);
		}

		return UnSubscribe(m_observers, iter);
	}

	virtual void Notify(const Ty& val) const
	{
		for (auto pObserver : m_observers)
		{
			pObserver->OnNext(val);
		}
	}

	virtual void Error(std::exception const* pError) const
	{
		for (auto pObserver : m_observers)
		{
			pObserver->OnError(pError);
		}
	}

	virtual void Completed() const
	{
		for (auto pObserver : m_observers)
		{
			pObserver->OnCompleted();
		}
	}

private:
	ObserverPtrList m_observers;

};



#endif // !__OBSERABLE_BASE_HEADER__