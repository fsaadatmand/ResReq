#include "resreq.h"
#include "Absence.h"
#include <boost/date_time/gregorian/greg_duration.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/gregorian/parsers.hpp>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

int main(int argc, char **argv)
{
	using boost::gregorian::date_period;
	using boost::gregorian::days;
	using boost::gregorian::from_string;
	using boost::gregorian::date;

	auto [filename, end_date, period_years, inclusive] = 
		parseArguments(argc, argv);

	std::vector<std::string> dates_input;
	try {
		dates_input = loadFile(filename);
	} catch (std::runtime_error &e) {
		std::cout << e.what() << '\n';
		return EXIT_FAILURE;
	}
	const auto main_period = setPeriod(end_date, period_years);
	auto absences = std::map<date_period, days>();
	for (const auto &line : dates_input) {
		try {
			std::stringstream sstream(line);
			std::string s1, s2;
			sstream >> s1 >> s2;
			auto dates = date_period(from_string(s1), from_string(s2));
			auto duration = dates_difference(main_period, dates, inclusive);
			if (duration.days() != 0) {
				absences[dates] = duration;
			}
		} catch (std::exception &e) {
			die({ "Error: bad date format entry:", "\nDetail:", e.what() });
		}
	}
	print_dates(std::cout, main_period, absences);
	std::exit(EXIT_SUCCESS);
}
