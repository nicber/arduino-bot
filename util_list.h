#pragma once

namespace util
{
template <typename T>
class list
{
	struct nodo
	{
		nodo* sig_ = nullptr;
		T data_;
		
		template <typename... Args>
		nodo(Args&&... args)
		: data_(static_cast<Args&&>(args)...)
		{}
	};
	nodo* com_ = nullptr;
	nodo* fin_ = nullptr;
	
public:
	class const_iterador
	{
	protected:
		nodo* obj_;
		
		const_iterador(nodo* obj)
		: obj_(obj)
		{}
		
		friend class list<T>;
	public:
		const_iterador operator++()
		{
			return const_iterador(obj_->sig_);
		}
		
		operator bool() const
		{
			return obj_;
		}
		
		const T& operator*() const
		{
			return obj_->data;
		}
		
		const T& operator->() const
		{
			return obj_->data;
		}
	};
	
	class iterador : public const_iterador
	{
		iterador(nodo* obj)
		: const_iterador(obj)
		{}
		
		friend class list<T>;
	public:
		iterador operator++()
		{
			return iterador(const_iterador::obj_->sig_);
		}
		
		T& operator*() const
		{
			return const_iterador::obj_->data_;
		}
		
		T& operator->() const
		{
			return const_iterador::obj_->data_;
		}
	};
	
	template <typename... Args>
	void emplace_back(Args&&... args)
	{
		nodo* next = new nodo(static_cast<Args&&>(args)...);
		fin_->sig_ = next;
		fin_ = next;
	}
	
	void remover(const const_iterador& it)
	{
		nodo* anterior_it{com_};
		while (anterior_it->sig_ != it.obj_)
		{
			// Recorre toda la lista y segfaultea si no esta en la lista.
			anterior_it = anterior_it->sig_;
		}
		
		anterior_it->sig_ = anterior_it->sig_->sig_;
		delete anterior_it;
	}
	
	iterador begin()
	{
		return iterador(com_);
	}
	
	const_iterador begin() const
	{
		return const_iterador(com_);
	}
	
	iterador end()
	{
		return iterador(fin_->sig_);
	}
	
	const_iterador end() const
	{
		return const_iterador(fin_->sig);
	}
	
	~list()
	{
		for(; com_ != nullptr;)
		{
			com_ = com_->sig_;
			delete com_;
		}
	}
};
}