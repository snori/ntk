/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: string_test.cpp,v 1.4 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>


#include "ntk/support/string.h"
using namespace ntk;


class StringTest : public CppUnit::TestFixture {
public:
	//
	// tests
	//
	void format_string_test1()
	{
		String result = format_string("%d, %2x", 1, 0xaa);
		CPPUNIT_ASSERT_EQUAL(result, String("1, aa"));
	}

private:
	//
	// test-case registrations
	//
	CPPUNIT_TEST_SUITE(StringTest);
	CPPUNIT_TEST(format_string_test1);
	CPPUNIT_TEST_SUITE_END();

};// class StringTest


CPPUNIT_TEST_SUITE_REGISTRATION(StringTest);
