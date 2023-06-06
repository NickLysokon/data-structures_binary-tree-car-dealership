#ifndef _SMART_PTR_H
#define _SMART_PTR_H

template<class T>
class SmartPtr {
	T* data;
	int* owners_count;
public:

	explicit SmartPtr(T* addr = nullptr);
	~SmartPtr();
	SmartPtr(const SmartPtr& ptr);
	SmartPtr& operator=(const SmartPtr& ptr);
	T& operator*();
	const T& operator*() const;
	T* operator->();

};


template<class T>
SmartPtr<T>::SmartPtr(T* addr) : data(addr), owners_count(new int)
{
	*owners_count = 1;
}


template<class T>
SmartPtr<T>::~SmartPtr()
{
	if (*owners_count == 1) {
		delete owners_count;
		delete data;
	}
	else {
		*owners_count -= 1;
	}
}


template<class T>
SmartPtr<T>::SmartPtr(const SmartPtr& ptr)
{
	data = ptr.data;
	owners_count = ptr.owners_count;
	*owners_count += 1;
}


template<class T>
SmartPtr<T>& SmartPtr<T>::operator=(const SmartPtr& ptr)
{
	if (this == &ptr) return *this;

	*owners_count -= 1;
	if (data == nullptr || *owners_count == 0) {
		delete data;
		delete owners_count;
	}

	data = ptr.data;
	owners_count = ptr.owners_count;
	*owners_count += 1;

	return *this;
}


template<class T>
T& SmartPtr<T>::operator*()
{
	//should check for data == null and throw an exception
	return *data;
}


template<class T>
const T& SmartPtr<T>::operator*() const
{
	return *data;
}


template<class T>
T* SmartPtr<T>::operator->()
{
	return data;
}



#endif /* SMART_PTR_H */