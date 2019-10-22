#ifndef DATE_H
#define DATE_H

#include <array>
#include <ostream>
#include <string>
#include <boost/date_time.hpp>

class Date {
	friend std::ostream& operator<<(std::ostream&, const Date &);
	friend bool operator==(const Date &, const Date &);
	friend bool operator<(const Date &, const Date &);
	friend int operator-(const Date &, const Date &); 
	public:
		Date() = default;
		Date(const std::string &);
		int get_day() const noexcept { return data.tm_mday; };
		int get_month() const noexcept { return data.tm_mon + 1; };
		int get_year() const noexcept { return data.tm_year + 1900; };
		int get_weekDay() const noexcept { return data.tm_wday + 1; };
		int get_yearDay() const noexcept { return data.tm_yday + 1; };
		long int daysSinceYear0() const noexcept;
		int countLeapYears() const noexcept;
		bool isLeapYear() const noexcept;
	private:
		int toYearDay() const;
		static constexpr std::size_t N = 12;
		static const std::array<int, N> daysPerMonth;
		std::tm data = {};
		time_t numberOfDays() const;
//		time_t day = 0;
//		time_t month = 0;
//		time_t year = 0;
};

int operator-(const Date &, const Date &); 
bool operator==(const Date &, const Date &);
bool operator!=(const Date &, const Date &);
bool operator<(const Date &, const Date &);
std::ostream& operator<<(std::ostream &, const Date &);
#endif // DATE_H
