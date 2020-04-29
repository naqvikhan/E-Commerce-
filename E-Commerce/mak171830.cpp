//Project 3
//M Naqvi Aziz Khan
// <mak171830>

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "product.h"

using namespace std;

//Function prototypes
void sortItems(Product array[],int count);
bool readData (Product array[], int counter);
bool checkItems(Product array[], Product finalArray[], string orderNumber, int count,Product &returnInventory);
void updateInventory(Product array[],int count);
void output(Product array[]);
void printTempItems(Product array[]);
void editInventory(bool costChange, Product array[], int count);
Product* addToCart(Product array[],int count);



int main()
{

	//declaring and opening Data file
	ifstream inFile;
	inFile.open("ProductData.csv");

	int count = 0;
	int arrayIndex = 0;

	inFile >> count;

	Product *input = new Product[count];
	Product *finalCart = new Product[count];

	//returns 0 if data cannot be read
	if (!readData(input,count))
	{
		return 0;
	}

	//calling the function sortArray
	sortItems(input,count);
	bool admin = true;
	bool valid = false;

	while (!valid)
		{
			char value;
			cout << "Please type A for Admin interface or C for Customer interface" << endl;
			cin >> value;
			value = toupper(value);

			if (value == 'C')
			{
				valid = true;
			}
			else if (value == 'A')
			{
				admin = true;
				while (admin)
				{
					cout << "Type A to add to inventory, Q to update an item quantity, P to update a item price, or E to quit the program." << endl;

					cin >> value;

					value = toupper(value);

					switch(value)
					{
					case 'A':
						input = addToCart(input,count);
						count += 1;
							break;
						case 'Q':
							editInventory(false, input,count);
							break;
						case 'P':
							editInventory(true, input, count);
							break;
						case 'E':
							admin = false;
							break;
						default:
							cout << "Input invalid, please try again." << endl;
							break;
					}
				}
			}
			else
			{
				cout << "Incorrect input, please try again." << endl;
			}
		}

	int finalCartCount = 0;
	while (finalCartCount < count)
	{
		//variables to keep track of array items
		string orderNumber;
		Product returnedList;

		cout << "Please enter an order number or '1' to exit." << endl;
		cout << "Type 'CART' to view existing items in the cart." << endl;

		getline(cin, orderNumber,'\n');

		//Check for exit code
		if (orderNumber == "1")
		{
			break;
		}

		//Allows user to view their cart
		if (orderNumber == "CART" || orderNumber == "Cart" || orderNumber == "cart")
		{
			printTempItems(finalCart); //calling print cart function
			continue;
		}

		//if items are available, move to cart
		if (checkItems(input,finalCart, orderNumber,count,returnedList))
		{
			finalCart[arrayIndex] = returnedList;

			//Keep track of current index
			arrayIndex++;
			finalCartCount++;
		}
	}

	//calling output function
	output(finalCart);

	//calling updateInventory function
	updateInventory(input,count);

	inFile.close();

	//delete to clear existing memory
	delete[] input;
	delete[] finalCart;

	return 0;
}


//This function will take in the array and sort it
// in ascending order, using the bubble sort method

void sortItems(Product array[],int count)
{
	int swapped = 1;
	//Go until no more swaps
	while (swapped > 0)
	{
		swapped = 0;
		for (int i = 0; i < count - 1; i++)
		{
			//Check for greater then next value.
			if (array[i].getItemNumber() > array[i + 1].getItemNumber())
			{
				Product swap = array[i];
				array[i] = array[i + 1];
				array[i + 1] = swap;
				swapped++;
			}
		}
	}
}


//function will store the data of the file into a passed array
//converting the data into all the appropriate data types


bool readData(Product array[], int counter)
{
	//Open input file stream
	ifstream inFile;
	inFile.open("ProductData.csv");

	//Check to see if file is open
	if (inFile.is_open())
	{
		//clear the top line of the csv file
		string topLine;
		getline(inFile,topLine);
		getline(inFile, topLine);
		string value1 = "";
		string value2= "";
		string value3 = "";
		string value4 = "";

		//Get all lines
		for(int i = 0; i < counter; i++)
		{
			getline(inFile, value1, ',');
			getline(inFile, value2, ',');
			getline(inFile, value3, ',');
			getline(inFile, value4, '\n');
			Product list(stoi(value1), value2, stod(value3), stoi(value4));
			array[i] = list;
		}


	}
	else
	{
		cout << "Could not open file" << endl;
		return 0;
	}

	//Close file
	inFile.close();
	return 1;
}


