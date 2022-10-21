#include <iostream>
#include <vector>
#include "implicant.h"
#include <fstream>
#include <cctype>
#include<math.h>
#include <set>
#include <sstream>
#include <string>
#include <exception>

//#include "InputLib.h"
using namespace std;
void Extract(vector<int>& fucntion_minterms, vector<int>& fucntion_dontcares, int& no_of_variables, bool& valid_in);
bool CheckValidVarSize(string in, int no_of_variables);
bool CheckValidTerm(string in, int no_of_variables, set<int>& terms);
bool CheckRangeVarCount(string in, int size);
bool CheckRangeTerm(string in, int size);
bool CheckCharacter(string in);
vector<implicant> PrimeImp( vector<int> mint, vector<int> dc, int num_of_var);
vector<implicant> ImplicantsList(vector<int> minterms, vector<int> dont_cares);
int NumChunks(vector<implicant> terms);
//vector<string> toString(vector<implicant> m);



int main()
{
	vector<int> function_minterms;
	vector<int> function_dontcares;
	vector <implicant> prime_implicants;
	int no_of_variables=-1;
	int w=0;
	bool valid_in = true;


	Extract(function_minterms, function_dontcares, no_of_variables, valid_in);

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


	w = pow(2, no_of_variables);

	prime_implicants = PrimeImp(function_minterms, function_dontcares, no_of_variables);



	for(int i=0; i<= prime_implicants.size(); i++)
	cout << prime_implicants[i].imp << endl;

}

void Extract(vector<int>& fucntion_minterms, vector<int>& fucntion_dontcares, int& no_of_variables, bool& valid_in)
{
	ifstream file;
	string temp, temp2;
	char new_line_trash;
	bool new_line=false;
	set<int>terms;
	file.open("input.txt");
	getline(file, temp);
	if (!CheckValidVarSize(temp, 20))
	{
		valid_in = false;
		return;
	}
	no_of_variables = stoi(temp);
	while (!new_line && valid_in)
	{
		getline(file, temp, ','); //check for new line
		for (int i = 0; i < temp.length(); i++)
		{
			if (temp[i] == '\n')
			{
				temp2 = temp.substr(i+1, string::npos);
				temp = temp.substr(0, temp.length() - (temp.length() - i));
				new_line = true;
			}

		}
		valid_in = CheckValidTerm(temp, no_of_variables, terms);
		if (valid_in)
		{
			fucntion_minterms.push_back(stoi(temp));
		}
	}
	if (valid_in)
	{
		valid_in = CheckValidTerm(temp2, no_of_variables, terms);
		if (valid_in)
		{
			fucntion_dontcares.push_back(stoi(temp2));
		}
	}
	

	while (!file.eof() && valid_in)
	{
		getline(file, temp, ',');
		valid_in = CheckValidTerm(temp, no_of_variables, terms);
		if (valid_in)
		{
			fucntion_dontcares.push_back(stoi(temp));
		}
	}
}

bool CheckValidVarSize(string in, int no_of_variables)
{
	bool valid = true;
	valid = (CheckRangeVarCount(in, 20) && CheckCharacter(in));
	return valid;
}
bool CheckValidTerm(string in, int no_of_variables, set<int>& terms)
{
	bool valid = true;
	int size = (pow(2, no_of_variables) - 1);
	valid = (CheckRangeTerm(in, size) && CheckCharacter(in) && (terms.find(stoi(in)) == terms.end()));
	if (valid)
	{
		terms.insert(stoi(in));
	}
	return valid;
}
bool CheckRangeVarCount(string in, int size)
{
	bool valid;
	int tempNum = stoi(in);
	if (tempNum > 0 && tempNum <= size)
	{
		valid = true;
	}
	else
	{
		valid = false;
	}
	return valid;
}
bool CheckRangeTerm(string in, int size)
{
	bool valid;
	int tempNum = stoi(in);
	if (tempNum >= 0 && tempNum <= size)
	{
		valid = true;
	}
	else
	{
		valid = false;
	}
	return valid;
}
bool CheckCharacter(string in)
{
	bool valid = true;
	int i = 0;
	while (i < in.length() && valid)
	{
		if (isalpha(in[i]) || ispunct(in[i]))
		{
			valid = false;
		}
		i++;
	}
	return valid;
}

vector<implicant> PrimeImp(vector<int> mint, vector<int> dc, int num_of_var)
{ 
	vector<vector<vector<implicant>>> col;
	vector<vector<implicant>> chunk;
	vector<implicant> imp;
	vector<implicant> PrimeImplicantsList;
	vector<implicant> Has_it_all= ImplicantsList(mint,dc);
	vector<int> forMerge;
	int w = 0;
	forMerge.resize(Has_it_all.size());

	int num_of_chunks= NumChunks(Has_it_all);
	chunk.resize(num_of_chunks);

	
	for (int i = 0; i <= Has_it_all.size(); i++)
	{
		chunk[Has_it_all[i].chunk].push_back(Has_it_all[i]);
		col.push_back(chunk);
	}
	col.resize(col.size() + 1);
	
	for (int t = 0; t <= num_of_chunks; t++)     //chunks loop //looping on each chunk
	{
		for (int i = 0; i <= chunk.size(); i++)       //loops on each implicant in chunk one //2
		{
			for (int j = i+1; j <= chunk.size(); j++)  //Loops on each implicant in chunk two //3
			{
				vector<int> p = Has_it_all[i].delta(Has_it_all[j].imp);
				if( p.size()==1)
				Has_it_all[i].merge(Has_it_all[i].imp, Has_it_all[j].imp, forMerge);   //if 1, merge
			}

			if (Has_it_all[i].merged)                                 //i merged, put into chunk, then new col
			{
				chunk[Has_it_all[i].chunk].push_back(Has_it_all[i]);
				col.push_back(chunk);
				w++;
			}
			else                                                   //add to prime implicant list
			{
				PrimeImplicantsList.push_back(Has_it_all[i]);
			}
		}
		col.resize(col.size() + 1);
	}

	return PrimeImplicantsList;
}
int NumChunks(vector<implicant> terms)
{
	int m=-1;
	for (int i = 0; i <= terms.size()-1; i++)
	{
		for (int j = i; j <= terms.size(); j++)
		{
			m=max(terms[i].chunk, terms[j].chunk);
		}
	}
	return m;
}
vector<implicant> ImplicantsList(vector<int> minterms, vector<int> dont_cares)
{
	vector<implicant> j;
	for(int i=0; i<=minterms.size(); i++)
	{
		j.push_back(implicant(minterms[i]));
	}
	for (int i = 0; i <= dont_cares.size(); i++)
	{
		j.push_back(implicant(dont_cares[i]));
	}
	return j;
}
/*
vector<string> toString(vector<implicant> m)
{
	vector<string> temp;
	temp.resize(m.size());

	int i = 0;

	for (int j = 0; j <= m.size(); j++)
	{
		i = m[j];
		temp.push_back(to_string(i));
	}
}
*/


/*
* So the plan is to operate on 20 bits
* Program converts these terms into a binary number and countes the number of 1s to identify chunk
* We input minterms and don't cares by their number
* The program generates the details of these minterms accordingly
* We'll need a function that converts decimal to binary inside the implicant class
* We compare each element in a chunk to each element in the chunk after it
* If they combine then we check the presence of the combination in the next column
* If there is no next column, create it
* If the combined implicant exists already, add the minterms combined into the object
* If it doesn't exist, create the combined implicant accordingly (we'll need a function for that)
* 
*/