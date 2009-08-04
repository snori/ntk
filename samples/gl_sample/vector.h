/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: vector.h,v 1.1 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_GEOMETRY_VECTOR_H__
#define __NTK_GEOMETRY_VECTOR_H__


#include <math.h>
#include <assert.h>

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#include "utility.h"


#pragma pack(push, 1)

template<typename T>
class Vector2_ {
private:
	//
	// types
	//
	typedef Vector2_<T> This;

public:
	//
	// types
	//
	typedef T Value;

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Value value_t;
	#endif

	//
	// constants
	//
	enum {DIMENSION = 2};

	/* data position */
	enum {X = 0, Y};

	//
	// data
	//
	union {
		Value element[DIMENSION];
		struct {Value x, y;};
		struct {Value gray, alpha;};
	};

	//
	// methods
	//
	Vector2_()
	{}
	Vector2_(const Value* array)
	:	x(array[0]), y(array[1])
	{}
	Vector2_(const Value& x_, const Value& y_)
	:	x(x_), y(y_)
	{}
	Vector2_(const This& val)
	:	x(val.x), y(val.y)
	{}
	This& operator=(const This& rhs)
	{
		x = rhs.x; y = rhs.y;
		return *this;
	}

	Value length() const
	{
		return (Value)sqrt(squared_length());
	}
	Value squared_length() const
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
		return dot_product(*this, rhs);
	}
	Value cross(const This& rhs) const
	{
		return cross_product(*this, rhs);
	}

	//
	// manipulator
	//
	void reset(Value x_, Value y_)
	{
		x = x_;
		y = y_;
	}
	void reset(const Value* array)
	{
		x = array[0];
		y = array[1];
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
		return lhs.x==rhs.x && lhs.y==rhs.y;
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

		return lhs;
	}
	inline friend const This operator*(This lhs, const Value& rhs)
	{
		return lhs *= rhs;
	}

	// vector / vector
	inline friend const This& operator/=(This& lhs, const This& rhs)
	{
		assert(rhs.x != Value(0.f) && rhs.y != Value(0.f));

		lhs.x /= rhs.x;
		lhs.y /= rhs.y;

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

		return lhs;
	}
	inline friend const This operator/(This lhs, const Value& rhs)
	{
		return lhs /= rhs;
	}

	// -(vector)
	const This operator-() const
	{
		return This(-x, -y);
	}

};// class Vector2_

template<typename T>
class Vector3_ {
private:
	//
	// types
	//
	typedef Vector3_<T> This;

public:
	//
	// types
	//
	typedef T Value;

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Value value_t;
	#endif

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
	Vector3_()
	{}
	Vector3_(const Value* array)
	:	x(array[0]), y(array[1]), z(array[2])
	{}
	Vector3_(const Value& x_, const Value& y_, const Value& z_)
	:	x(x_), y(y_), z(z_)
	{}
	Vector3_(const This& val)
	:	x(val.x), y(val.y), z(val.z)
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
		return dot_product(*this, rhs);
	}
	This cross(const This& rhs) const
	{
		return cross_product(*this, rhs);
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

};// class Vector3_

template<typename T>
class Vector4_ {
private:
	//
	// types
	//
	typedef Vector4_<T> This;

public:
	//
	// types
	//
	typedef T Value;

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Value value_t;
	#endif

	//
	// constants
	//
	enum {DIMENSION = 4};

	/* data position */
	enum {X = 0, Y, Z, W,};
	enum {RED = 0, GREEN, BLUE, ALPHA};

	//
	// data
	//
	union {
		Value element[DIMENSION];
		struct {Value x, y, z, w;};
		struct {Value red, green, blue, alpha;};
	};

	//
	// methods
	//
	Vector4_()
	{}
	Vector4_(const Value* array)
	:	x(array[0]), y(array[1]), z(array[2]), w(array[3])
	{}
	Vector4_(const Value& x_, const Value& y_, const Value& z_, const Value& w_)
	:	x(x_), y(y_), z(z_), w(w_)
	{}
	Vector4_(const This& val)
	:	x(val.x), y(val.y), z(val.z), w(val.w)
	{}
	This& operator=(const This& rhs)
	{
		x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
		return *this;
	}

