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
	/// observer ��ü�� observable�� ����
	/// ���� �����ϴ� �Լ��� ��ȯ
	/// </summary>
	/// <param name="observer">observable�� ������ ��ü</param>
	/// <returns>���� �����ϴ� �Լ�</returns>
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
	/// ���� observer�� ����
	/// </summary>
	/// <param name="val">���� �� ��</param>
	virtual void Notify(const Ty& val) const
	{
		for (auto pObserver : m_observers)
		{
			pObserver->OnNext(val);
		}
	}

	/// <summary>
	/// error�� �߻������� oserver�� ����
	/// </summary>
	/// <param name="pError">���� ��</param>
	virtual void Error(std::exception const* pError) const
	{
		for (auto pObserver : m_observers)
		{
			pObserver->OnError(pError);
		}
	}

	/// <summary>
	/// overvable�� �Ϸ�Ǿ����� �˸�
	/// </summary>
	virtual void Completed() const
	{
		for (auto pObserver : m_observers)
		{
			pObserver->OnCompleted();
		}
	}

	/// <summary>
	/// observer�� ���� �����ϴ� �Լ�
	/// </summary>
	/// <param name="observer">���� ���� observer</param>
	void UnSubscribe(ObserverPtr observer)
	{
		m_observers.remove(observer);
	}

private:
	ObserverPtrList m_observers;

};



#endif // !__OBSERABLE_BASE_HEADER__