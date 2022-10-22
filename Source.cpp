#include <iostream>
#include <vector>
#include "implicant.h"
#include <fstream>
#include <cctype>
#include<math.h>
#include <set>
#include <map>

//#include "InputLib.h"
using namespace std;
void Extract(vector<int>& fucntion_minterms, vector<int>& fucntion_dontcares, int& no_of_variables, bool& valid_in);
bool CheckValidVarSize(string in, int no_of_variables);
bool CheckValidTerm(string in, int no_of_variables, set<int>& terms);
bool CheckRangeVarCount(string in, int size);
bool CheckRangeTerm(string in, int size);
bool CheckCharacter(string in);
vector<implicant> PrimeImp(vector<int> mint, vector<int> dc, int num_of_var);
vector<implicant> ImplicantsList(vector<int> minterms, vector<int> dont_cares);
int NumChunks(vector<implicant> terms);
//vector<string> toString(vector<implicant> m);
vector<implicant> RemoveDup(vector<implicant> p, int no_of_var);
vector<implicant> RemoveZeros(vector<implicant> p, int no_of_var);
void GenerateBoolean(vector<implicant> PIs, int no_of_variables);
vector<implicant> CleanUp(vector<implicant> prime_implicants);
void EPI(vector <implicant> prime_implicants, vector<int> function_minterms, int no_of_variables);


int main()
{
	vector<int> function_minterms;
	vector<int> function_dontcares;
	vector <implicant> prime_implicants;
	int no_of_variables = -1;
	int w = 0;
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

		prime_implicants = PrimeImp(function_minterms, function_dontcares, no_of_variables);

		cout << endl;

		//for (int i = 0; i < prime_implicants.size(); i++)
		//	cout << prime_implicants[i].imp << endl;

		//cout << endl << endl;

		//vector<implicant> second = RemoveDup(prime_implicants);

		prime_implicants = RemoveDup(prime_implicants, no_of_variables);
		//	vector<implicant> third = RemoveZeros(second, no_of_variables);
	//	prime_implicants = RemoveZeros(prime_implicants, no_of_variables);


		prime_implicants = CleanUp(prime_implicants);

		cout << "Minterms covered by Prime Implicants: " << endl;
		for (int j = 0; j < prime_implicants.size(); j++)
		{
			for (int i = 0; i < prime_implicants[j].minterms.size(); i++)
			{
				cout << prime_implicants[j].minterms[i] << "\t" << prime_implicants[j].imp << endl;
			}
		}

		cout << endl << "Don't Care terms covered by Prime Implicants : " << endl;
		for (int j = 0; j < prime_implicants.size(); j++)
		{
			for (int i = 0; i < prime_implicants[j].dontcares.size(); i++)
			{
				cout << prime_implicants[j].dontcares[i] << "\t" << prime_implicants[j].imp << endl;
			}
		}
		cout << endl;


		cout << "Binary Representation:" << endl;

		for (int i = 0; i < prime_implicants.size(); i++)
		{
			cout << prime_implicants[i].imp << endl;
		}

		//boolean expressions for Prime Implicants


		cout << "Prime Implicants: " << endl;
		GenerateBoolean(prime_implicants, no_of_variables);

		//cout << endl <<"Don't care terms covered by Prime Implicants:" << endl;

		cout << endl;

		EPI(prime_implicants, function_minterms, no_of_variables);

	}
	else
	{
		cout << "invalid input";
	}
}

