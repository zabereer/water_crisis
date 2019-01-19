#include"params.hpp"

#include<charconv>
#include<cstring>
#include<numeric>
#include<sstream>
#include<stdexcept>

#include<unistd.h>
#include<getopt.h>

namespace
{
	template <typename T>
	T parse(const char* s)
	{
		T i = 0;
		const auto res = std::from_chars(s, s + std::strlen(s), i);
		if (*res.ptr)
			throw std::invalid_argument{std::string{"Invalid argument: "} + s};
		return i;
	}
}

struct params::help_type
{
};

params::help_type params::help;

params::params(const int argc, char* argv[])
{
	bool measurement_specified = false;
	bool max_steps_specified = false;

	while (true) {
		static option long_options[] = {
			{"add-bucket", true, nullptr, 'a'},
			{"measure", true, nullptr, 'm'},
			{"max-steps", true, nullptr, 's'},
			{"help", false, nullptr, 'h'},
			{nullptr, false, nullptr, 0}
		};

		int c = getopt_long(argc, argv, "a:m:s:h", long_options, nullptr);
		if (c == -1)
			break;

		switch (c) {
		case 'a' : {
			int capacity = parse<int>(optarg);
			if (capacity < 1)
				throw std::invalid_argument{
					"Bucket capacity must be greater than 0"};
			capacities.emplace_back(capacity);
			break;
		}
		case 'm' : {
			if (measurement_specified)
				throw std::invalid_argument{
					"Measurement was already specified"};
			measurement_specified = true;
			measure = parse<int>(optarg);
			if (measure < 1)
				throw std::invalid_argument{
					"Invalid measurement, must be greater than 0"};
			break;
		}
		case 's' : {
			if (max_steps_specified)
				throw std::invalid_argument{
					"Max number of steps already specified"};
			max_steps_specified = true;
			max_steps = parse<unsigned>(optarg);
			if (max_steps < 1)
				throw std::invalid_argument{
					"Invalid maximum number of steps"};
			break;
		}
		case 'h' :
			help_required = true;
			break;
		case '?' :
			throw std::invalid_argument{"Invalid argument"};
			break;
		}
	}

	if (optind < argc) {
		std::ostringstream oss;
		oss << "invalid argument: ";
		for (; optind < argc; ++optind)
			oss << argv[optind] << ' ';
		throw std::invalid_argument{oss.str()};
	}

	if (!help_required) {
		try {
			if (capacities.empty())
				throw std::invalid_argument{"No buckets were added"};
			if (std::accumulate(capacities.begin(), capacities.end(), 0) < measure)
				throw std::invalid_argument{
					"Required measurement exceeds total bucket capacity"};
		}
		catch (const std::exception& e) {
			help_required = true;
			std::ostringstream oss;
			oss << e.what() << "\n\n" << help;
			throw std::invalid_argument{oss.str()};
		}
	}
}

std::ostream& operator<<(std::ostream& os, const params::help_type&)
{
	os << "Determine how to measure amount(s) of water using buckets of different capacities.\n"
		"Add buckets and specify amount to measure with arguments below.\n"
		"An absent measure would output ways to measure all amounts up to total capacity.\n"
		"Arguments are: -a | --add-bucket    add a bucket of specific capacity - repeat for each\n"
		"               -m | --measure       specify amount of water to measure\n"
		"               -s | --max-steps     specify maximum allowed steps, defaults to 100\n"
		"               -h | --help          output this message\n\n"
		"Buckets will be named from A(n), B(n), C(n) etc where n is the bucket's capacity\n"
		"User will be asked to extend number of steps if they are reached.\n"
		"Normally this will happen if no odd size bucket was specified.\n";
	return os;
}
