#pragma once
#include <iosfwd>

class DateTime
{
	//count of our fields
	const static unsigned s_membersCount = 5;
	//count of validate functions to create static array of function pointers
	const static unsigned s_functionsCount = 4;


	unsigned m_day;
	unsigned m_month;
	unsigned int m_year;
	unsigned m_hour;
	unsigned m_minute;

	//functions will check if the values, passed to out constructor are correct
	//for example: m_month can not me more that 12, and if it is we will throw an exception (watch class [WrongInput])
	bool validateMonth()  const;
	bool validateDay()    const;
	bool validateHour()   const;
	bool validateMinute() const;

	//we defining the array of function pointers and initialize it with our validate functions
	//we will use in in constructor to check, if our input values are correct
	typedef bool (DateTime::*p_Func)() const;
	p_Func validateFunctions[s_functionsCount] =
	{
		&DateTime::validateMonth,
		&DateTime::validateDay,
		&DateTime::validateHour,
		&DateTime::validateMinute
	};
	
public:
	DateTime(unsigned day, unsigned month, unsigned int year, unsigned hour, unsigned minute);
	bool operator<(const DateTime &obj) const;
	//bool operator>(const DateTime &obj) const;
	friend std::ostream &operator<<(std::ostream &os, const DateTime &obj);

	//Exception class
	//It contains the information about the field, which was wrong initialized
	class WrongInput
	{
		const unsigned m_fieldId;
		char *m_fieldName;

	public:
		WrongInput(unsigned fieldIndex)
			: m_fieldId(fieldIndex)
		{
			switch (m_fieldId)
			{
			case 0: {	m_fieldName = new char(*"Month");   break;	}
			case 1: {	m_fieldName = new char(*"Day");     break;	}
			case 2: {	m_fieldName = new char(*"Hour");    break;	}
			case 3: {	m_fieldName = new char(*"Minute");			}
			}
		}

		~WrongInput()
		{
			delete m_fieldName;
		}

		unsigned getFieldId()  const { return m_fieldId;   }
		char  *getFieldName()  const { return m_fieldName; }
	};
};