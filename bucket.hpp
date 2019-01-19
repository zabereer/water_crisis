#ifndef bucket_hpp
#define bucket_hpp

#include<string>

struct bucket
{
	std::string name;
	int capacity;
	int used = 0;

	bool full() const
	{
		return capacity == used;
	}

	bool empty() const
	{
		return !used;
	}
};

#endif
