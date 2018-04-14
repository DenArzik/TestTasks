#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

#include "date_time.h"

//our table data
struct Data
{
	DateTime m_time;
	unsigned m_truckNumber;
	float m_weight;

	Data(DateTime time, unsigned number, float weight)
		: m_time(time), m_truckNumber(number), m_weight(weight) {}
	bool operator<(const Data &obj) { return m_time < obj.m_time; }
	bool operator>(const Data &obj) { return !operator<(obj); }
	friend std::ostream &operator<<(std::ostream &os, const Data &obj);
};
std::ostream &operator<<(std::ostream &os, const Data &obj)
{
	os << obj.m_time << ", " << obj.m_weight;
	return os;
}

//just read the text from the file and geting lines count as a function argument
std::string read_file(int &OUT_linesCount);

//struct to contain all parced data and return it as one
struct all_data
{
	unsigned day, month;
	unsigned int year;
	unsigned hour, minute, number;
	float weight;
};
//take single line from file and parce it to values
all_data parce_line(std::string line);

//fill in the data to vector array
void fill_array(std::vector<Data> &vec, const std::string &file, unsigned linesCount);

//get the overall amount of trucks
int get_trucks_count(const std::vector<Data> &vec);

//here we write all data, that match specific truck, sorting it and writing down to unique file 
void write_to_file(std::vector<std::vector<Data> > &vec);

//insertion sort
void sort(std::vector<Data> &vec);

int main()
{
	int linesCount;
	std::string file( read_file(linesCount) );

	//converting text data from file to numeric and write it down to vector
	std::vector<Data> data;
	fill_array(data, file, linesCount);

	//creating array size of trucks count
	//to count how much data matches each of truck
	int truckCount (get_trucks_count(data));
	int *trucks = (int *)_alloca((truckCount) * sizeof(int));	//creating dynamicky sized array on stack
	memset(trucks, 0, (truckCount)*sizeof(int));				//setting values to 0

	for (int i(0); i < linesCount; ++i)					//we increment the array value if the index matches the truckID
		++trucks[data[i].m_truckNumber-1];				/*(m_truckNumber-1) is because there is no truckID == 0*/

	//vector to contain vectors of data for specific truck
	/*should be Data *, but I failed with it*/
	std::vector<std::vector<Data> > table(truckCount);
	for (int i(0); i < truckCount; ++i)
		table[i].reserve(trucks[i]);

	//filling in that vector with data
	/*index is truck number*/
	for (int i(0); i < linesCount; ++i)
		table[data[i].m_truckNumber-1].push_back(data[i]);
	
	//writing that data down to files
	write_to_file(table);

	std::cout << "Done.\n";
	system("pause");
	return 0;
}


std::string read_file(int &linesCount)
{
	std::ifstream file("resources/table.txt");

	//get lines count firstly
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

	//then read whole file
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
	//reading [file] line by line, parcing it and filling the [vec]
	for (unsigned i(0); i < linesCount; ++i)
	{
		line.assign(it1, it2);
		tempData = parce_line(line);
		if (i != linesCount-1)			//to prevent iterators going out of range by incrementing
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
	auto pred =												/*for some reason it broke std::sort when i was trying to use it*/
	[](const Data &obj1, const Data &obj2)					/*in [write_to_file] instead of custom sort*/
	{
		return (obj1.m_truckNumber < obj2.m_truckNumber);
	};

	//getting the iterator pointing to the object with highest [m_truckNumber] value
	//this is gonna be our truckCount value
	auto truckCountIt(std::max_element(vec.begin(), vec.end(), pred));

	int truckCount = (*truckCountIt).m_truckNumber;
	return truckCount;
}

void write_to_file(std::vector<std::vector<Data> > &vec)
{
	std::ofstream output;
	for (unsigned i(0); i < vec.size(); ++i)
	{
		sort(vec[i]);													//sorting [vec] by time (operator< is overloaded in struct)
		output.open("outputs/truck" + std::to_string(i + 1)+".txt");
		float weight(0.f);												//also counting the overall weight	
		for (unsigned j(0); j < vec[i].size(); ++j)
		{
			output << vec[i][j]<<'\n';									//and writing it to file (operator<< is also overloaded)
			weight += vec[i][j].m_weight;
		}
		output << weight;
		weight = 0;
		output.close();
	}
}

void sort(std::vector<Data> &vec)
{
	for (unsigned i = 1; i < vec.size(); i++)
		for (unsigned j = i; j > 0 && vec[j - 1] > vec[j]; j--)
			std::swap(vec[j], vec[j - 1]);
}