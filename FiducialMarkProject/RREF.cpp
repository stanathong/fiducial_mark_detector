/*
 * ***** FiducialMarkProject *****
 *
 * Author:		Supannee Tanathong
 * Date:        May 2010
 *
 */

#include "RREF.h"

// Swap the data from row1 to row 2 and swapt the data from row 2 to row 1
// Passed-in parameters:
//	col		the number of columns in this matrix
//	row1, row2	the row to be swapped
void CRREF::swap_row(float ** matrix, int col, int row1, int row2)
{
	int c;

	// Create tempolary buffer.
	float * row2_buf = reinterpret_cast<float *> (malloc(col * sizeof(float)));

	// Backup the value of row 2
	for (c = 0; c < col; c++)
		row2_buf[c] = matrix[row2][c];

	// Copy the data from row 1 to row 2
	for (c = 0; c < col; c++)
	{
		matrix[row2][c] = matrix[row1][c];
		matrix[row1][c] = row2_buf[c];
	}

	if (row2_buf != NULL)
		free(row2_buf);
}

// Check if the element in the specified row and col is zero.
bool CRREF::is_zero_element(float ** matrix, int row, int col)
{
	return DBL_EQ(matrix[row][col],0.0);
}

// Check if passed-in value is zero.
bool CRREF::is_zero_element(float fval)
{
	return DBL_EQ(fval,0.0);
}

// Check if the passed-in column "c" contains all zeros.
// parameter:
//	row		the total number of row in the matrix.
//	c		the column index to be checked
bool CRREF::is_all_zeros_column(float ** matrix, int row, int c)
{
	for (int r = 0; r < row; r++)
	{
		if (!is_zero_element(matrix[r][c]))
			return false;
	}

	return true;
}

// Check if the passed-in row "r" contains all zeros.
// parameter:
//	col		the total number of col in the matrix.
//	r		the row index to be checked
bool CRREF::is_all_zeros_row(float ** matrix, int col, int r)
{
	for (int c = 0; c < col; c++)
	{
		if (!is_zero_element(matrix[r][c]))
			return false;
	}

	return true;
}

// Return 'true' if success to find rref, otherwise return 'false'
bool CRREF::rref(float ** matrix, int nrow, int ncol)
{
	int r, c, rr;
	int * pivot_pos;
	int num_pivot;			// To store the number of pivot.
	bool bRemainingZeros;	// If the remaining elements in the column are all zeros.
	int r_first_nonzero;	// The first row that it is non zero in that column.

	int min_dim = min(nrow, ncol);	// The min dimension between row and column.

	// Allocate buffer to store pivot position (column) of each row.
	// pivot_pos[0] = c? <-- the pivot position at row 0 is at column c?
	// pivot_pos[1] = c? <-- the pivot position at row 1 is at column c?
	pivot_pos = reinterpret_cast<int *>(malloc(min_dim/*row*/*sizeof(int)));
	memset(pivot_pos, 0, /*row*/min_dim*sizeof(int));

	// Find the pivot position in each row.
	for (r = 0; r < min_dim/*nrow*/; r++)
	{
		// Initially define pivot position, it will be reset if false.
		pivot_pos[r] = r;

		for (c = r; c < ncol; c++)
		{
			// if the element (r,c) is 0, swap it with the next row.
			r_first_nonzero = r;
			bRemainingZeros = false;
			
			while (is_zero_element(matrix[r_first_nonzero][c]))
			{
				r_first_nonzero++;
				if (r_first_nonzero >= nrow)
				{
					bRemainingZeros =  true;
					break;
				}
			}
			
			// If the remainings are all zeroes, go to the next column.
			if (bRemainingZeros == true)
				continue; // continue next column
			else
			{
				// If this row is zero, swap it with the first row with non zero.
				if (r != r_first_nonzero)
					swap_row(matrix, ncol, r, r_first_nonzero);
			
				// Store pivot position
				pivot_pos[r] = c;
				break;	// go to next row
			}
		} // end for column
	} // end for row

	// At this point, we know the position of pivot.

	// Make the value at pivot position equal to 1.

	int n_lastrow = nrow-1;
	bool bEnd = false;

	for (r = 0; r <= /*min_dim*/n_lastrow; r++)
	{
		if (bEnd == true)
			break;

		// If the processing row is all zeros, swap it to the end.
		if (is_all_zeros_row(matrix, ncol, r))
		{
			if (r >= n_lastrow)
			{
				bEnd = true;
				break;
			}

			swap_row(matrix, ncol, 0, n_lastrow);
			n_lastrow--;
		}

		// Get the pivot position of row r
		int pivot_col = pivot_pos[r];
		float fpivot = matrix[r][pivot_col];
		
		// Divide elements of each row with the value of pivot for each row
		for (c = 0; c < ncol; c++)
		{
			matrix[r][c] = matrix[r][c]/fpivot;
		}

		// Make the element at the same column as pivot to zero
		for (rr = r+1; rr <= /*min_dim*/n_lastrow; rr++)
		{
			// If the processing row is all zeros, swap it to the end.
			if (is_all_zeros_row(matrix, ncol, rr))
			{
				if (rr >= n_lastrow)
				{
					bEnd = true;
					break;
				}

				swap_row(matrix, ncol, 0, n_lastrow);
				n_lastrow--;
			}

			float fval_pivot_col = matrix[rr][pivot_col];
			for (c = 0; c < ncol; c++)
			{	// The element at the same column as pivot will be equal to zero.
				matrix[rr][c] = matrix[r][c] - matrix[rr][c]/fval_pivot_col;
			}

		}

	}

	// At this point, we obtained "row-echelon form".


	// Therefore, we need to produce "reduced row-echelon form"
	num_pivot = min(n_lastrow, ncol);
	// The pivot position is where the col value is 1.
	for (r = 0; r < num_pivot; r++)
	{
		for (c = 0; c < ncol; c++)
		{
			// The element is equal to 1, get the pivot position.
			if 	( DBL_EQ(matrix[r][c],1.0) )		
			{
				pivot_pos[r] = c;
				break;
			}
		}
	}

	// Make the element at the same column as pivot to zero.
	// From row 0, to the specified row (diagonal up)
	// For each pivot, from last pivot
	for (p = num_pivot-1; p >= 0; p--)
	{
		int pivot_col = pivot_pos[p];

		r = p-1;
		while ( r >= 0 )
		{
			float fval_pivot_col = matrix[r][pivot_col];

			for (c = 0; c < ncol; c++)
			{	// The element at the same column as pivot will be equal to zero.
				matrix[r][c] = matrix[r][c] - matrix[p][c]*fval_pivot_col;
			}			

			r--;
		}
	}

	// Free memory
	if (pivot_pos != NULL)
		free(pivot_pos);

	return true;
}