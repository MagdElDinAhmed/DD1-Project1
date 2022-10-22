#include <iostream>
#include <vector>
#include "implicant.h"
#include <fstream>
#include <cctype>
#include<math.h>
#include <set>
#include <map>


using namespace std;
void Extract(vector<int>& fucntion_minterms, vector<int>& fucntion_dontcares, int& no_of_variables, bool& valid_in); //extracts all necessary information from the file and validates the input
bool CheckValidVarSize(string in, int no_of_variables); //checks if the number of variables input is valid
bool CheckValidTerm(string in, int no_of_variables, set<int>& terms); //checks if the term is valid
bool CheckRangeVarCount(string in, int size); //Checks if the number of variables is between 1 and 20 inclusive
bool CheckRangeTerm(string in, int size); //checks if the term is in the appropriate range
bool CheckCharacter(string in); //checks if the input has any non-numeric characters
vector<implicant> PrimeImp(vector<int> mint, vector<int> dc, int num_of_var); //creates a list of prime implicants
vector<implicant> ImplicantsList(vector<int> minterms, vector<int> dont_cares); //creates a disordered vector of all implicants from the minterms and don't care terms
int NumChunks(vector<implicant> terms); //counts the maximum number of chunks in the first column 

vector<implicant> RemoveDup(vector<implicant> p, int no_of_var); //removes duplicate implicants and merges their terms, minterms and don't cares
vector<implicant> RemoveZeros(vector<implicant> p, int no_of_var); //removes extra 0s from the binary representation of the prime implicants
void GenerateBoolean(vector<implicant> PIs, int no_of_variables);//generates the boolean expression for the implicants (Prime or Essential Prime)
//X0 is the most significant bit, as the number increases, the bit becomes less significant

vector<implicant> CleanUp(vector<implicant> prime_implicants); //makes sure no duplicate minterms are listed in the now merged prime implicants
void EPI(vector <implicant> prime_implicants, vector<int> function_minterms, int no_of_variables); //Finds EPIs, Outputs them, and outputs the minterms not covered by them


int main()
{
	vector<int> function_minterms;
	vector<int> function_dontcares;
	vector <implicant> prime_implicants;
	int no_of_variables = -1; //dummy value
	bool valid_in = true; //default true value


	Extract(function_minterms, function_dontcares, no_of_variables, valid_in);

	if (valid_in)
	{
		//Already explained what that does
		prime_implicants = PrimeImp(function_minterms, function_dontcares, no_of_variables);

		cout << endl;


		//these 2 lines are general clean up so that the prime implicants are usable for later
		prime_implicants = RemoveDup(prime_implicants, no_of_variables);



		prime_implicants = CleanUp(prime_implicants);

		//outputs the minterm values of each prime implicant
		cout << "Minterms covered by Prime Implicants: " << endl;
		for (int j = 0; j < prime_implicants.size(); j++)
		{
			cout << prime_implicants[j].imp;
			for (int i = 0; i < prime_implicants[j].minterms.size(); i++)
			{
				cout << "\t" << prime_implicants[j].minterms[i] << endl;
			}
		}

		//outputs the don't care values of each prime implicant
		cout << endl << "Don't Care terms covered by Prime Implicants : " << endl;
		for (int j = 0; j < prime_implicants.size(); j++)
		{
			cout<<prime_implicants[j].imp;
			for (int i = 0; i < prime_implicants[j].dontcares.size(); i++)
			{
				cout << "\t" << prime_implicants[j].dontcares[i]<< endl;
			}
		}
		cout << endl;


		cout << "Binary Representation:" << endl;
		//outputting binary representation of each prime implicant
		for (int i = 0; i < prime_implicants.size(); i++)
		{
			cout << prime_implicants[i].imp << endl;
		}

		//boolean expressions for Prime Implicants


		cout << "Prime Implicants: " << endl;
		//We explained what this function does
		GenerateBoolean(prime_implicants, no_of_variables);



		cout << endl;
		//We explained what this function does
		EPI(prime_implicants, function_minterms, no_of_variables);

	}
	else //if input is invalid
	{
		cout << "invalid input";
	}
}


