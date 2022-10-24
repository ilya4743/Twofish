#pragma once

#include "def.h"
#include <iostream>
#include <boost/qvm/mat.hpp>
#include <cmath>
#include <vector>

using namespace std;

class Twofish
{
private:
	vector<uint32_t> Me;
	vector<uint32_t> Mo;
	vector<uint32_t> V;
	vector<vector<uint8_t>> me;
	vector<vector<uint8_t>> mo;
	vector<vector<uint8_t>> v;
	vector<uint32_t> sk;
	int k;

	uint8_t q1(uint8_t x);
	uint8_t q0(uint8_t x);
	uint32_t RS_MDS_Encode(uint32_t k0, uint32_t k1);
	vector<uint32_t> ParseHexDword(int bits, char* srcTxt);
	uint32_t h(uint32_t x, const vector<vector<uint8_t>>& l);
public:
	void setKey();	
	void keySchedule(uint8_t* key, uint32_t k);

	void encrypt(const uint8_t* in, uint8_t* out);
	void decrypt(const uint8_t* in, uint8_t* out);
	void printSubkeys();
};