	Value length() const
	{
		return (Value)sqrt(squared_length());
	}
	Value squared_length() const
	{
		return x*x + y*y + z*z + w*w;
	}
	Value length_2d() const
	{
		return sqrt(squared_length_2d());
	}
	Value squared_length_2d() const
	{
		return x*x + y*y;
	}
	Value length_3d() const
	{
		return sqrt(squared_length_3d());
	}
	Value squared_length_3d() const
	{
		return x*x + y*y + z*z;
	}

	void normalize()
	{
		float len = length();
		if(len != 0)
			*this /= len;
	}
	This normal() const
	{
		This n = *this;
		n.normalize();
		return n;
	}
	void homogenize()
	{
		if(w != Value(0))
		{
			x /= w;
			y /= w;
			z /= w;
			w = 1;
		}
	}

	Value dot(const This& rhs) const
	{
		return dot_product(*this, rhs);
	}
	This cross(const This& rhs) const
	{
		return cross_product(*this, rhs);
	}

	//
	// manipulator
	//
	void reset(Value x_, Value y_, Value z_, Value w_)
	{
		x = x_;
		y = y_;
		z = z_;
		w = w_;
	}
	void reset(const Value* array)
	{
		x = array[0];
		y = array[1];
		z = array[2];
		w = array[3];
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
		return lhs.x==rhs.x && lhs.y==rhs.y && lhs.z==rhs.z && lhs.w==rhs.w;
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
		lhs.w += rhs.w;

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
		lhs.w += rhs;

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
		lhs.w -= rhs.w;

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
		lhs.w -= rhs;

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
		lhs.w *= rhs.w;

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
		lhs.w *= rhs;

		return lhs;
	}
	inline friend const This operator*(This lhs, const Value& rhs)
	{
		return lhs *= rhs;
	}

	// vector / vector
	inline friend const This& operator/=(This& lhs, const This& rhs)
	{
		assert(rhs.x != Value(0.f) && rhs.y != Value(0.f) && rhs.z != Value(0.f) && rhs.w != Value(0.f));

		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
		lhs.z /= rhs.z;
		lhs.w /= rhs.w;

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
		lhs.w /= rhs;

		return lhs;
	}
	inline friend const This operator/(This lhs, const Value& rhs)
	{
		return lhs /= rhs;
	}

	// -(vector)
	const This operator-() const
	{
		return This(-x, -y, -z, -w);
	}

};// class Vector4_

template<typename T>
class Matrix4_ {
private:
	//
	// types
	//
	typedef Matrix4_<T> This;

public:
	//
	// types
	//
	typedef T Value;
	typedef size_t Size;

	typedef Vector3_<T> Tuple3;
	typedef Vector4_<T> Tuple4;

	enum InitialType {UNIT_MATRIX};

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Value value_t;
		typedef Size size_t;
		typedef InitialType initia_type_t;
	#endif

	//
	// constants
	//
	enum {ROW_DIMENSION = 4, COLUMN_DIMENSION = 4};
	enum {NUM_ELEMENTS = ROW_DIMENSION * COLUMN_DIMENSION};

	enum {
		XX=0, XY=4, XZ=8,  XW=12,
		YX=1, YY=5, YZ=9,  YW=13,
		ZX=2, ZY=6, ZZ=10, ZW=14,
		WX=3, WY=7, WZ=11, WW=15,
	};

	//
	// data
	//
	union {
		Value elements[NUM_ELEMENTS];
		struct {
			Value _00, _10, _20, _30;
			Value _01, _11, _21, _31;
			Value _02, _12, _22, _32;
			Value _03, _13, _23, _33;
		};
		struct {
			Value xx, yx, zx, wx;
			Value xy, yy, zy, wy;
			Value xz, yz, zz, wz;
			Value xw, yw, zw, ww;
		};
	};

