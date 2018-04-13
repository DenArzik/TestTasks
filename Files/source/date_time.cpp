#include "date_time.h"
#include <iostream>
#include <algorithm>


DateTime::DateTime(unsigned day, unsigned month, unsigned int year, unsigned hour, unsigned minute)
	:m_day(day), m_month(month), m_year(year), m_hour(hour), m_minute(minute)
{
	for (int i(0); i < s_functionsCount; ++i)
		if ( !(this->*validateFunctions[i])() )
			throw WrongInput(i);
}

bool DateTime::validateMonth() const
{
	if(m_month<1 || m_month>12)
		return false;

	return true;
}

bool DateTime::validateDay() const
{
	//we check the max possible day in current month due to that table
	//////////////////////////////////////////////////////////
	//  x	1  |2	|3	|4	|5	|6	|7	|8	|9	|10	|11	|12 //
	//f(x)	31 |28	|31	|30	|31	|30	|31	|31	|30	|31	|30	|31 //
	//////////////////////////////////////////////////////////
	if (m_month == 2)
	{
		if (m_day < 1 || m_day>28)
			return false;
	}
	else if ((m_month == 4) || (m_month == 6) || (m_month == 9) || (m_month == 1))
	{
		if (m_day < 1 || m_day>30)
			return false;
	}
	else if (m_day<1 || m_day>31)
		return false;

	return true;
}

bool DateTime::validateHour() const
{
	if (m_hour > 24)
		return false;

	return true;
}

bool DateTime::validateMinute() const
{
	if (m_minute > 59)
		return false;

	return true;
}

bool DateTime::operator<(const DateTime &obj) const
{
	int arrThis[s_membersCount], arrObj[s_membersCount];

	arrThis[0] = m_year;	arrObj[0] = obj.m_year;
	arrThis[1] = m_month;	arrObj[1] = obj.m_month;
	arrThis[2] = m_day;		arrObj[2] = obj.m_day;
	arrThis[3] = m_hour;	arrObj[3] = obj.m_hour;
	arrThis[4] = m_minute;	arrObj[4] = obj.m_minute;

	__int8 arr[s_membersCount];
	memset(arr, 0, s_membersCount * sizeof(__int8));

	for (int i(0); i < s_membersCount; ++i)
	{
		if (arrThis[i] < arrObj[i])
			arr[i] = 1;
		else if (arrThis[i] == arrObj[i])
			arr[i] = 0;
		else arr[i] = -1;
	}

	for (int i(0); i < s_membersCount; ++i)
	{
		if (arr[i] != 0)
		{
			if (arr[i] == 1) return true;
			else return false;
		}
	}
}

/*bool DateTime::operator>(const DateTime &obj) const
{
	int arrThis[s_membersCount], arrObj[s_membersCount];

	arrThis[0] = m_year;	arrObj[0] = obj.m_year;
	arrThis[1] = m_month;	arrObj[1] = obj.m_month;
	arrThis[2] = m_day;		arrObj[2] = obj.m_day;
	arrThis[3] = m_hour;	arrObj[3] = obj.m_hour;
	arrThis[4] = m_minute;	arrObj[4] = obj.m_minute;

	for (int i(0); i < s_membersCount; ++i)
	{
		if (arrThis[i] < arrObj[i])
			return false;
	}

	return true;
}*/

std::ostream &operator<<(std::ostream &os, const DateTime &obj)
{
	os << obj.m_day << '.' << obj.m_month << '.' << obj.m_year << ' ' << obj.m_hour << ':' << obj.m_minute;
	return os;
}
