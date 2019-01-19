#include"params.hpp"
#include"solution.hpp"

#include<cstdlib>
#include<exception>
#include<iostream>
#include<numeric>

namespace
{
	template <typename I>
	void do_solution(I begin, I end,
			 const int measure, const unsigned max_steps)
	{
		solution sol{begin, end};
		sol.find_solution(measure, max_steps);
		std::cout << sol << std::endl;
	}

	template <typename I>
	void do_all_solutions(I begin, I end,
			      const unsigned max_steps)
	{
		const int max = std::accumulate(begin, end, 0);
		solution sol{begin, end};
		for (int measure = 1; measure <= max; ++measure) {
			sol.find_solution(measure, max_steps);
			std::cout << sol << std::endl;
		}
	}

	void find_solutions(const params& arguments)
	{
		if (arguments.measure) {
			do_solution(arguments.begin(), arguments.end(),
				    arguments.measure, arguments.max_steps);
		}
		else {
			do_all_solutions(arguments.begin(),arguments.end(),
					 arguments.max_steps);
		}
	}
}

int main(int argc, char* argv[])
{
	int rc = EXIT_SUCCESS;

	try {
		const params arguments{argc, argv};
		if (arguments.help_required)
			std::cout << params::help << std::endl;
		else
			find_solutions(arguments);
	}
	catch (const std::exception& e) {
		std::cerr << "\nError: " << e.what() << std::endl;
		rc = EXIT_FAILURE;
	}

	return rc;
}
