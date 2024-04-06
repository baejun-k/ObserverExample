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

protected:
	ThreadSafeObservableBase()
		: IObservable<Ty, std::shared_ptr<IObserver<Ty>>>()
		, m_observers()
	{}

public:
	virtual ~ThreadSafeObservableBase() {}

	/// <summary>
	/// observer 객체를 observable에 구독
	/// 구독 해제하는 함수를 반환
	/// </summary>
	/// <param name="observer">observable을 구독할 객체</param>
	/// <returns>구독 해제하는 함수</returns>
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

		return std::bind(&ThreadSafeObservableBase::UnSubscribe, this, observer);
	}

	/// <summary>
	/// 값을 observer에 전달
	/// </summary>
	/// <param name="val">전달 할 값</param>
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

	/// <summary>
	/// error가 발생했음을 oserver에 전달
	/// </summary>
	/// <param name="pError">에러 값</param>
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

	/// <summary>
	/// overvable이 완료되었음을 알림
	/// </summary>
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

	/// <summary>
	/// observer가 구독 해제하는 함수
	/// </summary>
	/// <param name="observer">구독 중인 observer</param>
	void UnSubscribe(std::shared_ptr<IObserver<Ty>>& observer)
	{
		std::lock_guard<std::mutex> _lock(m_mutex);
		m_observers.remove_if([&](std::weak_ptr<IObserver<Ty>> _ptr) {
			return (observer == _ptr.lock());
		});
	}

private:
	mutable std::mutex m_mutex;
	ObserverPtrList m_observers;

};



#endif // !__THREAD_SAFE_OBSERABLE_BASE_HEADER__