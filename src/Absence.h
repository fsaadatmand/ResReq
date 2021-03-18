#ifndef ABSENCE_H
#define ABSENCE_H

#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/gregorian/greg_duration.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <string>

class Absence {
	public:
		Absence() = delete;
		Absence(const boost::gregorian::date &beg, boost::gregorian::date end,
				const std::string &str)
			: period(beg, end), days(end - beg), comment(str) {} 
		Absence(const boost::gregorian::date_period &p, const std::string &str) :
			period(p), days(p.length()), comment(str) {}
		auto dates() const { return period; } 
	//	auto length() const { return days; }
		auto reason() const { return comment; }
	private:
		boost::gregorian::date_period period;
		boost::gregorian::date_duration days;
		std::string comment;
};

#endif /* ifndef ABSENCE_H */