void Extract(vector<int>& fucntion_minterms, vector<int>& fucntion_dontcares, int& no_of_variables, bool& valid_in) //account for there being no don't cares
{
	ifstream file;
	string temp, temp2; //temporary strings to store inputs
	bool new_line = false; //a way to mark the 2nd and 3rd lines
	set<int>terms; //a set to avoid duplicates
	file.open("input.txt"); //opens the file
	getline(file, temp); //gets the number of variables
	//checks if number of variables input is valid
	if (!CheckValidVarSize(temp, 20))
	{
		valid_in = false;
		return;
	}
	no_of_variables = stoi(temp); //assigns the number of variables
	while (!new_line && valid_in)
	{
		getline(file, temp, ','); //check for new line
		for (int i = 0; i < temp.length(); i++)
		{
			if (temp[i] == '\n') //splits the string into the last minterm and first don't care term as necessary
			{
				temp2 = temp.substr(i + 1, string::npos);
				temp = temp.substr(0, temp.length() - (temp.length() - i));
				new_line = true; //marks the beginning of the don't care line
			}

		}
		valid_in = CheckValidTerm(temp, no_of_variables, terms);
		if (valid_in)
		{
			fucntion_minterms.push_back(stoi(temp));
		}
	}
	//checks the first don't care term if the last minterm was valid
	if (valid_in)
	{
		valid_in = CheckValidTerm(temp2, no_of_variables, terms);
		if (valid_in)
		{
			fucntion_dontcares.push_back(stoi(temp2));
		}
	}

	//takes in all the don't care terms
	while (!file.eof() && valid_in)
	{
		getline(file, temp, ',');
		valid_in = CheckValidTerm(temp, no_of_variables, terms);
		if (valid_in)
		{
			fucntion_dontcares.push_back(stoi(temp));
		}
	}
	//checks if there are no don't cares intended to be present
	if (temp2 == "NA")
	{
		valid_in = true;
	}
	file.close(); //closes the file
}

bool CheckValidVarSize(string in, int no_of_variables)
{
	bool valid = true;
	valid = (CheckCharacter(in) && CheckRangeVarCount(in, 20));
	return valid;
}

bool CheckValidTerm(string in, int no_of_variables, set<int>& terms)
{
	bool valid = true;
	//marks maximum size of a term
	int size = (pow(2, no_of_variables) - 1);
	//checks for range, if the term is numeric, and checks to see if it is duplicated
	valid = (CheckCharacter(in) && CheckRangeTerm(in, size) && (terms.find(stoi(in)) == terms.end()));
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
	vector<vector<vector<implicant>>> col; //our implication table
	vector<vector<implicant>> chunk; //a 2D vector to insert into our first column
	vector<implicant> imp; //a vector of implicants to implement in our chunk vector
	vector<implicant> PrimeImplicantsList; //a vector to store all our prime implicants
	vector<implicant> Has_it_all = ImplicantsList(mint, dc); //the disorganised list with all implicants from the first column

	int num_of_chunks = NumChunks(Has_it_all); //number of chunks in the first column
	chunk.resize(num_of_chunks + 1); //resizing the chunks in the first column to fit the necessary elements through direct access

	//assigning each implicant in the first column to its appropriate group based on the number of 1s
	for (int i = 0; i < Has_it_all.size(); i++)
	{
		chunk[Has_it_all[i].chunk].push_back(Has_it_all[i]);

	}
	col.push_back(chunk); //Now our first column is perfectly ready

	int colsize = col.size(); //we had column size as a separate integer as we doubted it had to do with a bug we came across
	for (int t = 0; t < colsize; t++)     //looping in each column
	{
		for (int i = 0; i < col[t].size(); i++)       //loops on each chunk
		{
			if (col[t][i].size() > 0) //checks if there are any elements with this number of 1s
			{
				for (int j = 0; j < col[t][i].size(); j++)  //Loops on each implicant in chunk
				{
					if (i + 1 < col[t].size()) //checks if there is a next chunk
					{
						for (int l = 0; l < col[t][i + 1].size(); l++) //loops in the chunk after the element we are checking
						{

							vector<int> p = col[t][i][j].delta(col[t][i + 1][l].imp); //this vector marks the locations where there are differences between the 2 implicants
							if (p.size() == 1) //if its size is 1 then they are logically adjacent and can be merged
							{
								implicant merged(col[t][i][j], col[t][i + 1][l], p); //a temporary implicant to insert into the table
								//a check to see if there are enough columns for the insertion
								if (col.size() < t + 2)
								{
									col.resize(col.size() + 1); //creates an extra column
									colsize = col.size(); //modifies colsize so the loop continues
								}
								if (merged.chunk >= col[t + 1].size()) //checks if there are enough chinks to insert the implicant
								{
									col[t + 1].resize(merged.chunk + 1); //creates the needed chunks if there are not enough
								}
								col[t + 1][merged.chunk].push_back(merged); //inserts the new implicant
								//mark its 2 component implicants as having been merged (i.e. not prime implicants)
								col[t][i][j].merged = true;
								col[t][i + 1][l].merged = true;
							}
						}
						if (!col[t][i][j].merged) //Checks if the implicant was merged with other implicants
						{
							PrimeImplicantsList.push_back(col[t][i][j]); //adds the prime implicant to the vector
						}
					}


				}
			}



		}

	}

	//adds all the elements from the last chunk of the last column to the prime implicants list
	for (int i = 0; i < col[col.size() - 1][col[col.size() - 1].size() - 1].size(); i++)
	{
		if (!col[col.size() - 1][col[col.size() - 1].size() - 1][i].merged) //Checks if the implicant was merged with other implicants
		{
			PrimeImplicantsList.push_back(col[col.size() - 1][col[col.size() - 1].size() - 1][i]); //adds the prime implicant to the vector
		}
		
	}




	//returns the list of prime implicants
	return PrimeImplicantsList;
}

