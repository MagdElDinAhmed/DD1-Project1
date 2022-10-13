#ifndef IMPLICANT
#define IMPLICANT
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
using namespace std;
class implicant
{
	implicant(int n);
	implicant(implicant i1, implicant i2, vector<int> reploc);
	string convert_to_binary(int n, int& chunk);
	string merge(string i1, string i2, vector<int> reploc); //I need to check that they have a delta of 1
	vector<int> delta(string i1, string i2);
	string imp;
	vector<int> minterms;
	int chunk;
};
#endif