	//
	// methods
	//
	Matrix4_() {}
	Matrix4_(// 3x4 Matrix
		Value xx_, Value xy_, Value xz_, Value xw_,
		Value yx_, Value yy_, Value yz_, Value yw_,
		Value zx_, Value zy_, Value zz_, Value zw_)
	:	xx(xx_), yx(yx_), zx(zx_), wx(0)
	,	xy(xy_), yy(yy_), zy(zy_), wy(0)
	,	xz(xz_), yz(yz_), zz(zz_), wz(0)
	,	xw(xw_), yw(yw_), zw(zw_), ww(1)
	{}
	Matrix4_(// 4x4 Matrix
		Value xx_, Value xy_, Value xz_, Value xw_,
		Value yx_, Value yy_, Value yz_, Value yw_,
		Value zx_, Value zy_, Value zz_, Value zw_,
		Value wx_, Value wy_, Value wz_, Value ww_)
	:	xx(xx_), yx(yx_), zx(zx_), wx(wx)
	,	xy(xy_), yy(yy_), zy(zy_), wy(wy)
	,	xz(xz_), yz(yz_), zz(zz_), wz(wz)
	,	xw(xw_), yw(yw_), zw(zw_), ww(ww)
	{}
	Matrix4_(Value* initial_matrix)
	{
		reset(initial_matrix);
	}
	Matrix4_(InitialType initial_type)
	:	xx(1), yx(0), zx(0), wx(0)
	,	xy(0), yy(1), zy(0), wy(0)
	,	xz(0), yz(0), zz(1), wz(0)
	,	xw(0), yw(0), zw(0), ww(1)
	{}
	Matrix4_(const Matrix4_& val)
	{
		reset(val.elements);
	}

	void reset(
		Value xx_, Value xy_, Value xz_, Value xw_,
		Value yx_, Value yy_, Value yz_, Value yw_,
		Value zx_, Value zy_, Value zz_, Value zw_)
	{
		xx=xx_, xy=xy_, xz=xz_, xw=xw_;
		yx=yx_, yy=yy_, yz=yz_, yw=yw_;
		zx=zx_, zy=zy_, zz=zz_, zw=zw_;
	}
	void reset(
		Value xx_, Value xy_, Value xz_, Value xw_,
		Value yx_, Value yy_, Value yz_, Value yw_,
		Value zx_, Value zy_, Value zz_, Value zw_,
		Value wx_, Value wy_, Value wz_, Value ww_)
	{
		xx=xx_, xy=xy_, xz=xz_, xw=xw_;
		yx=yx_, yy=yy_, yz=yz_, yw=yw_;
		zx=zx_, zy=zy_, zz=zz_, zw=zw_;
		wx=wx_, wy=wy_, wz=wz_, ww=ww_;
	}
	void reset(const Value* matrix)
	{
		memcpy(elements, matrix, sizeof(Value) * NUM_ELEMENTS);
	}

	void translate(const Tuple3& v)
	{
		xw += xx * v.x + xy * v.y + xz * v.z;
		yw += yx * v.x + yy * v.y + yz * v.z;
		zw += zx * v.x + zy * v.y + zz * v.z;
	}
	void translate(const Tuple4& v)
	{
		xw += xx * v.x + xy * v.y + xz * v.z;
		yw += yx * v.x + yy * v.y + yz * v.z;
		zw += zx * v.x + zy * v.y + zz * v.z;
		ww += wx * v.x + wy * v.y + wz * v.z;
	}

	void rotate_x(Value deg)
	{
		Value rad  = deg2rad(deg);
		Value sin_ = sin(rad);
		Value cos_ = cos(rad);

		Value
		tmp =  xy * cos_ + xz * sin_;
		xz  = -xy * sin_ + xz * cos_;
		xy  =  tmp;
		tmp =  yy * cos_ + yz * sin_;
		yz  = -yy * sin_ + yz * cos_;
		yy  =  tmp;
		tmp =  zy * cos_ + zz * sin_;
		zz  = -zy * sin_ + zz * cos_;
		zy  =  tmp;
	}
	void rotate_y(Value deg)
	{
		Value rad  = deg2rad(deg);
		Value sin_ = sin(rad);
		Value cos_ = cos(rad);

		Value
		tmp = xx * cos_ - xz * sin_;
		xz  = xx * sin_ + xz * cos_;
		xx  = tmp;
		tmp = yx * cos_ - yz * sin_;
		yz  = yx * sin_ + yz * cos_;
		yx  = tmp;
		tmp = zx * cos_ - zz * sin_;
		zz  = zx * sin_ + zz * cos_;
		zx  = tmp;
	}
	void rotate_z(Value deg)
	{
		Value rad  = deg2rad(deg);
		Value sin_ = sin(rad);
		Value cos_ = cos(rad);

		Value
		tmp =  xx * cos_ + xy * sin_;
		xy  = -xx * sin_ + xy * cos_;
		xx  =  tmp;
		tmp =  yx * cos_ + yy * sin_;
		yy  = -yx * sin_ + yy * cos_;
		yx  =  tmp;
		tmp =  zx * cos_ + zy * sin_;
		zy  = -zx * sin_ + zy * cos_;
		zx  =  tmp;
	}

