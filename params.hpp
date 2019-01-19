#ifndef params_hpp
#define params_hpp

#include<ostream>
#include<list>

class params
{
public:
	params(const int argc, char* argv[]);

	auto begin() const {
		return capacities.begin();
	}

	auto end() const {
		return capacities.end();
	}

	int measure = 0; // 0 means find all possible measures
	unsigned max_steps = 100;
	bool help_required = false;

	struct help_type;
	static help_type help; // insert on ostream to for help

private:
	std::list<int> capacities;
};

std::ostream& operator<<(std::ostream&, const params::help_type&);

#endif
