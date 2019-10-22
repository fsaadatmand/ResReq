#include <boost/date_time/gregorian/gregorian.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Duration {
	const boost::gregorian::date start;
	const boost::gregorian::date end;
	const boost::gregorian::date_duration period;
	boost::gregorian::date_duration absences;
};

std::vector<std::string> loadFile(const std::string &filename) {
	std::ifstream inFile(filename);
	if (!inFile) {
		std::cerr << "cannot open file " << filename << '\n';
		exit(EXIT_FAILURE);
	}
	std::vector<std::string> data;
	std::string line;
	while (std::getline(inFile, line))
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

Duration calculate_duration(const int &years)
{
	const auto end =
		boost::gregorian::date(boost::gregorian::day_clock::local_day()); 
	const auto start = end - boost::gregorian::years(years);
	const auto period = end - start;
	return {start, end, period, boost::gregorian::date_duration()};
}

std::ostream& printHeader(std::ostream &os, const Duration &duration)
{
	os << "\nPeriod from "
		      << duration.start << " to "
			  << duration.end << "\n";
	os << "----------------------------------------------\n" 
	   << " Departure\tReturn\t\tAbsences\n"
	   << " (dd/mm/yyyy)\t(dd/mm/yyyy)\t(days)\n"
	   << "----------------------------------------------\n";
	return os;
}

std::ostream& printFooter(std::ostream &os, const Duration &duration, const int &required)
{
	os << "\t\tTotal\t\t" << duration.absences << '\n';
	os << "\nRequired presence in Canada " << required << " days\n";
	auto presence = duration.period - duration.absences;
	os << "Total presence in Canada " << presence << " days\n";
	if (presence.days() < required)
		std::cout << "You do not meet the residency requirement\n";
	else
		std::cout << "You meet the residency requiremnt\n";
	return os;
}

int main(int argc, char **argv)
{
	std::string progName(*argv), fileName;
	auto inclusive = parseArguments(argc, argv, fileName);
	auto data = loadFile(fileName);
	auto duration = calculate_duration(5);
	printHeader(std::cout, duration);
	for (const auto &line : data) {
		if (line[0] == '#') // ignore comments
			continue;
		std::stringstream sstream(line);
		std::string s1, s2;
		sstream >> s1 >> s2; 
		auto d1(boost::gregorian::from_string(s1));
		auto d2(boost::gregorian::from_string(s2));
		boost::gregorian::date_duration absence;
		if (d1 >= duration.start && d2 <= duration.end) {
			absence = d2 - d1;
			if (inclusive)
				absence += boost::gregorian::date_duration(1);
			std::cout << s1 << '\t' << s2 << '\t' << absence << '\n';
			duration.absences += absence;
		}
	}
	constexpr int required_presence = 730;
	printFooter(std::cout, duration, required_presence);
	return 0;
}
