#include "implicant.h"

implicant::implicant(int n, bool minterm)
{
	chunk = 0;
	imp = convert_to_binary(n, chunk);
	terms.push_back(n);
	if (minterm)
	{
		minterms.push_back(n);
	}
	else
	{
		dontcares.push_back(n);
	}
	merged = false;
}
implicant::implicant(implicant i1, implicant i2, vector <int> reploc) //will define how to compare in source
{
	chunk = 0; //create getters
	imp = merge(i1.imp, i2.imp, reploc, chunk);
	i1.merged = true;
	i2.merged = true;
	terms = i2.terms;
	minterms = i2.minterms;
	dontcares = i2.dontcares;
	terms.insert(terms.begin(), i1.terms.begin(), i1.terms.end());
	minterms.insert(minterms.begin(), i1.minterms.begin(), i1.minterms.end());
	dontcares.insert(dontcares.begin(), i1.dontcares.begin(), i1.dontcares.end());
	
	merged = false;
}
string implicant::convert_to_binary(int n, int& chunk)
{
	string final_binary="00000000000000000000";
	string binar = "";
	int p = 0;
	int temp;
	int temp2;
	int number_of_1 = 0;
	if (n!= 0)
	{
		while (pow(2, p) <= n)
		{
			p++;
		}
		temp2 = p;
		while (temp2 > 0)
		{
			temp = (n % int(pow(2, temp2 - 1)));
			if (temp == n)
			{
				binar += "0";
			}
			else
			{
				binar += "1";
				n -= pow(2, temp2 - 1);
			}
			temp2--;
		}
		final_binary.replace(final_binary.length() - p, binar.length(), binar);
		for (int i = 0; i < binar.length(); i++)
		{
			if (binar[i] == '1')
			{
				number_of_1++;
			}
		}
	}
	chunk = number_of_1;
	return final_binary;
}
string implicant::merge(string i1, string i2, vector<int> reploc, int& chunk)
{
	string merged = i1;
	int number_of_1 = 0;
	merged.replace(reploc[0], 1, "-");
	for (int i = 0; i < merged.length(); i++)
	{
		if (merged[i] == '1')
		{
			number_of_1++;
		}
	}
	chunk = number_of_1;
	return merged;
}

vector<int> implicant::delta(string i2)
{
	vector <int> reploc; //reploc stands for replace location
	string new_imp = this->imp;
	for (int i = 0; i < this->imp.length(); i++)
	{
		if (this->imp[i] != i2[i])
		{
			reploc.push_back(i);
		}
	}
	return reploc;
}