int NumChunks(vector<implicant> terms)
{
	int m = -1; //an arbitrary maximum value that is guaranteed to be replaced by something
	int temp; //a temporary value for comparison
	//loops over all elements in the first column to find the maximum number of 1s
	for (int i = 0; i < terms.size() - 1; i++)
	{
		for (int j = i; j < terms.size(); j++)
		{
			temp = max(terms[i].chunk, terms[j].chunk);
			if (temp > m)
			{
				m = temp;
			}
		}
	}
	return m;
}

vector<implicant> ImplicantsList(vector<int> minterms, vector<int> dont_cares)
{
	vector<implicant> j;
	for (int i = 0; i < minterms.size(); i++)
	{
		j.push_back(implicant(minterms[i], true));
	}
	for (int i = 0; i < dont_cares.size(); i++)
	{
		j.push_back(implicant(dont_cares[i], false));
	}
	return j;
}


vector<implicant> RemoveDup(vector<implicant> p, int no_of_var)
{
	//loops over all prime implicants
	for (int i = 0; i < p.size(); i++)
	{
		//loops over all prime implicants
		for (int j = 0; j < p.size(); j++)
		{
			//if the prime implicants haave the same binary representation but aren't the exact same implicant, eliminate the duplicate
			if (p[i].imp == p[j].imp && i != j)
			{
				set<int> temp; //used to avoid duplication
				vector<int> final_terms; //used to mark the final form of terms for the unduplicated implicant
				vector<int> final_minterms; //used to mark the final form of minterms for the unduplicated implicant
				vector<int> final_dontcares; //used to mark the final form of don't care terms for the unduplicated implicant

				//loops over all terms and puts the ones from the first instance in a set
				for (int k = 0; k < p[i].terms.size(); k++)
				{
					temp.insert(p[i].terms[k]);
				}
				//loops over all terms and puts the different ones from the second instance in final_terms
				for (int k = 0; k < p[j].terms.size(); k++)
				{
					if ((temp).find(p[j].terms[k]) == temp.end())
					{
						final_terms.push_back(p[j].terms[k]);
					}
				}
				//clears the set for the next use
				temp.clear();

				//loops over all minterms and puts the ones from the first instance in a set
				for (int k = 0; k < p[i].minterms.size(); k++)
				{
					temp.insert(p[i].minterms[k]);
				}
				//loops over all minterms and puts the different ones from the second instance in final_minterms
				for (int k = 0; k < p[j].minterms.size(); k++)
				{
					if ((temp).find(p[j].minterms[k]) == temp.end())
					{
						final_minterms.push_back(p[j].minterms[k]);
					}
				}
				//clears the set for the next use
				temp.clear();

				//loops over all don't care terms and puts the ones from the first instance in a set
				for (int k = 0; k < p[i].dontcares.size(); k++)
				{
					temp.insert(p[i].dontcares[k]);
				}
				//loops over all don't care terms and puts the different ones from the second instance in final_dontcares
				for (int k = 0; k < p[i].dontcares.size(); k++)
				{
					if ((temp).find(p[j].dontcares[k]) == temp.end())
					{
						final_dontcares.push_back(p[j].dontcares[k]);
					}
				}
				//clears the set for good measure
				temp.clear();
				//inserts the different terms, minterms, and don't care terms into the first instance of the implicant
				p[i].terms.insert(p[i].terms.begin(), final_terms.begin(), final_terms.end());
				p[i].minterms.insert(p[i].minterms.begin(), final_minterms.begin(), final_minterms.end());
				p[i].dontcares.insert(p[i].dontcares.begin(), final_dontcares.begin(), final_dontcares.end());
				//removes the duplicate
				p.erase(p.begin() + j);

				//clearing the vectors for use with other implicants in the loop
				final_terms.clear();
				final_minterms.clear();
				final_dontcares.clear();
			}
		}
	}


	p = RemoveZeros(p, no_of_var);

	return p;
}


