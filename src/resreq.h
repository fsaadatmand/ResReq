#ifndef RES_REQ_H
#define RES_REQ_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <fstream>
#include <map>
#include <ostream>
#include <string>
#include <vector>

std::string lstrip(const std::string &);
unsigned long to_int(const char *str);
std::vector<std::string> loadFile(const std::string &);
void die(std::initializer_list<std::string>);
std::tuple<const std::string, boost::gregorian::date, unsigned long, bool>
parseArguments(int, char **);
boost::gregorian::date_period
setPeriod(const boost::gregorian::date &end =
              boost::gregorian::day_clock::local_day(),
          const int &prev_years = 5);
boost::gregorian::date_duration
dates_difference(const boost::gregorian::date_period &,
                 boost::gregorian::date_period &, const bool &inclusive);
std::ostream &printHeader(std::ostream &,
                          const boost::gregorian::date_period &);
std::ostream &printFooter(std::ostream &, const boost::gregorian::date_period &,
                          const boost::gregorian::date_duration &,
                          const int &required_presence = 730);
std::ostream &print_dates(std::ostream &, const boost::gregorian::date_period &,
                          const std::map<boost::gregorian::date_period,
                                         boost::gregorian::date_duration> &);
#endif /* RES_REQ_H */
