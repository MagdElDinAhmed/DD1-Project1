#ifndef IMPLICANT
#define IMPLICANT
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
using namespace std;
class implicant
{
public:
	implicant(int n, bool minterm); //creates the first row implicants
	implicant(implicant i1, implicant i2, vector<int> reploc); //creates all merged implicants
	string convert_to_binary(int n, int& chunk); //converts minterms and don't cares into their binary representation
	string merge(string i1, string i2, vector<int> reploc, int& chunk); //creates the new binary representation for a merged implicant
	vector<int> delta(string i2); //generates a vector that contains the location of all differences between the binary representation of 2 implicants
	string imp; //binary representation of implicant
	vector<int> terms; //minterms and don't cares
	vector<int> minterms;
	vector<int> dontcares;
	int chunk; //number of 1s
	bool merged; //marker as to whether or not the implicant was merged with another implicant
};
#endif
