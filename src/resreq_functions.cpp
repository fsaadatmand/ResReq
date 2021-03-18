#include "resreq.h"
#include <algorithm>
#include <boost/concept_check.hpp>
#include <boost/date_time/gregorian/greg_duration.hpp>
#include <boost/date_time/gregorian/greg_duration_types.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <initializer_list>
#include <map>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

using boost::gregorian::date;
using boost::gregorian::days;
using boost::gregorian::date_period;

std::string
lstrip(const std::string &str)
{
	auto pos = std::find_if_not(str.cbegin(), str.cend(),
			[](const auto &ch) { return std::isspace(ch); });
	if (pos == str.cend())
		return str;
	return std::string(pos, str.cend());
}

unsigned long
to_int(const char *str)
{
	char *endptr;
	errno = 0; 
	auto intValue = strtoul(str, &endptr, 10);
	if (errno == ERANGE || *endptr != '\0' || str == endptr) {
		throw std::out_of_range("stroul: out of range");
	}
	return intValue;
}

std::vector<std::string>
loadFile(const std::string &filename)
{
	std::ifstream inFile(filename);
	if (!inFile)
		throw std::runtime_error("cannot open file");
	std::vector<std::string> data;
	std::string line;
	while (std::getline(inFile, line)) {
		// strip leading whitespaces
		line = lstrip(line);
		// ignore empty and commented lines
		if (!line.empty() && line.front() != '#')
			// add try-catch here instead of in main
			data.push_back(line);
	}
	return data;
}

void
die(std::initializer_list<std::string> il)
{
	for (const auto &str : il)
		std::cerr << str << ' ';
	std::cerr << std::endl;
	std::exit(EXIT_FAILURE);
}

// TODO: use one form of error handling.
std::tuple<const std::string, date, unsigned long, bool>
parseArguments(int count, char **list)
{
	using boost::gregorian::day_clock;
	using boost::gregorian::from_string;

	std::string progname(*list);
	auto end_date = day_clock::local_day();
	unsigned period_length = 5;
	bool inclusive = false;
	if (count < 2)
		die({ "Usage:", progname, "[option] <filename>" });
	while (--count > 0)
		if (**++list == '-') { 
			if (!std::strcmp(++*list, "i"))
				inclusive = true;
			else if (!std::strcmp(*list, "d")) {
				try {
					end_date = from_string(*++list);
					--count;
				} catch(std::exception &e) {
					die({ progname + ":", "invalid date format",
							*list, "(yyyy/mm/dd)" });
				}
			} else if (!std::strcmp(*list, "p")) {
				// need to exclude negative period lengths 
				if (!(period_length = to_int(*++list)))
					die({ progname + ":", "invalid period length:", *list });
				--count;
			} else
				die({ progname + ":", "Invalid option" });
		}
	std::string filename = *list;
	return std::make_tuple(filename, end_date, period_length, inclusive);
}

date_period
setPeriod(const date &end, const int &prev_years)
{
	using boost::gregorian::years;

	const auto begin = end - years(prev_years);
	return {begin, end};
}

days
dates_difference(const date_period &period, date_period &absence,
		const bool &inclusive)
{
	auto days_absent = days();
	if (absence.intersects(period)) {
		absence = absence.intersection(period);
		// CIC does not include beginning date
		days_absent = absence.length() - days(1);
		if (inclusive)
			days_absent += days(2);
	}
	return days_absent;
}

std::ostream&
printHeader(std::ostream &os, const date_period &period)
{
	os << "\n Period from "
		      << period.begin() << " to "
			  << period.end() << "\n";
	os << " ----------------------------------------------\n" 
	   << "  Departure\tReturn\t\tAbsence (days)\n"
	   << "----------------------------------------------\n";
	return os;
}

std::ostream&
printFooter(std::ostream &os, const date_period &period,
		const days &absences, const int &required_presence)
{
	long unsigned days_in_a_year = 365; 
	os << "\t\tTotal\t\t" << absences << '\n';
	os << "\n Required presence in Canada " << required_presence << " days "
		<< "in the past " << period.length() / days_in_a_year << " years\n";
	auto presence = period.length() - absences;
	os << " Total physical presence in Canada " << presence << " days\n";
	if (presence.days() < required_presence)
		std::cout << " You DO NOT meet the residency requirement\n";
	else
		std::cout << " You meet the residency requirement\n";
	return os;
}

std::ostream&
print_dates(std::ostream &os, const date_period &period,
		const std::map<date_period, days> &dates)
{
	printHeader(std::cout, period);
	auto total_absences = days();
	for (const auto &[date, duration] : dates) { // using C++17 structured binding
		std::cout << ' ' << date.begin() << '\t' << date.end()
	  			  << '\t' << duration << '\n';
		total_absences += duration;
	}
	printFooter(std::cout, period, total_absences);
	return os;
}
