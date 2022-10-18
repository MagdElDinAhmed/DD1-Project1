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
	int no_of_variables=-1;
	ifstream file;
	file.open("input.txt");
	char temp;
	string temp2;
	bool valid_in = true;

	//this bulk of code extracts the number of variables

	file.get(temp);
	if (isdigit(temp))
	{
		if (isdigit(file.peek()))
		{
			temp2 = temp;
			file.get(temp);
			if (isdigit(file.peek())) //checks if number has 3 digits
			{
				valid_in = false;
			}
			temp2 += temp;
			no_of_variables= stoi(temp2);
		}
		else {
			no_of_variables = (int)(temp - '0');
		}
		
	}
	if (no_of_variables < 1 || no_of_variables>20)
	{
		valid_in = false;
	}
	file.get(temp);//disposing of the '\n'

	//this do while loop extracts the minterms
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
				function_minterms.push_back((int)(temp - '0'));
			}
		}
		else if (isalpha(temp) || (ispunct(temp) && temp != ','))
		{
			valid_in = false;
		}
	} while (file.peek() != '\n' && valid_in);

	file.get(temp);//disposing of the '\n'

	//this piece of code extracts the don't care terms
	do
	{
		file.get(temp);
		if (isdigit(temp) && temp!=EOF)
		{
			if (isdigit(file.peek()))
			{
				temp2 = temp;
				file.get(temp);
				temp2 += temp;
				function_dontcares.push_back(stoi(temp2));
			}
			else {
				function_dontcares.push_back((int)(temp - '0'));
			}
		}
		else if (isalpha(temp) || (ispunct(temp) && temp != ','))
		{
			valid_in = false;
		}
	} while (file.peek() != EOF && valid_in);



	if (valid_in)
	{
		//program code
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
	else
	{
		cout << "invalid input";
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