	void scale(Tuple3 v)
	{
		static const float EPSILON = 1.e-10f;
		if(v.x == 0.f) v.x = EPSILON;
		if(v.y == 0.f) v.y = EPSILON;
		if(v.z == 0.f) v.z = EPSILON;

		xx *= v.x; xy *= v.y; xz *= v.z;
		yx *= v.x; yy *= v.y; yz *= v.z;
		zx *= v.x; zy *= v.y; zz *= v.z;
	}
	void scale(Tuple4 v)
	{
		static const float EPSILON = 1.e-10f;
		if(v.x == 0.f) v.x = EPSILON;
		if(v.y == 0.f) v.y = EPSILON;
		if(v.z == 0.f) v.z = EPSILON;

		xx *= v.x; xy *= v.y; xz *= v.z;
		yx *= v.x; yy *= v.y; yz *= v.z;
		zx *= v.x; zy *= v.y; zz *= v.z;
		wx *= v.x; wy *= v.y; wz *= v.z;
	}

	//
	// accessors and manipulators
	//
	Value& operator[](int index) {return elements[index];}
	const Value& operator[](int index) const {return elements[index];}

	//
	// operators
	//
	friend const Tuple3 operator*(const Tuple3& lhs, const This& rhs_matrix)
	{
		const Value* rhs = rhs_matrix.elements;
		return Tuple3(
			lhs.x * rhs[XX] + lhs.y * rhs[XY] + lhs.z * rhs[XZ] + rhs[XW],
			lhs.x * rhs[YX] + lhs.y * rhs[YY] + lhs.z * rhs[YZ] + rhs[YW],
			lhs.x * rhs[ZX] * lhs.y * rhs[ZY] + lhs.z * rhs[ZZ] + rhs[ZW]);
	}
	friend const Tuple4 operator*(const Tuple4& lhs, const This& rhs_matrix)
	{
		const Value* rhs = rhs_matrix.elements;
		return Tuple3(
			lhs.x * rhs[XX] + lhs.y * rhs[XY] + lhs.z * rhs[XZ] + lhs.w * rhs[XW],
			lhs.x * rhs[YX] + lhs.y * rhs[YY] + lhs.z * rhs[YZ] + lhs.w * rhs[YW],
			lhs.x * rhs[ZX] * lhs.y * rhs[ZY] + lhs.z * rhs[ZZ] + lhs.w * rhs[ZW],
			lhs.x * rhs[WX] * lhs.y * rhs[WY] + lhs.z * rhs[WZ] + lhs.w * rhs[WW]);
	}
	friend const This operator*(const This& lhs_matrix, const This& rhs_matrix)
	{
		const Value *lhs = lhs_matrix.elements, *rhs = rhs_matrix.elements;// ローカル変数からアクセスするほうが効率がいいはず
		return This(
			lhs[XX] * rhs[XX] + lhs[XY] * rhs[YX] + lhs[XZ] * rhs[ZX] + lhs[XW] * rhs[WX],
			lhs[XX] * rhs[XY] + lhs[XY] * rhs[YY] + lhs[XZ] * rhs[ZY] + lhs[XW] * rhs[WY],
			lhs[XX] * rhs[XZ] + lhs[XY] * rhs[YZ] + lhs[XZ] * rhs[ZZ] + lhs[XW] * rhs[WZ],
			lhs[XX] * rhs[XW] + lhs[XY] * rhs[YW] + lhs[XZ] * rhs[ZW] + lhs[XW] * rhs[WW],
			lhs[YX] * rhs[XX] + lhs[YY] * rhs[YX] + lhs[YZ] * rhs[ZX] + lhs[YW] * rhs[WX],
			lhs[YX] * rhs[XY] + lhs[YY] * rhs[YY] + lhs[YZ] * rhs[ZY] + lhs[YW] * rhs[WY],
			lhs[YX] * rhs[XZ] + lhs[YY] * rhs[YZ] + lhs[YZ] * rhs[ZZ] + lhs[YW] * rhs[WZ],
			lhs[YX] * rhs[XW] + lhs[YY] * rhs[YW] + lhs[YZ] * rhs[ZW] + lhs[YW] * rhs[WW],
			lhs[ZX] * rhs[XX] + lhs[ZY] * rhs[YX] + lhs[ZZ] * rhs[ZX] + lhs[ZW] * rhs[WX],
			lhs[ZX] * rhs[XY] + lhs[ZY] * rhs[YY] + lhs[ZZ] * rhs[ZY] + lhs[ZW] * rhs[WY],
			lhs[ZX] * rhs[XZ] + lhs[ZY] * rhs[YZ] + lhs[ZZ] * rhs[ZZ] + lhs[ZW] * rhs[WZ],
			lhs[ZX] * rhs[XW] + lhs[ZY] * rhs[YW] + lhs[ZZ] * rhs[ZW] + lhs[ZW] * rhs[WW]);//,
//			lhs[WX] * rhs[XX] + lhs[WY] * rhs[YX] + lhs[WZ] * rhs[ZX] + lhs[WW] * rhs[WX],
//			lhs[WX] * rhs[XY] + lhs[WY] * rhs[YY] + lhs[WZ] * rhs[ZY] + lhs[WW] * rhs[WY],
//			lhs[WX] * rhs[XZ] + lhs[WY] * rhs[YZ] + lhs[WZ] * rhs[ZZ] + lhs[WW] * rhs[WZ],
//			lhs[WX] * rhs[XW] + lhs[WY] * rhs[YW] + lhs[WZ] * rhs[ZW] + lhs[WW] * rhs[WW]);
	}

};// class Matrix4_

