#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

#include "date_time.h"


struct Data
{
	DateTime m_time;
	unsigned m_truckNumber;
	float m_weight;

	Data(DateTime time, unsigned number, float weight)
		: m_time(time), m_truckNumber(number), m_weight(weight) {}
	bool operator<(const Data &obj) { return m_time < obj.m_time; }
	//bool operator>(const Data &obj) { return m_time > obj.m_time; }
	friend std::ostream &operator<<(std::ostream &os, const Data &obj);
};
std::ostream &operator<<(std::ostream &os, const Data &obj)
{
	os << obj.m_time << ", " << obj.m_weight;
	return os;
}

std::string read_file(int &linesCount);

//struct to contain all parced data and return it as one
struct all_data
{
	unsigned day, month;
	unsigned int year;
	unsigned hour, minute, number;
	float weight;
};
all_data parce_line(std::string line);

void fill_array(std::vector<Data> &vec, const std::string &file, unsigned linesCount);

int get_trucks_count(const std::vector<Data> &vec);

void write_to_file(std::vector<std::vector<Data> > &vec);

void sort(std::vector<Data> &vec);

int main()
{
	int linesCount;
	std::string file( read_file(linesCount) );

	std::vector<Data> data;

	fill_array(data, file, linesCount);

	int truckCount (get_trucks_count(data));

	int *trucks = (int *)_alloca((truckCount) * sizeof(int));
	memset(trucks, 0, (truckCount)*sizeof(int));

	for (int i(0); i < linesCount; ++i)
		++trucks[data[i].m_truckNumber-1];

	std::vector<std::vector<Data> > table(truckCount);

	for (int i(0); i < truckCount; ++i)
		table[i].reserve(trucks[i]);

	for (int i(0); i < linesCount; ++i)
	{
		table[data[i].m_truckNumber-1].push_back(data[i]);
	}
	
	write_to_file(table);

	std::cin.get();
	return 0;
}


std::string read_file(int &linesCount)
{
	std::ifstream file("resources/table.txt");

	linesCount = 0;

	{
		std::string dummy;
		while (!file.eof())
		{
			std::getline(file, dummy);
			++linesCount;
		}
	}
	file.seekg(0);

	std::string str((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	file.close();
	return str;
}

all_data parce_line(std::string line)
{
	all_data data;
	std::string::size_type sz;
	std::string temp;

	auto lb(line.cbegin());
	auto le(line.cend());

	//parce day
	auto lb2(std::find(lb, le, '.'));
	temp.assign(lb, lb2);
	data.day = std::stoi(temp, &sz);
	std::advance(lb2, 1);

	//parce month
	lb = std::find(lb2, le, '.');
	temp.assign(lb2, lb);
	data.month = std::stoi(temp, &sz);
	std::advance(lb, 1);

	//parce year
	lb2 = std::find(lb, le, ' ');
	temp.assign(lb, lb2);
	data.year = std::stoi(temp, &sz);
	std::advance(lb2, 1);

	//parce hour
	lb = std::find(lb2, le, ':');
	temp.assign(lb2, lb);
	data.hour = std::stoi(temp, &sz);
	std::advance(lb, 1);

	//parce minute
	lb2 = std::find(lb, le, ',');
	temp.assign(lb, lb2);
	data.minute = std::stoi(temp, &sz);
	std::advance(lb2, 2);

	//parce truck number
	{
		auto pred = 
		[](char c)
		{
			return (isdigit(c));
		};
		lb = std::find_if(lb2, le, pred);
	}
	lb2 = std::find(lb, le, ',');
	temp.assign(lb, lb2);
	data.number = std::stoi(temp, &sz);
	std::advance(lb2, 2);

	//parce weight
	temp.assign(lb2, le);
	data.weight = std::stof(temp, &sz);

	return data;
}

void fill_array(std::vector<Data> &vec, const std::string &file, unsigned linesCount)
{
	vec.reserve(linesCount);
	std::string line;
	std::string::const_iterator it1(file.cbegin());
	std::string::const_iterator it2(std::find(file.cbegin(), file.cend(), '\n'));
	all_data tempData;
	for (unsigned i(0); i < linesCount; ++i)
	{
		line.assign(it1, it2);
		tempData = parce_line(line);
		if (i != 4)
			it1 = ++it2;
		it2 = find(it1, file.cend(), '\n');

		vec.emplace_back(
			DateTime(tempData.day, tempData.month, tempData.year, tempData.hour, tempData.minute),
			tempData.number,
			tempData.weight
		);
	}
}

int get_trucks_count(const std::vector<Data> &vec)
{
	auto pred =
	[](const Data &obj1, const Data &obj2)
	{
		return (obj1.m_truckNumber < obj2.m_truckNumber);
	};

	auto truckCountIt(std::max_element(vec.begin(), vec.end(), pred));

	int truckCount = (*truckCountIt).m_truckNumber;
	return truckCount;
}

void write_to_file(std::vector<std::vector<Data> > &vec)
{
	std::ofstream output;
	for (unsigned i(0); i < vec.size(); ++i)
	{
		//sort(vec[i]);
		std::sort(vec[i].begin(), vec[i].end(), [](const Data &obj1, const Data &obj2) {return obj1.m_time < obj2.m_time; });
		output.open("outputs/truck" + std::to_string(i + 1)+".txt");
		float weight(0.f);
		for (unsigned j(0); j < vec[i].size(); ++j)
		{
			output << vec[i][j]<<'\n';
			weight += vec[i][j].m_weight;
		}
		output << weight;
		output.close();
	}
}

/*void sort(std::vector<Data> &vec)
{
	for (unsigned i = 1; i < vec.size(); i++)
		for (unsigned j = i; j > 0 && vec[j - 1] > vec[j]; j--)
			std::swap(vec[j], vec[j - 1]);
}*/