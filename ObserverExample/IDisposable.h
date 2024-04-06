#ifndef __INTERFACE_DISPOSE_HEADER__
#define __INTERFACE_DISPOSE_HEADER__


class IDisposable
{
protected:
	IDisposable() {}

public:
	virtual ~IDisposable() {}

	virtual void Dispose() = 0;
};


#endif // !__INTERFACE_DISPOSE_HEADER__