#pragma once

#include <cassert>


template <typename  T>
class Iterator
{
private:
	T &container;
	typename T::iterator current;

public:
	Iterator(T &_container) :
			container(_container),
			current(container.begin())
	{};

	~Iterator()
	{};

	void rest()
	{
		current = container.begin();
	};

	void next()
	{
		++current;
	};

	bool isEnd()
	{
		return current == container.end();
	};

	typename T::value_type &operator* ()
	{
		return *current;
	};

};
