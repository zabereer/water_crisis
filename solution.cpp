#include"solution.hpp"

#include<algorithm>
#include<iostream>
#include<sstream>

namespace
{
	struct NoSolution
	{
		const char* reason = nullptr;
	};

	unsigned ask_user_for_more_max_steps()
	{
		unsigned increment = 0;
		std::cout << "*** Reached maximum number of steps, ";
		std::cout << "specify by how many to increment (0 to stop) ***" << std::endl;
		std::cin >> increment;
		if (!increment)
			throw NoSolution{"Maximum number of steps reached"};
		return increment;
	}
}

void solution::find_solution(const int m, unsigned max_steps)
{
	clear();
	measure = m;
	try {
		int current_measure = 0;
		unsigned step_count = 1;
		while (current_measure != measure) {
			while (step_count > current_steps.size())
				current_measure += add_step();
			if (current_measure == measure)
				break;

			current_measure += current_steps.back().undo();

			std::size_t ix;
			do {
				ix = next_possible_step_ix(steps_ixes.back() + 1);
				if (ix == possible_steps.size()) {
					current_steps.pop_back();
					steps_ixes.pop_back();
					if (!current_steps.empty())
						current_measure += current_steps.back().undo();
				}
			} while (ix == possible_steps.size() && !current_steps.empty());

			if (current_steps.empty()) {
				++step_count;
				if (step_count > max_steps) {
					max_steps += ask_user_for_more_max_steps();
				}
			}
			else {
				current_steps.back() = possible_steps[ix];
				steps_ixes.back() = ix;
				current_measure += current_steps.back().apply();
			}
		}
	}
	catch (const NoSolution& e) {
		not_found_reason = e.reason;
	}
}

std::string solution::bucket_name_for_index(unsigned ix, const int capacity)
{
	std::ostringstream oss;
	static const unsigned range = 'Z' - 'A' + 1; // hopefully not EBCDIC!
	do {
		const unsigned mod = ix % range;
		oss << static_cast<char>('A' + mod);
		ix /= range;
	} while (ix);
	std::string name = oss.str();
	std::reverse(name.begin(), name.end());
	oss = std::ostringstream{};
	oss << name << '(' << capacity << ')';
	return oss.str();
}

void solution::setup_possible_steps()
{
	for (bucket& b : buckets)
		possible_steps.emplace_back(make_fill_step(wastage, &b));

	for (bucket& b1 : buckets)
		for (bucket& b2 : buckets)
			if (&b1 != &b2)
				possible_steps.emplace_back(
					make_transfer_step(wastage, &b1, &b2));

	for (bucket& b : buckets)
		possible_steps.emplace_back(make_empty_step(wastage, &b));
}

int solution::add_step()
{
	std::size_t ix = next_possible_step_ix(0);
	if (ix == possible_steps.size())
		throw NoSolution{"No next step applicable to extend search for solution"};
	steps_ixes.emplace_back(ix);
	current_steps.emplace_back(possible_steps[ix]);
	return current_steps.back().apply();
}

std::size_t solution::next_possible_step_ix(std::size_t ix)
{
	for (; ix < possible_steps.size(); ++ix) {
		if (possible_steps[ix].can_apply())
			break;
	}
	return ix;
}

void solution::clear()
{
	for (bucket& b : buckets)
		b.used = 0;
	current_steps.clear();
	steps_ixes.clear();
	not_found_reason = nullptr;
	measure = 0;
	wastage = 0;
}

std::ostream& operator<<(std::ostream& os, const solution& sol)
{
	if (sol.not_found_reason) {
		os << "No solution found for measurement " << sol.measure;
		os << " : " << sol.not_found_reason;
	}
	else {
		os << "For measuring " << sol.measure << " :\n";
		for (auto&& s : sol.current_steps) {
			os << "  " << s << '\n';
		}
		os << "  Wasted water: " << sol.wastage << '\n';
	}
	return os;
}
