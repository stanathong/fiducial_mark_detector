/*
 * ***** FiducialMarkProject *****
 *
 * Author:		Supannee Tanathong
 * Date:        May 2010
 *
 */

#ifndef __RREF_H__
#define __RREF_H__

#pragma once

#include "Stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

// This class is to calculate the reduced-row-echelon.
// Assumption:
// 1. The matrix does not contain rows with all zeroes.
// 2. The matrix does not contain columns with all zeros.

class CRREF
{
public:
	CRREF();
	~CRREF();

	// Return 'true' if success to find rref, otherwise return 'false'
	bool rref(float ** matrix, int nrow, int ncol);


private:
	// Swap the data from row1 to row 2 and swapt the data from row 2 to row 1
	// Passed-in parameters:
	//	col		the number of columns in this matrix
	//	row1, row2	the row to be swapped
	void swap_row(float ** matrix, int col, int row1, int row2);

	// Check if the element in the specified row and col is zero.
	bool is_zero_element(float ** matrix, int row, int col);

	// Check if passed-in value is zero.
	bool is_zero_element(float fval);

	// Check if the passed-in column "c" contains all zeros.
	// parameter:
	//	row		the total number of row in the matrix.
	//	c		the column index to be checked
	bool is_all_zeros_column(float ** matrix, int row, int c);

	// Check if the passed-in row "r" contains all zeros.
	// parameter:
	//	col		the total number of col in the matrix.
	//	r		the row index to be checked
	bool is_all_zeros_row(float ** matrix, int col, int r);

};

#endif