#include "ddiff.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <fstream>
#include <map>
#include <string>
#include <vector>

std::vector<std::string> loadFile(const std::string &filename) {
	std::ifstream inFile(filename);
	if (!inFile) {
		std::cerr << "cannot open file " << filename << '\n';
		exit(EXIT_FAILURE);
	}
	std::vector<std::string> data;
	std::string line;
	while (std::getline(inFile, line))
		if (line[0] != '#')
			data.push_back(line);
	return data;
}

bool parseArguments(int count, char **list, std::string &filename)
{
	auto progName = *list;
	if (count < 2 || count > 3) {
		std::cerr << "Usage: " << *list << " [option] <filename> \n";
		exit(EXIT_FAILURE);
	}
	filename = *++list;
	bool option = false;
	if (count == 3) {
		if (**list == '-' && *++(*list) == 'i') {
			option = true;
			filename = *++list;
		} else {
			std::cerr << progName << ": Ivalid option " << *list << '\n';
			exit(EXIT_FAILURE);
		}
	}
	return option;
}

boost::gregorian::date_period
setPeriod(const boost::gregorian::date &end, const int &prev_years)
{
	const auto begin = end - boost::gregorian::years(prev_years);
	return {begin, end};
}

boost::gregorian::date_duration
calculateAbsence(const boost::gregorian::date_period &period,
		const boost::gregorian::date_period &absence,
		const bool &inclusive)
{
	boost::gregorian::date_duration days_absent;
	if (absence.intersects(period)) {
		days_absent = absence.intersection(period).length();
		if (inclusive)
			days_absent += boost::gregorian::date_duration(1);
	}
	return days_absent;
}

std::ostream&
printHeader(std::ostream &os, const boost::gregorian::date_period &period)
{
	os << "\n Period from "
		      << period.begin() << " to "
			  << period.end() << "\n";
	os << " ----------------------------------------------\n" 
	   << "  Departure\tReturn\t\tAbsences\n"
	   << "  (dd/mm/yyyy)\t(dd/mm/yyyy)\t(days)\n"
	   << "----------------------------------------------\n";
	return os;
}

std::ostream&
printFooter(std::ostream &os, const boost::gregorian::date_period &period,
		const boost::gregorian::date_duration &absences, const int &required_presence)
{
	os << "\t\tTotal\t\t" << absences << '\n';
	os << "\n Required presence in Canada " << required_presence << " days\n";
	auto presence = period.length() - absences;
	os << " Total presence in Canada " << presence << " days\n";
	if (presence.days() < required_presence)
		std::cout << " You do not meet the residency requirement\n";
	else
		std::cout << " You meet the residency requiremnt\n";
	return os;
}

std::ostream&
print_dates(std::ostream &os, const boost::gregorian::date_period &period,
		const std::map<boost::gregorian::date_period,
		boost::gregorian::date_duration> &dates)
{
	printHeader(std::cout, period);
	auto total_absences = boost::gregorian::date_duration();
	for (const auto date : dates) {
		std::cout << ' ' << date.first.begin() << '\t' << date.first.begin()
	  			  << '\t' << date.second << '\n';
		total_absences += date.second;
	}
	printFooter(std::cout, period, total_absences);
	return os;
}
