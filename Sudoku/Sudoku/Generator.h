/*
 * Generator.h
 *
 *  Created on: Sep 17, 2015
 *      Author: Dimitar Gueorguiev
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_


#include "Sudoku.h"


using namespace std;

namespace sudoku
{

	// this is the class which will generate Sudoku puzzle at random
	// which has unique solution
	class Generator
	{
	protected:	
		Puzzle * m_pSrc;
		Puzzle * m_pSol; // to obtain the solution using depth-first search

		unsigned char m_iDim;
		unsigned char m_iRegionDim;

		HorizLine ** m_pRows;
		VertLine ** m_pCols;
		Region ** m_pRegions;

		long long m_lError;

	public:
	
		Generator(unsigned char dim, unsigned char regionDim) : m_iDim(dim), m_iRegionDim(regionDim)
		{

		}
			

		virtual bool generate() = 0;


	};

	// generates the puzzle by filling rows in consecutive order and then opening holes on specific
	// places such that the puzzle matches the desired level of difficulty
	class RGenerator : public Generator
	{
	private:		
		Puzzle * m_pSrc;
		void fillRandom(vector<char> &, int);
	public:
	
		RGenerator(unsigned char dim, unsigned char regionDim) : Generator(dim, regionDim)
		{
			m_pSrc = new Puzzle(dim, regionDim);

		}

		~RGenerator()
		{
			delete m_pSrc;
		}

		bool generate();

#ifdef _DEBUG
		void print_row(vector<char> &);
		void init_test_buffer(vector<char> &);
#endif

	};

}

#endif /* GENERATOR_H_ */
