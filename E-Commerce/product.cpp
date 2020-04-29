#include<iostream>
#include "product.h"
using namespace std;

Product::Product()
{
	itemNumber = 0;
	name = "";
	cost = 0;
	quantity = 0;
}

Product::Product(int item, string str_name, double _cost, int _quantity)
{
	itemNumber = item;
	name = str_name;
	cost = _cost;
	quantity = _quantity;
}

int Product::getItemNumber()
{
	return itemNumber;
}


void Product::setItemNumber(int num)
{
	itemNumber = num;
}

void Product::setCost(double _cost)
{
	cost = _cost;
}

Product::~Product()
{
}