//This function checks the array for the order, and then if it exist, assigns the values of the order.

bool checkItems(Product array[], Product finalArray[], string orderNumber, int count, Product &returnInventory)
{
	bool itemFound = false;
	int itemIndex = 0;

	//Check array for item
	for (int i = 0; i < count; i++)
	{
		Product tempArray = array[i];

		//check to see if item is available
		if (tempArray.getItemNumber() == stoi(orderNumber))
		{
			cout << tempArray.name + ": $" << fixed << setprecision(2) << tempArray.cost << endl;
			itemIndex = i;
			itemFound = true;
			returnInventory = tempArray;
			break;
		}
	}

	if (itemFound)
	{
		//Check if item is in stock
		if (returnInventory.quantity == 0)
		{
			cout << "The specified item is out of stock" << endl;
			return false;
		}

		bool stockAvailable = true;

		//Check if the amount the user wants is available
		while (stockAvailable)
		{
			string count;
			cout << "There are " << returnInventory.quantity << " Items in stock." << endl;
			cout << "How many items would you like to purchase?" << endl;

			getline(cin, count, '\n');

			int _itemCount = atoi(count.c_str());

			//compare stock to asked value
			if (_itemCount <= returnInventory.quantity)
			{
				int counter = 0;
				bool alreadyExist = false;

				while (alreadyExist == false)
				{
					Product temp1 = finalArray[counter];
					if (temp1.name == "")
						break;
					if (temp1.getItemNumber() == stoi(orderNumber))
					{
						alreadyExist = true;
						break;
					}
					counter++;
				}

				if (!alreadyExist)
				{
					//Set new list item quantity
					returnInventory.quantity = stoi(count);

					Product tempList = array[itemIndex];
					//Subtract quantity from original list
					tempList.quantity = tempList.quantity - returnInventory.quantity;
					array[itemIndex] = tempList;
					return true;
				}
				else
				{
					finalArray[counter].quantity = finalArray[counter].quantity + stoi(count);

					Product tempList = array[itemIndex];
					//Subtract quantity
					tempList.quantity = tempList.quantity - stoi(count);
					array[itemIndex] = tempList;
					return false;
				}
			}
			else
			{
				cout << "There are not enough inventory items to provide that amount, please enter amount and try again." << endl;
			}
		}
		return true;
	}
	else
	{
		cout << "The item specified could not be found." << endl;
		return false;
	}
}


//this function will reduce the items bought by the customer and
//update the product inventory file

void updateInventory(Product array[],int count)
{
	ofstream outputStream;

	outputStream.open("ProductData.csv");

	outputStream << count << endl;
	outputStream << "Item Number,Name,Cost,Qty" << endl;

	int counter = 0;
	for(int i = 0; i < count; i++)
	{
		Product tempList = array[counter];
		outputStream << tempList.getItemNumber() << ',' << tempList.name << ',' <<
			fixed << setprecision(2) << tempList.cost << ',' << tempList.quantity << endl;
		counter++;
	}

	outputStream.close();
}



//Function to get customer details and output the cart items
//onto the console as well as into the .txt file

