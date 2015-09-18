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

	class RGenerator : public Generator
	{

	public:
		bool generate();

	};

}

#endif /* GENERATOR_H_ */
