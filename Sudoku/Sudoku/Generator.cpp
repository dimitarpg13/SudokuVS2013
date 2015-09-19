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

	void RGenerator::fillRandom(vector<char> & buffer, int n)
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
	void RGenerator::print_row(vector<char> & v)
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

	// pseudo random input for testing purposes
	void RGenerator::init_test_buffer(vector<char> & v)
	{
		v.clear();
		v.push_back('5');
		v.push_back('6');
		v.push_back('5');
		v.push_back('6');
		v.push_back('2');
		v.push_back('9');
		v.push_back('4');
		v.push_back('3');
		v.push_back('1');

		v.push_back('5');
		v.push_back('9');
		v.push_back('2');
		v.push_back('4');
		v.push_back('9');
		v.push_back('7');
		v.push_back('5');
		v.push_back('6');
		v.push_back('4');

		v.push_back('3');
		v.push_back('5');
		v.push_back('3');
		v.push_back('7');
		v.push_back('3');
		v.push_back('4');
		v.push_back('1');
		v.push_back('2');
		v.push_back('8');

		v.push_back('7');
		v.push_back('1');
		v.push_back('6');
		v.push_back('8');
		v.push_back('3');
		v.push_back('8');
		v.push_back('4');
		v.push_back('6');
		v.push_back('5');

		v.push_back('3');
		v.push_back('8');
		v.push_back('1');
		v.push_back('4');
		v.push_back('7');
		v.push_back('9');
		v.push_back('7');
		v.push_back('6');
		v.push_back('9');

		v.push_back('4');
		v.push_back('2');
		v.push_back('4');
		v.push_back('2');
		v.push_back('2');
		v.push_back('5');
		v.push_back('4');
		v.push_back('6');
		v.push_back('2');

		v.push_back('2');
		v.push_back('3');
		v.push_back('7');
		v.push_back('7');
		v.push_back('6');
		v.push_back('2');
		v.push_back('5');
		v.push_back('1');
		v.push_back('1');

		v.push_back('8');
		v.push_back('8');
		v.push_back('3');
		v.push_back('9');
		v.push_back('5');
		v.push_back('6');
		v.push_back('1');
		v.push_back('8');
		v.push_back('8');

		v.push_back('2');
		v.push_back('3');
		v.push_back('5');
		v.push_back('4');
		v.push_back('1');
		v.push_back('3');
		v.push_back('7');
		v.push_back('6');
		v.push_back('3');



	}

#endif



	bool RGenerator::generate()
	{
		if (m_iDim != Puzzle::CLASSIC_SUDOKU_DIM || m_iRegionDim != Puzzle::CLASSIC_SUDOKU_REGION_DIM)
		{
			m_lError |= SUDOKU_ERROR_FEATURE_NOT_IMPLEMENTED;
			return false;
		}

		bool res = true;

		vector<char> buffer;
		int n = m_iDim * m_iDim;
		//fillRandom(buffer, n);
		init_test_buffer(buffer);
		
#ifdef _DEBUG
		print_row(buffer);
#endif

		m_pSrc->load(buffer);
	
		m_pSrc->printToConsole();



		return res;
	}


}


