/************************************************************
 * Project:		FiducialMarkProject
 * Objective:	To determine the Fiducial Mark positions based
 *				on image matching technique for automatic 
 *				interior orientation.
 * Developer:	Supannee Tanathong, LSM
 * Date:		May 2010
 ************************************************************/


/***********************************************************************
 * File Name:	Definition.h
 * Description: This file contains the struct or type definitions used
				in this program.
 ***********************************************************************/

#pragma once

#include <map>
#include <algorithm>

using namespace std;

#define CROSS_CORRELATION 0
#define MAX_MATCHING	8
#define DEFAULT_PERCENT_COVERAGE 10

// View size
#define FIT_SCREEN	0
#define FULL_IMAGE	1

/*
 * struct SUB_XY_POS
 */
struct SUB_XY_POS
{
	float	fX;
	float	fY;

	SUB_XY_POS()
	{
		fX = 0.0;
		fY = 0.0;
	}

	SUB_XY_POS(float x, float y)
	{
		fX = x;
		fY = y;
	}
};

typedef multimap<float, SUB_XY_POS, greater_equal<float> > MAP_MATCHING;
typedef multimap<float, SUB_XY_POS, greater_equal<float> >::value_type PAIR_MATCHING;
typedef multimap<float, SUB_XY_POS, greater_equal<float> >::iterator ITER_MATCHING;

/*
 * struct MATCHING_RESULT
 */
struct MATCHING_RESULT
{
	float	fX;
	float	fY;
	float	fCCC;

	SUB_XY_POS(float x, float y, float ccc)
	{
		fX = x;
		fY = y;
		fCCC = ccc;
	}
};

typedef map<int, MATCHING_RESULT, less<int> > MAP_MATCHED;
typedef map<int, MATCHING_RESULT, less<int> >::value_type PAIR_MATCHED;
typedef map<int, MATCHING_RESULT, less<int> >::iterator ITER_MATCHED;


// Comparing two floating point number
#define SMALL_NON_ZERO 1e-16 /* or something else small */
#define DBL_GT(X,Y) ( (X) - (Y) > SMALL_NON_ZERO ) /* X > Y */
#define DBL_LT(X,Y) ( (Y) - (X) > SMALL_NON_ZERO ) /* Y > X */
#define DBL_EQ(X,Y) ( !DBL_GT((X),(Y)) && !DBL_LT((X),(Y)) ) /* X == Y */

/*
 * enum MATCHING_FUNCTION
 * This enum lists all matching functions.
 */
enum MATCHING_FUNCTION
{
    CCC = 0,
    LSM
};

/*
 * enum MATCHING_COLOR
 * This enum lists the color based processing.
 */
enum MATCHING_COLOR
{
    GRAYSCALE = 0,
    RGB
};

/*
 * struct XY_POSITION
 */
struct XY_POSITION
{
	int	iX;
	int	iY;
};