#pragma pack(pop)


inline void
Vector2_<float>::normalize()
{
	float len = length();
	if(len != 0.f)
		*this *= 1.f/len;
}

void
Vector3_<float>::normalize()
{
	float len = length();
	if(len != 0.f)
		*this *= 1.f/len;
}

void
Vector4_<float>::normalize()
{
	float len = length();
	if(len != 0.f)
		*this *= 1.f/len;
}

inline void
Vector2_<double>::normalize()
{
	double len = length();
	if(len != 0.)
		*this *= 1./len;
}

void
Vector3_<double>::normalize()
{
	double len = length();
	if(len != 0.)
		*this *= 1./len;
}

void
Vector4_<double>::normalize()
{
	double len = length();
	if(len != 0.)
		*this *= 1./len;
}

template<typename T>
inline T
dot_product(const Vector2_<T>& lhs, const Vector2_<T>& rhs)
{
	return lhs.x*rhs.x + lhs.y*rhs.y;
}

template<typename T>
inline T
dot_product(const Vector3_<T>& lhs, const Vector3_<T>& rhs)
{
	return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}

template<typename T>
inline T
dot_product(const Vector4_<T>& lhs, const Vector4_<T>& rhs)
{
	return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z + lhs.w*rhs.w;
}

template<typename T>
inline Vector3_<T>
cross_product(const Vector3_<T>& lhs, const Vector3_<T>& rhs)
{
	return Vector3_<T>(
		lhs.y*rhs.z - rhs.y*lhs.z,
		lhs.z*rhs.x - rhs.z*lhs.x,
		lhs.x*rhs.y - rhs.x*lhs.y);
}


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	#define vector2_ Vector2_
	#define vector3_ Vector3_
	#define vector4_ Vector4_
	#define matrix4_ Matrix4_
#endif


#endif//EOH
