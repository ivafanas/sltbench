#pragma once

#include <string>


namespace sltbench {

// TODO: use std::optional when it became widespread
template<typename T>
class scoped_optional
{
private:
	class storage
	{
	public:
		storage() {}
		~storage() {}

	public:
		T* addr() { return reinterpret_cast<T*>(&(storage_.data[0])); }

	private:
		union storage_as_union
		{
			char data[sizeof(T)];
			T obj;

			storage_as_union() {};
			~storage_as_union() {};
		} storage_;
	};

public:
	scoped_optional()
	{}

	scoped_optional(const scoped_optional&) = delete;
	scoped_optional(scoped_optional&&) = delete;
	scoped_optional& operator = (const scoped_optional&) = delete;
	scoped_optional& operator = (scoped_optional&&) = delete;

	~scoped_optional()
	{
		reset();
	}

	bool is_initialized() const
	{
		return is_initialized_;
	}

	void reset()
	{
		if (is_initialized_)
		{
			is_initialized_ = false;
			addr()->T::~T();
		}
	}

	template<class... Args>
	void emplace(Args&&... args)
	{
		reset();

		new (addr()) T(std::forward<Args>(args)...);
		is_initialized_ = true;
	}

	const T& get() const
	{
		return *addr();
	}

	T& get()
	{
		return *addr();
	}

private:
	T* addr() const
	{
		return storage_.addr();
	}

private:
	mutable storage storage_;
	bool is_initialized_ = false;
};

} // namespace sltbench
