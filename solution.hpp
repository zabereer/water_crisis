#ifndef solution_hpp
#define solution_hpp

#include"bucket.hpp"
#include"step.hpp"

#include<cstddef>
#include<deque>
#include<iterator>
#include<ostream>
#include<string>
#include<vector>

class solution;
std::ostream& operator<<(std::ostream&, const solution&);

class solution
{
public:
	template <typename I>
	solution(I begin, I end);
	void find_solution(const int measure, unsigned max_steps);

private:
	std::string bucket_name_for_index(unsigned, const int);
	void setup_possible_steps();

	// apply step and return difference in measurement
	int add_step();

	// returns index into possible_steps or possible_steps.size() if none applicable
	std::size_t next_possible_step_ix(std::size_t start_ix);

	void clear();

	std::vector<bucket> buckets;
	std::deque<step> possible_steps;
	std::deque<step> current_steps;
	// index of step in possible_steps, for example steps_ixes[0] is index of step
	// in possible_steps that was copied into current_steps[0]
	std::deque<std::size_t> steps_ixes;
	const char* not_found_reason = nullptr;
	int measure = 0;
	unsigned wastage = 0;

	friend std::ostream& operator<<(std::ostream&, const solution&);
};

template <typename I>
solution::solution(I begin, I end)
{
	buckets.reserve(std::distance(begin, end));
	for (unsigned ix = 0; begin != end; ++begin, ++ix)
		buckets.emplace_back(bucket{bucket_name_for_index(ix, *begin), *begin});
	setup_possible_steps();
}

#endif
