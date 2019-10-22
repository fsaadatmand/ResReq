#include "Date.h"
#include <array>
#include <ctime>
#include <exception>
#include <iomanip>
#include <ostream>
#include <string>
#include <sstream>

const std::array<int, Date::N>
Date::daysPerMonth = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// constructor
Date::Date(const std::string &s)
{

	std::istringstream stream(s);
	stream.imbue(std::locale("en_US.UTF-8"));
	if (!(stream >> std::get_time(&data, "%Y/%m/%d")))
		throw std::invalid_argument("invalid date format");
	data.tm_yday = toYearDay();
	data.tm_wday = (daysSinceYear0() + 365) % 7;
}

// member functions
time_t
Date::numberOfDays() const
{
	if (this->isLeapYear() && get_month() == 2)
		return 29; 
	return Date::daysPerMonth[get_month() - 1];
}

// convert date to year day
int
Date::toYearDay() const
{
	auto yearDay = get_day();
	for (decltype(get_month()) i = 0; i < get_month() - 1; ++i)
		yearDay += Date::daysPerMonth[i];
	return yearDay;
}

long int
Date::daysSinceYear0() const noexcept
{
	return get_year() * 365 + get_yearDay() + this->countLeapYears();
}

int
Date::countLeapYears() const noexcept
{
	auto years = get_year();
	if (get_month() <= 2)
		--years;
	return years / 4 - years / 100 + years / 400;
}

bool
Date::isLeapYear() const noexcept
{
	return (!(get_year() % 4) && get_year() % 100) || !(get_year() % 400);
}

// non-member functions
int operator-(const Date &lhs, const Date &rhs) 
{
	return lhs.daysSinceYear0() - rhs.daysSinceYear0();
}

bool operator==(const Date &lhs, const Date &rhs)
{
	return lhs.get_year() == rhs.get_year() &&
		   lhs.get_month() == rhs.get_month() &&
		   lhs.get_year() == rhs.get_year();
}

bool operator!=(const Date &lhs, const Date &rhs)
{
	return !(lhs == rhs);
}

bool operator<(const Date &lhs, const Date &rhs)
{
	if ((lhs - rhs) < 0)
		return true; 
	return false;
}

std::ostream& operator<<(std::ostream &os, const Date &d)
{
	os << d.get_year() << '/' << d.get_month() << '/' << d.get_year();
	return os;
}

