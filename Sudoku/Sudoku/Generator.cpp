/*
 * Generator.cpp
 *
 *  Created on: Sep 17, 2015
 *      Author: Dimitar Gueorguiev
 */
#include <algorithm>
#include <stdlib.h>     /* srand, rand */
#include <ctime>       /* time */

#include "Generator.h"
#include "Sudoku.h"

using namespace std;

namespace sudoku
{
	int rand_func(int idx) { return std::rand() % idx; } // random generator

	void RGenerator::fillRandom(vector<unsigned char> & buffer, int n)
	{
		

		std::srand(unsigned(std::time(0)));
		
		vector<int> arr;

		// set some values:
		unsigned short dimSquare = n;
		for (unsigned short i = 0; i < n; ++i) 
			arr.push_back(i % m_iDim); 

		// using built-in random generator:
		random_shuffle(arr.begin(), arr.end());

		// using rand()
		random_shuffle(arr.begin(), arr.end(), rand_func);
		for (unsigned short i = 0; i < n; i++)
		{		
		   buffer.push_back(Parser::symbolTable[arr[i]]);
		}

	}

#ifdef _DEBUG
	void RGenerator::print_row(vector<unsigned char> & v)
	{
		cout << endl;
		for (unsigned int i = 0; i < v.size(); i++)
		{
			cout << v[i] << " ";
			if (i > 0 && (i+1) % m_iDim == 0)
				cout << endl;
		}
		cout << endl;

	}
#endif

	bool RGenerator::generate()
	{
		bool res = true;

		vector<unsigned char> buffer;
		int n = 1000;
		fillRandom(buffer, n);
		
#ifdef _DEBUG
		print_row(buffer);
#endif

		m_pSrc->load(buffer);
	
		

		return res;
	}


}


