/******************************************************************************

	The NTK Library : samples/FireSim
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: vector3.h,v 1.1.1.1 2003/02/26 06:49:06 nsuzuki Exp $

******************************************************************************/
#ifndef __VECTOR3_H__
#define __VECTOR3_H__


#include "Tuple3.h"


namespace ntk {


template<class T>
class Vector3_ : public Tuple3_<T> {
private:
	//
	// types
	//
	typedef Tuple3_<Value> Super;
	typedef Vector3_<Value> This;

public:
	//
	// methods
	//
	Vector3_()
	:	Super()
	{}
	Vector3_(const Value* array)
	:	Super(array)
	{}
	Vector3_(const Value& x_, const Value& y_, const Value z_)
	:	Super(x_, y_, z_)
	{}
	Vector3_(const Super& vec)
	:	Super(vec)
	{}
	This& operator=(const Super& rhs)
	{
		Super::operator=(rhs);
		return *this;
	}

};// class Vector3_


typedef Vector3_<int>    Vector3i;
typedef Vector3_<float>  Vector3f;
typedef Vector3_<double> Vector3d;
typedef Vector3_<coord>  Vector3cd;


}// namespace ntk


#endif//EOH