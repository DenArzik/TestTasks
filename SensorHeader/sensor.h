#pragma once

class Sensor
{
public:
	//Constructor will initialize values
	Sensor(double sensivity = 0.5, double acceptableFilling = 0.5, long long timeGap = 0);

	//It will do the same as constructor
	//just to give possibility to pass all data in a single line
	//and not to call set methods for each field
	//virtuality will give the possibility to overload it
	//and change the behavior in derived classes
	virtual void reInitialize(double sensivity, double acceptableFilling, long long timeGap = 0) = 0;

	//Main scan function
	//it is expected, that it will be called in some kind of endless loop
	//it will update inner data depending on scan results
	//there is a feature containing in [m_timeGap]
	//that var is responsible for delay between scannings
	//if bCanDoLoop is false, function will do nothing
	//by default [m_timeGap] is 0, and each calling will do something
	virtual void doScan() = 0;

	//common setters and getters
	void setSensivity(double sens);
	double getSensivity();

	void setAcceptableFilling(double fill);
	double getAcceptableFilling();

	double getIntensity();
	double getFilling();

	//maybe our scanner will be able to detect the objects count
	//at least derived classes will be able to override [doScan]
	//and set [m_objectsCount] there
	unsigned int getObjectsCount();

	bool getbCanDoLoop();

private:
	//maybe we willing to count how buch time our scanner is affected
	long long m_time{ 0 };
	//delay between scans
	long long m_timeGap{ 0 };

	//the value of power, that currently pushing on receiving element of scanner
	//it is expected to be in '%' (value between 0...100)
	double m_intensity{ 0.0 };

	//that value stands for the size or, to be more specific:
	//how much space the receiving affection is filing in the entire scope of sensor perception
	//also '%'
	double m_filling{ 0.0 };

	//if our sensor is able to detect affectiog objects count, it will
	unsigned int m_objectsCount{ 0 };

	//it is stands for how much pressure we need to do to perception element
	//to count it as an affection
	//also '%'
	double m_sensivity{ 0.5 };

	//it is stands for how much scope of perception element the affection is have to cover
	//to count it as an affection
	//also '%'
	double m_acceptableFilling{ 0.5 };

	//it stands for time calculation
	//and this is the first thing to call in [doScan]
	//if it is false (and it is false if the value of [m_timeGap]>0)
	//we won't do scan
	//also sets bCanDoLoop value
	void checkIfCanLoop();

	bool bCanDoLoop;
};
