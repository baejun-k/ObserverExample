#pragma warning(disable : 4996)
#include "interface/IObserve.h"

#include <chrono>
#include <vector>
#include <stdlib.h>

namespace chr = std::chrono;

class Clock : public IObservable<chr::duration<double>>
{
public:
	struct Observer 	
	{
		IObserver<chr::duration<double>> *pObserver;
		std::shared_ptr<IUnsubscriber>    pUnsubscribeHandle;

		Observer(IObserver<chr::duration<double>> * const observer, 
				 std::shared_ptr<IUnsubscriber> unsubscribeHlr) :
			pObserver(observer), pUnsubscribeHandle(unsubscribeHlr)
		{}
	};

	class Unsubscriber : public IUnsubscriber 	{
	private:
		std::vector<Observer> &_observers;
		IObserver<chr::duration<double>> *_pObserver;

	public:
		Unsubscriber(std::vector<Observer> &observers,
					 IObserver<chr::duration<double>> *const observer) :
			_observers(observers), _pObserver(observer)
		{}

		// IUnsubscriber을(를) 통해 상속됨
		virtual void Unsubscribe() override
		{
			using itorator = std::vector<Observer>::const_iterator;
			itorator beg = _observers.begin();

			size_t memId = (size_t)_pObserver;
			for ( int i = _observers.size() - 1; i >= 0; --i )
			{
				itorator it = ( beg + i );
				if ( memId == (size_t)it->pObserver )
				{
					_observers.erase(it);
					return;
				}
			}
		}
	};

private:
	chr::system_clock::time_point _startTime;
	chr::duration<double> _sec;

	std::vector<Observer> _observers;

public:
	Clock()
	{
		_startTime = chr::system_clock::now();
		_sec = chr::duration<double>(0);
	}
	~Clock()
	{
		Completed();
	}

	// IObservable을(를) 통해 상속됨
	virtual std::shared_ptr<IUnsubscriber> Subscribe(IObserver<chr::duration<double>> * const observer) override
	{
		using itorator = std::vector<Observer>::const_iterator;
		itorator beg = _observers.begin();

		size_t memId = (size_t)observer;
		for ( int i = _observers.size() - 1; i >= 0; --i )
		{
			itorator it = ( beg + i );
			if ( memId == (size_t)it->pObserver )
			{
				return it->pUnsubscribeHandle;
			}
		}

		std::shared_ptr<IUnsubscriber> unsubscribeHandle(new Unsubscriber(_observers, observer));
		_observers.push_back(Observer(observer, unsubscribeHandle));

		return unsubscribeHandle;
	}

	void Updated()
	{
		printf("Updated Start======================\n");
		_sec = chr::system_clock::now() - _startTime;

		using itorator = std::vector<Observer>::const_iterator;
		itorator beg = _observers.begin();

		for ( int i = _observers.size() - 1; i >= 0; --i )
		{
			itorator it = ( beg + i );
			it->pObserver->OnNext(_sec);
		}

		printf("Updated End========================\n\n");
	}

	void Completed()
	{
		using itorator = std::vector<Observer>::const_iterator;
		itorator beg = _observers.begin();

		for ( int i = _observers.size() - 1; i >= 0; --i )
		{
			itorator it = ( beg + i );
			it->pObserver->OnCompleted();
		}
	}

	double GetDuration()
	{
		return _sec.count();
	}
};

class DurationChecker : public IObserver<chr::duration<double>>
{
private:
	std::shared_ptr<IUnsubscriber> _unsubscribeHandle;

public:
	DurationChecker() :
		_unsubscribeHandle(nullptr)
	{}


	// IObserver을(를) 통해 상속됨
	virtual void Subscribe(IObservable<chr::duration<double>> * const provider) override
	{
		_unsubscribeHandle = provider->Subscribe(this);
	}

	virtual void Unsubscribe() override
	{
		if ( _unsubscribeHandle )
		{
			_unsubscribeHandle->Unsubscribe();
			_unsubscribeHandle.reset();
		}
		_unsubscribeHandle = nullptr;
	}

	virtual void OnCompleted() override
	{
		Unsubscribe();
	}

	virtual void OnError(std::exception *error) override
	{
		fprintf(stderr, "%s\n", error->what());
	}

	virtual void OnNext(const chr::duration<double> &value) override
	{
		fprintf(stdout, "%lf (sec)\n", value.count());
	}
};

Clock g_clock;

int main()
{
	DurationChecker *pCheker = new DurationChecker();
	pCheker->Subscribe(&g_clock);

	while ( g_clock.GetDuration() < 2.0 )
	{
		g_clock.Updated();
		_sleep(400);
	}
	
	pCheker->Unsubscribe();

	while ( g_clock.GetDuration() < 4.0 )
	{
		g_clock.Updated();
		_sleep(400);
	}

	pCheker->Subscribe(&g_clock);

	while ( g_clock.GetDuration() < 6.0 )
	{
		g_clock.Updated();
		_sleep(400);
	}

	g_clock.Completed();

	g_clock.Updated();

	return 0;
}