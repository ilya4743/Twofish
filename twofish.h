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

	const uint32_t p = 16843009;

	const uint8_t Tq0[4][16] =
	{
		0x8, 0x1, 0x7, 0xD, 0x6, 0xF, 0x3, 0x2, 0x0, 0xB, 0x5, 0x9, 0xE, 0xC, 0xA, 0x4,
		0xE, 0XC, 0XB, 0X8, 0X1, 0X2, 0X3, 0X5, 0XF, 0X4, 0XA, 0X6, 0X7, 0X0, 0X9, 0XD,
		0XB, 0XA, 0X5, 0XE, 0X6, 0XD, 0X9, 0X0, 0XC, 0X8, 0XF, 0X3, 0X2, 0X4, 0X7, 0X1,
		0XD, 0X7, 0XF, 0X4, 0X1, 0X2, 0X6, 0XE, 0X9, 0XB, 0X3, 0X0, 0X8, 0X5, 0XC, 0XA
	};

	const uint8_t Tq1[4][16] =
	{
		0X2, 0X8, 0XB, 0XD, 0XF, 0X7, 0X6, 0XE, 0X3, 0X1, 0X9, 0X4, 0X0, 0XA, 0XC, 0X5,
		0X1, 0XE, 0X2, 0XB, 0X4, 0XC, 0X3, 0X7, 0X6, 0XD, 0XA, 0X5, 0XF, 0X9, 0X0, 0X8,
		0X4, 0XC, 0X7, 0X5, 0X1, 0X6, 0X9, 0XA, 0X0, 0XE, 0XD, 0X8, 0X2, 0XB, 0X3, 0XF,
		0xB, 0X9, 0X5, 0X1, 0XC, 0X3, 0XD, 0XE, 0X6, 0X4, 0X7, 0XF, 0X2, 0X0, 0X8, 0XA
	};

	vector<uint32_t> Me;
	vector<uint32_t> Mo;
	vector<uint32_t> V;
	vector<vector<uint8_t>> me;
	vector<vector<uint8_t>> mo;
	vector<vector<uint8_t>> v;
	vector<uint32_t> sk;
	uint32_t k;

	uint8_t q1(uint8_t x);
	uint8_t q0(uint8_t x);
	uint32_t RS_MDS_Encode(uint32_t k0, uint32_t k1);
	inline vector<uint32_t> ParseHexDword(int bits, vector<uint8_t>&& srcTxt);
	uint32_t h(uint32_t x, const vector<vector<uint8_t>>& l);
public:
	void setKey();	
	void keySchedule(vector<uint8_t>&& key);

	vector<uint8_t> encrypt(vector<uint8_t>&& in);
	vector<uint8_t> decrypt(vector<uint8_t>&& in);
	void printSubkeys();
};