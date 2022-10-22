#include "implicant.h"

implicant::implicant(int n, bool minterm)
{
	chunk = 0;
	imp = convert_to_binary(n, chunk); //prepares binary representation and chunk value
	terms.push_back(n); //adds term
	if (minterm) //checks if the term is a minterm or a don't care
	{
		minterms.push_back(n);
	}
	else
	{
		dontcares.push_back(n);
	}
	merged = false; //not merged out of the gate
}
implicant::implicant(implicant i1, implicant i2, vector <int> reploc)
{
	chunk = 0;
	imp = merge(i1.imp, i2.imp, reploc, chunk); //prepares binary representation and chunk value
	i1.merged = true; //marks components as merged
	i2.merged = true; //marks components as merged
	//merges terms, minterms and don't cares
	terms = i2.terms;
	minterms = i2.minterms;
	dontcares = i2.dontcares;
	terms.insert(terms.begin(), i1.terms.begin(), i1.terms.end());
	minterms.insert(minterms.begin(), i1.minterms.begin(), i1.minterms.end());
	dontcares.insert(dontcares.begin(), i1.dontcares.begin(), i1.dontcares.end());
	
	merged = false; //not merged out of the gate
}
string implicant::convert_to_binary(int n, int& chunk)
{
	string final_binary="00000000000000000000"; //a blank slate with a bit size of 20
	string binar = ""; //contains the binary form of the term
	int p = 0; //number of bits
	int temp;
	int temp2; //number of bits but we'll use them for writing the binary number
	int number_of_1 = 0; //self-explanatory
	if (n!= 0) //if the term is not 0
	{
		while (pow(2, p) <= n) //while the number of bits is insufficient to represent the number, increase the number of bits
		{
			p++;
		}
		temp2 = p; //made this for traversal
		while (temp2 > 0) //while we're not done creating the binary representation
		{
			temp = (n % int(pow(2, temp2 - 1))); //the remainder of the term and the bit representation
			//if nothing happens then it's a 0 if the number changes, it's a 1
			if (temp == n)
			{
				binar += "0";
			}
			else
			{
				binar += "1";
				n -= pow(2, temp2 - 1); //decrease the number to continue work
			}
			temp2--; //to traverse to a smaller bit
		}
		final_binary.replace(final_binary.length() - p, binar.length(), binar); //replace some of the 0s with the binary representation of the term
		for (int i = 0; i < binar.length(); i++) //counting the number of 1s
		{
			if (binar[i] == '1')
			{
				number_of_1++;
			}
		}
	}
	chunk = number_of_1; //assigning the chunk value
	return final_binary;
}
string implicant::merge(string i1, string i2, vector<int> reploc, int& chunk)
{
	string merged = i1; //the base is the first implicant's binary representation
	int number_of_1 = 0; //a temporary value
	merged.replace(reploc[0], 1, "-"); //replaces the character where there is a difference by a -
	//counts the number of 1s
	for (int i = 0; i < merged.length(); i++)
	{
		if (merged[i] == '1')
		{
			number_of_1++;
		}
	}
	chunk = number_of_1; //assigns the number of 1s
	return merged;
}

vector<int> implicant::delta(string i2)
{
	vector <int> reploc; //reploc stands for replace location
	string new_imp = this->imp; //baseline for comparison
	for (int i = 0; i < this->imp.length(); i++)
	{
		//if it finds a difference, push its location to the vector
		if (this->imp[i] != i2[i])
		{
			reploc.push_back(i);
		}
	}
	return reploc;
}