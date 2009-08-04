/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: rect_test.cpp,v 1.2 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>


#include "ntk/interface/rect.h"
using namespace ntk;


class RectTest : public CppUnit::TestFixture {
public:
	//
	// tests
	//
	void and_operator_test1()// &
	{
		Rect l(10, 10, 20, 20);
		Rect r(15, 15, 25, 25);
		Rect result = l & r;

		CPPUNIT_ASSERT(result.width() == 5 && result.height() == 5);
	}
	void and_operator_test2()// &
	{
		Rect l(10, 10, 20, 20);
		Rect r(25, 25, 35, 35);
		Rect result = l & r;

		CPPUNIT_ASSERT_EQUAL(result.is_valid(), false);
	}
	void or_operator_test1()// |
	{
		Rect l(10, 10, 20, 20);
		Rect r(15, 15, 25, 25);
		Rect result = l | r;

		CPPUNIT_ASSERT(result.width() == 15 && result.height() == 15);
	}
	void or_operator_test2()// |
	{
		Rect l(10, 10, 20, 20);
		Rect r(25, 25, 35, 35);
		Rect result = l | r;

		CPPUNIT_ASSERT(result.width() == 25 && result.height() == 25);
	}

private:
	//
	// test-case registrations
	//
	CPPUNIT_TEST_SUITE(RectTest);
	CPPUNIT_TEST(and_operator_test1);
	CPPUNIT_TEST(and_operator_test2);
	CPPUNIT_TEST(or_operator_test1);
	CPPUNIT_TEST(or_operator_test2);
	CPPUNIT_TEST_SUITE_END();

};// class RectTest


CPPUNIT_TEST_SUITE_REGISTRATION(RectTest);
