/******************************************************************************

	The NTK Library : samples/FireSim
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: tuple3.h,v 1.2 2003/05/02 19:04:51 snori Exp $

******************************************************************************/
#ifndef __TUPLE3_H__
#define __TUPLE3_H__


#include <math.h>
#include <assert.h>
#include <ntk/defs.h>


namespace ntk {


#pragma pack(push, 1)
template<typename T>
class Tuple3_ {
private:
	//
	// types
	//
	typedef Tuple3_<T> This;

public:
	//
	// types
	//
	typedef T Value;

	//
	// constants
	//
	enum {DIMENSION = 3};

	/* data position */
	enum {X = 0, Y, Z};
	enum {RED = 0, GREEN, BLUE};

	//
	// data
	//
	union {
		Value element[DIMENSION];
		struct {Value x, y, z;};
		struct {Value red, green, blue;};
	};

	//
	// methods
	//
	Tuple3_()
	{}
	Tuple3_(const Value* array)
	:	x(array[0]), y(array[1]), z(array[2])
	{}
	Tuple3_(const Value& x_, const Value& y_, const Value& z_)
	:	x(x_), y(y_), z(z_)
	{}
	Tuple3_(const This& vector)
	:	x(vector.x), y(vector.y), z(vector.z)
	{}
	This& operator=(const This& rhs)
	{
		x = rhs.x; y = rhs.y; z = rhs.z;
		return *this;
	}

	Value length() const
	{
		return (Value)sqrt(squared_length());
	}
	Value squared_length() const
	{
		return x*x + y*y + z*z;
	}
	Value length_2d() const
	{
		return sqrt(squared_length_2d());
	}
	Value squared_length_2d() const
	{
		return x*x + y*y;
	}

	void normalize()
	{
		float len = length();
		if(len > 0.f) *this /= len;
	}
	This normal() const
	{
		This n = *this;
		n.normalize();
		return n;
	}

	Value dot(const This& rhs) const
	{
		return dot(*this, rhs);
	}
	This cross(const This& rhs) const
	{
		return cross(*this, rhs);
	}

	//
	// manipulator
	//
	void reset(Value x_, Value y_, Value z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}
	void reset(const Value* array)
	{
		x = array[0];
		y = array[1];
		z = array[2];
	}

	//
	// operators
	//
	Value& operator[](int index)
	{
		assert(0 <= index && index < DIMENSION);
		return element[index];
	}
	const Value& operator[](int index) const
	{
		assert(0 <= index && index < DIMENSION);
		return element[index];
	}

	//
	// friend global operators
	//
	inline friend bool operator==(const This& lhs, const This& rhs)
	{
		return lhs.x==rhs.x && lhs.y==rhs.y && lhs.z==rhs.z;
	}
	inline friend bool operator!=(const This& lhs, const This& rhs)
	{
		return !(lhs == rhs);
	}

	/* vector + vector */
	inline friend const This& operator+=(This& lhs, const This& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;

		return lhs;
	}
	inline friend const This operator+(This lhs, const This& rhs)
	{
		return lhs += rhs;
	}

	/* vector + Value */
	inline friend const This& operator+=(This& lhs, const Value& rhs)
	{
		lhs.x += rhs;
		lhs.y += rhs;
		lhs.z += rhs;

		return lhs;
	}
	inline friend const This operator+(This lhs, const Value& rhs)
	{
		return lhs += rhs;
	}

	/* vector - vector */
	inline friend const This& operator-=(This& lhs, const This& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;

		return lhs;
	}
	inline friend const This operator-(This lhs, const This& rhs)
	{
		return lhs -= rhs;
	}

	/* vector - Value */
	inline friend const This& operator-=(This& lhs, const Value& rhs)
	{
		lhs.x -= rhs;
		lhs.y -= rhs;
		lhs.z -= rhs;

		return lhs;
	}
	inline friend const This operator-(This lhs, const Value& rhs)
	{
		return lhs -= rhs;
	}

	/* vector * vector */
	inline friend const This& operator*=(This& lhs, const This& rhs)
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		lhs.z *= rhs.z;

		return lhs;
	}
	inline friend const This operator*(This lhs, const This& rhs)
	{
		return lhs *= rhs;
	}

	/* vector * Value */
	inline friend const This& operator*=(This& lhs, const Value& rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;

		return lhs;
	}
	inline friend const This operator*(This lhs, const Value& rhs)
	{
		return lhs *= rhs;
	}

	// vector / vector
	inline friend const This& operator/=(This& lhs, const This& rhs)
	{
		assert(rhs.x != Value(0.f) && rhs.y != Value(0.f) && rhs.z != Value(0.f));

		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
		lhs.z /= rhs.z;

		return lhs;
	}
	inline friend const This operator/(This lhs, const This& rhs)
	{
		return lhs /= rhs;
	}

	// vector / Value
	inline friend const This& operator/=(This& lhs, const Value& rhs)
	{
		assert(rhs != Value(0.f));

		lhs.x /= rhs;
		lhs.y /= rhs;
		lhs.z /= rhs;

		return lhs;
	}
	inline friend const This operator/(This lhs, const Value& rhs)
	{
		return lhs /= rhs;
	}

	// -(vector)
	const This operator-() const
	{
		return This(-x, -y, -z);
	}

};// class Tuple3_
#pragma pack(pop)

void
Tuple3_<float>::normalize()
{
	float len = length();
	if(len != 0.f) *this *= 1.0f/len;
}

void
Tuple3_<double>::normalize()
{
	double len = length();
	if(len != 0.f) *this *= 1.0f/len;
}


template<typename T>
inline T
dot(const Tuple3_<T>& lhs, const Tuple3_<T>& rhs)
{
	return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}

template<typename T>
inline Tuple3_<T>
cross(const Tuple3_<T>& lhs, const Tuple3_<T>& rhs)
{
	return Tuple3_<T>(
		lhs.y*rhs.z - rhs.y*lhs.z,
		lhs.z*rhs.x - rhs.z*lhs.x,
		lhs.x*rhs.y - rhs.x*lhs.y);
}


}// namespace ntk


#endif//EOH