#include "Date.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

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

std::ostream& printHeader(std::ostream &os)
{
	os << "\n Departure\tReturn\t\tAbsences\n"
			  << " (dd/mm/yyyy)\t(dd/mm/yyyy)\t(days)\n"
			  << "----------------------------------------------\n";
	return os;
}

int main(int argc, char **argv)
{
	std::string progName(*argv), fileName;
	auto inclusive = parseArguments(argc, argv, fileName);
	auto data = loadFile(fileName);
	int sum = 0;
	printHeader(std::cout);
	for (const auto &line : data) {
		std::istringstream sstream(line);
		std::string startDate, endDate;
		while (sstream >> startDate >> endDate) {
			auto daysAbsent = std::abs(Date(endDate) - Date(startDate));
			if (inclusive)
				++daysAbsent;
			std::cout << ' ' << startDate << '\t' << endDate
				      << '\t' << daysAbsent << '\n';
			sum += daysAbsent;
		}
	}
	std::cout << "\n\t\t\tTOTAL\t" << sum << '\n';
	std::cout << "\nRequired to stay in Canada at least 730 days in the last 5 years\n";
	int total_presence = (Date("2019/10/01") - Date("2014/09/28")) - sum;
	constexpr int required_presence = 730;
	if (total_presence < required_presence) 
		std::cout << "You did not meet\n";
	else
		std::cout <<  " You met the reuirement\n";
	std::cout << " Days lived in Canada: " <<  total_presence << '\n';
	time_t tt;
	std::tm *ti;
	time(&tt);
	ti = localtime(&tt);
	std::cout << ti->tm_yday << '\n';
	std::cout << Date("2019/09/29").get_yearDay() << '\n';
	std::cout << Date("2019/09/29").get_weekDay() << '\n';
	return EXIT_SUCCESS;
}