void output(Product array[])
{
	string customerName;
	string addressLine1;
	string addressLine2;
	double total = 0;
	ofstream outputStream;

	cout << "Please enter your full name." << endl;

	getline(cin, customerName, '\n');

	cout << "Please enter your address in the given format (house number, street name)(Line 1)" << endl;

	getline(cin, addressLine1, '\n');

	cout << "Please enter your city, state and ZIP in the following format: (City, State(XX) ZIP)(Line 2)" << endl;

	getline(cin, addressLine2, '\n');

	cout << "Customer Name: " << customerName << endl;
	cout << "Customer Address: " << addressLine1 << endl;
	cout << "                  " << addressLine2 << endl;
	cout << endl;

	cout << setw(24) << left << "Item Number" << setw(24) << left << "Item Name" << setw(24) << left << "Quantity" << setw(24) << left << "Unit Cost" << setw(24) << left << "Total" << endl;
	cout << "------------------------------------------------------------------------------------------------------" << endl;
	int counter = 0;
	while(array[counter].name != "")
	{
		Product tempList = array[counter];

		if (counter != 0)
			cout << "---" << endl;

		cout << setw(24) << left << tempList.getItemNumber() << setw(24) << left << tempList.name << setw(24) << left << tempList.quantity << setw(24) << left << tempList.cost <<
			setw(24) << left << fixed << setprecision(2) << tempList.quantity * tempList.cost << endl;

		total += (tempList.quantity * tempList.cost);
		counter++;
	}
	cout << "------------------------------------------------------------------------------------------------------" << endl;
	cout << setw(96) << right << "Total: $" << fixed << setprecision(2) << total << endl;


	//To write the items of the shopping cart into a .txt file
	//open/create and open a .txt file
	outputStream.open("invoice.txt");

	if (outputStream.is_open())
	{
		total = 0;
		outputStream << "Customer Name: " << customerName << endl;
		outputStream << "Customer Address: " << addressLine1 << endl;
		outputStream << "                  " << addressLine2 << endl << endl;

		outputStream << setw(24) << left << "Item Number" << setw(24) << left << "Item Name" << setw(24) << left << "Quantity" << setw(24) << left << "Unit Cost" << setw(24) << left << "Total" << endl;
		outputStream << "------------------------------------------------------------------------------------------------------" << endl;

		counter = 0;
		while (array[counter].name != "")
		{
			Product tempList = array[counter];

			if (counter != 0)
				outputStream << "---" << endl;

			outputStream << setw(24) << left << tempList.getItemNumber() << setw(24) << left << tempList.name << setw(24) << left << tempList.quantity << setw(24) << left << tempList.cost <<
				setw(24) << left << fixed << setprecision(2) << tempList.quantity * tempList.cost << endl;

			total += (tempList.quantity * tempList.cost);
			counter++;
		}
		outputStream << "------------------------------------------------------------------------------------------------------" << endl;
		outputStream << setw(96) << right << "Total: $" << fixed << setprecision(2) << total << endl;
		//close stream
		outputStream.close();
	}
	else
	{
		cout << "Could not open file invoice.txt" << endl;
	}
}


//This function allows the user to add items to the cart


Product* addToCart(Product array[],int count)
{
	Product *returns = new Product[(count + 1)];

	for (int i = 0; i < count; i++)
	{
		returns[i] = array[i];
	}

	int itemNum = 0;
	string name;
	double cost = 0;
	int quant = 0;
	bool valid = false;

	while (!valid)
	{
		cout << "Please provide the item Number" << endl;

		cin >> itemNum;

		if (cin.good())
		{
			bool exists = false;
			for (int i = 0; i < count; i++)
			{
				if (array[i].getItemNumber() == itemNum)
				{
					cout << "Item already exist, please select new item number." << endl;
					exists = true;
				}
			}
			if (exists)
				continue;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			break;
		}
		else
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "Invalid input, please try again" << endl;
		}
	}

	while (!valid)
	{
		cout << "Please provide the item name" << endl;
		cin >> name;

		if (cin.good())
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			break;
		}
		else
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "Invalid input, please try again" << endl;
		}
	}

	while (!valid)
	{
		cout << "Please provide the item cost" << endl;
		cin >> cost;

		if (cin.good())
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			break;
		}
		else
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "Invalid input, please try again" << endl;
		}
	}

	while (!valid)
	{
		cout << "Please provide the item quantity" << endl;
		cin >> quant;

		if (cin.good())
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			break;
		}
		else
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "Invalid input, please try again" << endl;
		}
	}

	Product temp(itemNum, name, cost, quant);
	returns[(count)] = temp;

	return returns;
}

//This function will read allow the the the admin to make changes to the product quantity and product cost

//Struggled to create the derived classes o

void editInventory(bool costChange, Product array[], int count)
{
	int itemNumber = 0;
	bool valid = false;

	while (!valid)
	{
		cout << "Please enter an item number" << endl;
		cin >> itemNumber;

		if (!cin.good())
		{
			cout << "Incorrect input, please try again" << endl;
		}
		else
			valid = !valid;
	}
	bool itemFound = false;
	for (int i = 0; i < count; i++)
	{
		if (array[i].getItemNumber() == itemNumber)
		{
			itemFound = true;
			double cost = 0;
			int quantity = 0;
			if (costChange)
			{
				cout << "Please provide the new item cost" << endl;
				cin >> cost;

				if (cin.good())
				{
					array[i].cost = cost;
				}
			}
			else
			{
				cout << "Please provide the new item quantity" << endl;
				cin >> quantity;

				if (cin.good())
				{
					array[i].quantity = quantity;
				}
			}
			break;
		}
	}
	if (!itemFound)
	{
		cout << "Inventory item not found." << endl;
	}
}

//This function will print all items in the cart selected by the customer

void printTempItems(Product array[])
{
	cout << "Cart:" << endl;
	int index = 0;

	while (array[index].name != "")
	{
		Product tempList = array[index];
		cout << "Item: " << tempList.name << " - Quantity " << tempList.quantity << endl;
		index++;
	}
}




