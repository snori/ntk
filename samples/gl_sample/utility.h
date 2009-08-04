/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: utility.h,v 1.1 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_GEOMETRY_UTILITY_H__
#define __NTK_GEOMETRY_UTILITY_H__


static const double PI = 3.1415926535897932384626433832795;


template<typename T>
inline T
deg2rad(T deg)
{
	static const T A = PI / (T)180;
	return deg * A;
}

template<typename T>
inline T
rad2deg(T rad)
{
	static const T A = (T)180 / PI;
	return rad * A;
}


#endif//EOH