void Extract(vector<int>& fucntion_minterms, vector<int>& fucntion_dontcares, int& no_of_variables, bool& valid_in) //account for there being no don't cares
{
	ifstream file;
	string temp, temp2;
	char new_line_trash;
	bool new_line = false;
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
				temp2 = temp.substr(i + 1, string::npos);
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

	if (temp2 == "NA")
	{
		valid_in = true;
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
	vector<vector<vector<implicant>>> col;
	vector<vector<implicant>> chunk;
	vector<implicant> imp;
	vector<implicant> PrimeImplicantsList;
	vector<implicant> Has_it_all = ImplicantsList(mint, dc);
	vector<int> forMerge;
	int w = 0;
	forMerge.resize(Has_it_all.size());

	int num_of_chunks = NumChunks(Has_it_all);
	chunk.resize(num_of_chunks + 1);


	for (int i = 0; i < Has_it_all.size(); i++)
	{
		chunk[Has_it_all[i].chunk].push_back(Has_it_all[i]);
		//col.push_back(chunk); I'll move this out of the loop
	}
	col.push_back(chunk);
	//col.resize(col.size() + 1);
	int colsize = col.size();
	for (int t = 0; t < colsize; t++)     //chunks loop //looping on each chunk (MAGD: ayo hold up, shouldn't we be looping column->chunk->value ?)
	{
		for (int i = 0; i < col[t].size(); i++)       //loops on each implicant in chunk one //2
		{
			if (col[t][i].size() > 0)
			{
				for (int j = 0; j < col[t][i].size(); j++)  //Loops on each implicant in chunk two //3
				{
					if (i + 1 < col[t].size())
					{
						for (int l = 0; l < col[t][i + 1].size(); l++)
						{

							vector<int> p = col[t][i][j].delta(col[t][i + 1][l].imp);
							if (p.size() == 1)
							{
								implicant merged(col[t][i][j], col[t][i + 1][l], p);
								if (col.size() < t + 2)
								{
									col.resize(col.size() + 1);
									colsize = col.size();
								}
								if (merged.chunk >= col[t + 1].size())
								{
									col[t + 1].resize(merged.chunk + 1);
								}
								col[t + 1][merged.chunk].push_back(merged);
								col[t][i][j].merged = true;
								col[t][i + 1][l].merged = true;
							}
						}
						if (!col[t][i][j].merged)
						{
							PrimeImplicantsList.push_back(col[t][i][j]);
						}
					}


				}
			}


			//if (Has_it_all[i].merged)                                 //i merged, put into chunk, then new col
			//{
			//	chunk[Has_it_all[i].chunk].push_back(Has_it_all[i]);
			//	col.push_back(chunk);
			//	w++;
			//}
			//else                                                   //add to prime implicant list
			//{
			//	PrimeImplicantsList.push_back(Has_it_all[i]);
			//}
		}
		/*col.resize(col.size() + 1);*/
	}

	for (int i = 0; i < col[col.size() - 1][col[col.size() - 1].size() - 1].size(); i++)
	{
		PrimeImplicantsList.push_back(col[col.size() - 1][col[col.size() - 1].size() - 1][i]);
	}


	//cout << "Minterms covered by Prime Implicants:" << endl;



	return PrimeImplicantsList;
}
int NumChunks(vector<implicant> terms)
{
	int m = -1;
	int temp;
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

	for (int i = 0; i < p.size(); i++)
	{
		for (int j = 0; j < p.size(); j++)
		{
			if (p[i].imp == p[j].imp && i != j)
			{
				set<int> temp;
				vector<int> final_terms;
				vector<int> final_minterms;
				vector<int> final_dontcares;
				for (int k = 0; k < p[i].terms.size(); k++)
				{
					if ((temp).find(p[i].terms[k]) == temp.end())
					{
						temp.insert(p[i].terms[k]);
						final_terms.push_back(p[i].terms[k]);
					}
				}

				temp.clear();

				for (int k = 0; k < p[i].minterms.size(); k++)
				{
					if ((temp).find(p[i].minterms[k]) == temp.end())
					{
						temp.insert(p[i].minterms[k]);
						final_minterms.push_back(p[i].minterms[k]);
					}
				}

				temp.clear();

				for (int k = 0; k < p[i].dontcares.size(); k++)
				{
					if ((temp).find(p[i].dontcares[k]) == temp.end())
					{
						temp.insert(p[i].dontcares[k]);
						final_dontcares.push_back(p[i].dontcares[k]);
					}
				}

				temp.clear();

				p[i].terms.insert(p[i].terms.begin(), final_terms.begin(), final_terms.end());
				p[i].minterms.insert(p[i].minterms.begin(), final_minterms.begin(), final_minterms.end());
				p[i].dontcares.insert(p[i].dontcares.begin(), final_dontcares.begin(), final_dontcares.end());
				p.erase(p.begin() + j);

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

	if (no_of_var >= 0 && no_of_var < 20)
	{

		n = no_of_var;
		for (int j = 0; j < p.size(); j++)
		{
			p[j].imp.erase(p[j].imp.begin(), p[j].imp.begin() + (20 - n));
		}
	}
	else if (no_of_var > 20)
	{
		cout << "Invalid num of inputs" << endl;
		return p;
	}

	return p;
}

void GenerateBoolean(vector<implicant> PIs, int no_of_variables)
{
	for (int j = 0; j < PIs.size(); j++)
	{
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
	map<int, vector<implicant>> EPI_table;
	set<int> covered_by_EPIs;
	vector<int> not_covered_by_EPIs;
	vector<implicant> blank;
	vector<implicant> EPI;
	vector<implicant> temp;

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

	//finding EPIs
	map<int, vector<implicant>>::iterator it = EPI_table.begin();
	for (it; it != EPI_table.end(); it++)
	{
		if ((it->second).size() == 1)
		{
			implicant temp = it->second[0];
			for (int i = 0; i < temp.minterms.size(); i++)
			{
				//map<int, vector<implicant>>::iterator it2 = EPI_table[temp.minterms[i]];
				//EPI_table.erase(temp.minterms[i]);
				covered_by_EPIs.insert(temp.minterms[i]);
			}
			EPI.push_back(temp);
		}
	}
	for (int i = 0; i < function_minterms.size(); i++)
	{
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