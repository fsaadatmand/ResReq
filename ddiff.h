#ifndef D_DIFF_H
#define D_DIFF_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <fstream>
#include <map>
#include <ostream>
#include <string>
#include <vector>

std::vector<std::string> loadFile(const std::string &);
bool parseArguments(int, char **, std::string &);
boost::gregorian::date_period setPeriod(const boost::gregorian::date &end =
		boost::gregorian::day_clock::local_day(), const int &prev_years = 5);
boost::gregorian::date_duration
calculateAbsence(const boost::gregorian::date_period &,
		const boost::gregorian::date_period &, const bool &inclusive);
std::ostream& printHeader(std::ostream &, const boost::gregorian::date_period &);
std::ostream& printFooter(std::ostream &, const boost::gregorian::date_period &, 
		const boost::gregorian::date_duration &, const int &required_presence = 730);
std::ostream& print_dates(std::ostream &, const boost::gregorian::date_period &,
		const std::map<boost::gregorian::date_period,
		boost::gregorian::date_duration> &);
#endif

