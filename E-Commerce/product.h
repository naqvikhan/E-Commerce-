#pragma once
#include<string>
using namespace std;

class Product
{
private:

	int itemNumber;

public:

	std::string name;
	double cost;
	int quantity;
	Product();
	Product(int item, string str_name, double _cost, int _quantity);
	int getItemNumber();
	void setItemNumber(int num);
	void setCost(double _cost);
	~Product();
};
