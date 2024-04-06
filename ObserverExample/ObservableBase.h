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

protected:
	ObservableBase() 
		: IObservable<Ty>() 
		, m_observers()
	{}

public:
	virtual ~ObservableBase() {}

	/// <summary>
	/// observer 객체를 observable에 구독
	/// 구독 해제하는 함수를 반환
	/// </summary>
	/// <param name="observer">observable을 구독할 객체</param>
	/// <returns>구독 해제하는 함수</returns>
	virtual std::function<void()> Subscribe(IObserver<Ty>* observer) override
	{
		ObserverPtrListIter iter = std::find(m_observers.begin(), m_observers.end(), observer);
		if (m_observers.end() == iter)
		{
			m_observers.push_back(observer);
		}

		return std::bind(&ObservableBase::UnSubscribe, this, observer);
	}

	/// <summary>
	/// 값을 observer에 전달
	/// </summary>
	/// <param name="val">전달 할 값</param>
	virtual void Notify(const Ty& val) const
	{
		for (auto pObserver : m_observers)
		{
			pObserver->OnNext(val);
		}
	}

	/// <summary>
	/// error가 발생했음을 oserver에 전달
	/// </summary>
	/// <param name="pError">에러 값</param>
	virtual void Error(std::exception const* pError) const
	{
		for (auto pObserver : m_observers)
		{
			pObserver->OnError(pError);
		}
	}

	/// <summary>
	/// overvable이 완료되었음을 알림
	/// </summary>
	virtual void Completed() const
	{
		for (auto pObserver : m_observers)
		{
			pObserver->OnCompleted();
		}
	}

	/// <summary>
	/// observer가 구독 해제하는 함수
	/// </summary>
	/// <param name="observer">구독 중인 observer</param>
	void UnSubscribe(ObserverPtr observer)
	{
		m_observers.remove(observer);
	}

private:
	ObserverPtrList m_observers;

};



#endif // !__OBSERABLE_BASE_HEADER__