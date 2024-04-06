#ifndef __THREAD_SAFE_OBSERABLE_BASE_HEADER__
#define __THREAD_SAFE_OBSERABLE_BASE_HEADER__


#include "IObservable.h"
#include "IObserver.h"
#include <list>
#include <memory>
#include <mutex>


template<typename Ty>
class ThreadSafeObservableBase : public IObservable<Ty, std::shared_ptr<IObserver<Ty>>>
{
private:
	using ObserverPtrList = std::list<std::weak_ptr<IObserver<Ty>>>;

private:
	class UnSubscribe
	{
	private:
		std::mutex& m_mutex;
		ObserverPtrList& m_observers;
		std::shared_ptr<IObserver<Ty>> m_observer;

	public:
		UnSubscribe(std::mutex& mutex, ObserverPtrList& observers, std::shared_ptr<IObserver<Ty>>& observer)
			: m_mutex(mutex)
			, m_observers(observers)
			, m_observer(observer)
		{}
		~UnSubscribe()
		{}

		void operator()()
		{
			std::lock_guard<std::mutex> _lock(m_mutex);
			m_observers.remove_if([&](std::weak_ptr<IObserver<Ty>> _ptr) {
				return (m_observer == _ptr.lock());
			});
		}
	};

protected:
	ThreadSafeObservableBase()
		: IObservable<Ty, std::shared_ptr<IObserver<Ty>>>()
		, m_observers()
	{}

public:
	virtual ~ThreadSafeObservableBase() {}

	virtual std::function<void()> Subscribe(std::shared_ptr<IObserver<Ty>> observer) override
	{
		using ObserverPtrListIter = typename ObserverPtrList::iterator;

		std::lock_guard<std::mutex> _lock(m_mutex);

		ObserverPtrListIter iter = std::find_if(m_observers.begin(), m_observers.end(), [&observer](std::weak_ptr<IObserver<Ty>> _ptr) {
			return (observer == _ptr.lock());
		});
		if (m_observers.end() == iter)
		{
			std::weak_ptr<IObserver<Ty>> weakObserver = observer;
			m_observers.push_back(weakObserver);
		}

		return UnSubscribe(m_mutex, m_observers, observer);
	}

	virtual void Notify(const Ty& val) const
	{
		ObserverPtrList copyObservers;
		{
			std::lock_guard<std::mutex> _lock(m_mutex);
			copyObservers = m_observers;
		}

		for (std::weak_ptr<IObserver<Ty>> pWeakObserver : copyObservers)
		{
			if (std::shared_ptr<IObserver<Ty>> pObserver = pWeakObserver.lock())
			{
				pObserver->OnNext(val);
			}
		}
	}

	virtual void Error(std::exception const* pError) const
	{
		ObserverPtrList copyObservers;
		{
			std::lock_guard<std::mutex> _lock(m_mutex);
			copyObservers = m_observers;
		}

		for (std::weak_ptr<IObserver<Ty>> pWeakObserver : copyObservers)
		{
			if (std::shared_ptr<IObserver<Ty>> pObserver = pWeakObserver.lock())
			{
				pObserver->OnError(pError);
			}
		}
	}

	virtual void Completed() const
	{
		ObserverPtrList copyObservers;
		{
			std::lock_guard<std::mutex> _lock(m_mutex);
			copyObservers = m_observers;
		}

		for (std::weak_ptr<IObserver<Ty>> pWeakObserver : copyObservers)
		{
			if (std::shared_ptr<IObserver<Ty>> pObserver = pWeakObserver.lock())
			{
				pObserver->OnCompleted();
			}
		}
	}

private:
	mutable std::mutex m_mutex;
	ObserverPtrList m_observers;

};



#endif // !__THREAD_SAFE_OBSERABLE_BASE_HEADER__