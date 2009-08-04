/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: dc_test.cpp,v 1.3 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>


#include "ntk/interface/dc.h"
#include <ntk/interface/gdiobject.h>
using namespace ntk;


class DCTest : public CppUnit::TestFixture {
public:
	//
	// tests
	//
	void pop_test1()
	{
		ScreenDC dc;
		Pen pen = dc.pen();

		dc.push_state();

		dc.set_pen(RGBColor(100, 100, 101));
		dc.set_pen(RGBColor(100, 100, 102));
		dc.set_pen(RGBColor(100, 100, 103));
		dc.set_pen(RGBColor(100, 100, 104));
		dc.set_pen(RGBColor(100, 100, 105));
		dc.set_pen(RGBColor(100, 100, 106));

		CPPUNIT_ASSERT(dc.pop_state());
	}
	void pop_test2()
	{
		ScreenDC dc;
		Pen pen = dc.pen();

		dc.push_pen();
		dc.set_pen(RGBColor(100, 100, 100));

		CPPUNIT_ASSERT(dc.pop_pen());
	}
	void push_pop_test1()
	{
		ScreenDC dc;
		Pen pen = dc.pen();

		dc.push_pen();
		dc.set_pen(RGBColor(100, 100, 100));
		dc.pop_pen();

		CPPUNIT_ASSERT(pen.handle() == dc.pen());
	}

private:
	//
	// test-case registrations
	//
	CPPUNIT_TEST_SUITE(DCTest);
	CPPUNIT_TEST(pop_test1);
	CPPUNIT_TEST(pop_test2);
	CPPUNIT_TEST(push_pop_test1);
	CPPUNIT_TEST_SUITE_END();

};// class DCTest


CPPUNIT_TEST_SUITE_REGISTRATION(DCTest);
