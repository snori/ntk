/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: version_test.cpp,v 1.2 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>


#include "ntk/support/version.h"
using namespace ntk;


class VersionTest : public CppUnit::TestFixture {
public:
	//
	// tests
	//
	void operator_test1()
	{
		Version v1(1, 1, 1, 1), v2(1, 1, 1, 1);
		CPPUNIT_ASSERT(v1 == v2 && !(v1 != v2));
	}
	void operator_test2()
	{
		Version v1(1, 1, 1, 1), v2(1, 1, 1, 2);
		CPPUNIT_ASSERT(v1 != v2 && !(v1 == v2));
	}
	void operator_test3()
	{
		Version v1(0), v2(1);
		CPPUNIT_ASSERT(v1 < v2 && !(v1 > v2) && !(v1 >= v2));
	}
	void operator_test4()
	{
		Version v1(0, 1), v2(1);
		CPPUNIT_ASSERT(v1 < v2 && !(v1 > v2) && !(v1 >= v2));
	}
	void operator_test5()
	{
		Version v1(0, 0, 1), v2(1);
		CPPUNIT_ASSERT(v1 < v2 && !(v1 > v2) && !(v1 >= v2));
	}
	void operator_test6()
	{
		Version v1(0, 0, 0, 1), v2(1);
		CPPUNIT_ASSERT(v1 < v2 && !(v1 > v2) && !(v1 >= v2));
	}
	void operator_test7()
	{
		Version v1(1), v2(0);
		CPPUNIT_ASSERT(v1 > v2 && !(v1 < v2) && !(v1 <= v2));
	}
	void operator_test8()
	{
		Version v1(1), v2(0, 1);
		CPPUNIT_ASSERT(v1 > v2 && !(v1 < v2) && !(v1 <= v2));
	}
	void operator_test9()
	{
		Version v1(1), v2(0, 0, 1);
		CPPUNIT_ASSERT(v1 > v2 && !(v1 < v2) && !(v1 <= v2));
	}
	void operator_test10()
	{
		Version v1(1), v2(0, 0, 0, 1);
		CPPUNIT_ASSERT(v1 > v2 && !(v1 < v2) && !(v1 <= v2));
	}

private:
	//
	// test-case registrations
	//
	CPPUNIT_TEST_SUITE(VersionTest);
	CPPUNIT_TEST(operator_test1);
	CPPUNIT_TEST(operator_test2);
	CPPUNIT_TEST(operator_test3);
	CPPUNIT_TEST(operator_test4);
	CPPUNIT_TEST(operator_test5);
	CPPUNIT_TEST(operator_test6);
	CPPUNIT_TEST(operator_test7);
	CPPUNIT_TEST(operator_test8);
	CPPUNIT_TEST(operator_test9);
	CPPUNIT_TEST(operator_test10);
	CPPUNIT_TEST_SUITE_END();

};// class VersionTest


CPPUNIT_TEST_SUITE_REGISTRATION(VersionTest);
