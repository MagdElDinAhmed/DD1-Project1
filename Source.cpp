#include <iostream>
#include <vector>
#include "implicant.h"
#include <fstream>
#include <cctype>
using namespace std;
int main()
{
	vector<int> function_minterms;
	vector<int> function_dontcares;
	vector <implicant> prime_implicants;
	int no_of_variables;
	ifstream file;
	file.open("input.txt");
	char temp;
	string temp2;
	file.get(temp);
	if (isdigit(temp))
	{
		if (isdigit(file.peek()))
		{
			temp2 = temp;
			file.get(temp);
			temp2 += temp;
			function_minterms.push_back(stoi(temp2));
		}
		else {
			no_of_variables = (int)(temp - '0');
		}
		
	}
	file.get(temp);//disposing of the '\n'
	do
	{
		file.get(temp);
		if (isdigit(temp))
		{
			if (isdigit(file.peek()))
			{
				temp2 = temp;
				file.get(temp);
				temp2 += temp;
				function_minterms.push_back(stoi(temp2));
			}
			else {
				no_of_variables = (int)(temp - '0');
			}
		}
	} while (file.peek() != '\n');

	file.get(temp);//disposing of the '\n'
	while (!file.eof())
	{
		file.get(temp);
		if (isdigit(temp))
		{
			if (isdigit(file.peek()))
			{
				temp2 = temp;
				file.get(temp);
				temp2 += temp;
				function_dontcares.push_back(stoi(temp2));
			}
			else {
				no_of_variables = (int)(temp - '0');
			}
		}
	}
	function_dontcares.pop_back(); //haphazard way of fixing a glitch that causes the last don't care to be read twice

	for (int i = 0; i < function_minterms.size(); i++)
	{
		cout << function_minterms[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < function_dontcares.size(); i++)
	{
		cout << function_dontcares[i] << " ";
	}
	

}
/*
* So the plan is to operate on 20 bits
* Program converts these terms into a binary number and countes the number of 1s to identify chunk
* We input minterms and don't cares by their number
* The program generates the deatils of these minterms accordingly
* We'll need a function that converts decimal to binary inside the implicant class
* We compare each element in a chunk to each element in the chunk after it
* If they combine then we check the presence of the combination in the next column
* If there is no next column, create it
* If the combined implicant exists already, add the minterms combined into the object
* If it doesn't exist, create the combined implicant accordingly (we'll need a function for that)
* 
*/