vector<implicant> RemoveZeros(vector<implicant> p, int no_of_var)
{
	int n = 0;
	//checks if the number of variables is in range
	if (no_of_var >= 0 && no_of_var < 20)
	{

		n = no_of_var;
		//removes as many leading 0s as necessary
		for (int j = 0; j < p.size(); j++)
		{
			p[j].imp.erase(p[j].imp.begin(), p[j].imp.begin() + (20 - n));
		}
	}
	//a check in case the number of variables is greater than 20
	else if (no_of_var > 20)
	{
		cout << "Invalid num of inputs" << endl;
		return p;
	}

	return p;
}


void GenerateBoolean(vector<implicant> PIs, int no_of_variables)
{
	//loops over the vector of implicants
	for (int j = 0; j < PIs.size(); j++)
	{
		//Outputs the boolean expression accordingly where dashes are ignored, 0s are XN' and 1s are XN where N is the bit number
		for (int i = 0; i < no_of_variables; i++)
		{
			if (PIs[j].imp[i] == '0')
			{
				cout << "X" << i << "'";
			}
			else if (PIs[j].imp[i] == '1')
			{
				cout << "X" << i;
			}
			else if (PIs[j].imp[i] == '-')
			{
				cout << "";
			}
		}
		//a formatting thing to add a comma after each implicant if it isn't the last one
		if (j < PIs.size() - 1)
			cout << " ," << " ";
	}
}


vector<implicant> CleanUp(vector<implicant> prime_implicants)
{
	set<int> minterms;
	vector<int> final_minterms;
	for (int i = 0; i < prime_implicants.size(); i++)
	{
		for (int j = 0; j < prime_implicants[i].minterms.size(); j++)
		{
			if ((minterms).find(prime_implicants[i].minterms[j]) == minterms.end())
			{
				minterms.insert(prime_implicants[i].minterms[j]);
				final_minterms.push_back(prime_implicants[i].minterms[j]);
			}
		}
		prime_implicants[i].minterms = final_minterms;
		final_minterms.clear();
		minterms.clear();
	}

	return prime_implicants;
}


void EPI(vector <implicant> prime_implicants, vector<int> function_minterms, int no_of_variables)
{
	map<int, vector<implicant>> EPI_table; //the coverage chart
	set<int> covered_by_EPIs; //allows for marking the minterms covered by EPIs while avoiding duplicates
	vector<int> not_covered_by_EPIs;
	vector<implicant> blank; //just a blank vector for initializing the chart
	vector<implicant> EPI; //A vector to store the EPIs


	//initialising our map
	for (int i = 0; i < function_minterms.size(); i++)
	{
		EPI_table.insert({ function_minterms[i],blank });
	}

	//putting all implicants in the vector of the minterm they cover
	for (int i = 0; i < prime_implicants.size(); i++)
	{
		for (int j = 0; j < prime_implicants[i].minterms.size(); j++)
		{
			map<int, vector<implicant>>::iterator it2 = EPI_table.find(prime_implicants[i].minterms[j]);
			(it2->second).push_back(prime_implicants[i]);
		}
	}

	//finding EPIs and implicants covered by EPIs
	//loop over the map
	map<int, vector<implicant>>::iterator it = EPI_table.begin();
	for (it; it != EPI_table.end(); it++)
	{
		if ((it->second).size() == 1) //if the vector only has one element, then it is an EPI
		{
			implicant temp = it->second[0];
			for (int i = 0; i < temp.minterms.size(); i++)
			{

				covered_by_EPIs.insert(temp.minterms[i]);
			}
			EPI.push_back(temp);
		}
	}
	//finding implicants not covered by EPIs
	for (int i = 0; i < function_minterms.size(); i++)
	{
		//if the minterm is not found in covered_by_EPIs then it is not covered by EPIs
		if ((covered_by_EPIs).find(function_minterms[i]) == covered_by_EPIs.end())
		{
			not_covered_by_EPIs.push_back(function_minterms[i]);
		}
	}
	cout << "Essential Prime Implicants: " << endl;

	//output EPI
	GenerateBoolean(EPI, no_of_variables);

	//output minterms not covered by EPIs
	cout << endl;
	cout << "Minterms not covered by EPIs: \n";
	for (int i=0; i<not_covered_by_EPIs.size(); i++)
	{
		cout << not_covered_by_EPIs[i] << " ";
	}
}

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