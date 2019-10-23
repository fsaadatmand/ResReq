#include "ddiff.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <sstream>
#include <string>

int main(int argc, char **argv)
{
	using namespace boost::gregorian;

	std::string progName(*argv), fileName;
	auto inclusive = parseArguments(argc, argv, fileName);
	auto dates_input = loadFile(fileName);
	const auto period = setPeriod(from_string("2020/01/30"));
//	const auto period = setPeriod();
	date_duration total_absences;
	std::map<date_period, date_duration> absent_dates;
	for (const auto &line : dates_input) {
		try {
			std::stringstream sstream(line);
			std::string s1, s2;
			sstream >> s1 >> s2;
			auto absence = date_period(from_string(s1), from_string(s2));
			auto days_absent = calculateAbsence(period, absence, inclusive);
			if (days_absent.days())
				absent_dates[absence] = days_absent;
		} catch(std::exception &e) {
			std::cerr << "Error: bad date fromat entry:\n"
				<< "Detail: " << e.what() <<'\n';
			exit(EXIT_FAILURE);
		}
	}
	print_dates(std::cout, period, absent_dates);
	return 0;
}
