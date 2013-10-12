#pragma once
#include <iostream>

namespace util
{
/**
 * Clase similar a std::unique_ptr.
 */
template <typename T>
class unique_ptr
{
	T* ptr_ = nullptr;
	template <typename U>
	friend void swap(unique_ptr<U>&, unique_ptr<U>&);
public:
	unique_ptr()
	{}
	unique_ptr(T* ptr)
	: ptr_(ptr)
	{}
	
	~unique_ptr()
	{
		delete ptr_;
	}
	
	unique_ptr(unique_ptr<T>& u_ptr) = delete;
	
	unique_ptr(unique_ptr<T>&& u_ptr)
	: unique_ptr()
	{
		swap(*this, u_ptr);
	}
	
	unique_ptr<T>& operator=(unique_ptr<T> u_ptr)
	{
		swap(*this, u_ptr);
	}
	
	T& operator*() const
	{
		return *ptr_;
	}
	
	T& operator->() const
	{
		return *ptr_;
	}
};

/**
 * Funcion para swap.
 */
template <typename T>
void swap(unique_ptr<T>& lhs, unique_ptr<T>& rhs)
{
	std::swap(lhs.ptr_, rhs.ptr_);
}

/**
 * Auxiliar como std::make_shared.
 */
template <typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args)
{
	return unique_ptr<T>(new T(static_cast<Args&&>(args)...));
}
}