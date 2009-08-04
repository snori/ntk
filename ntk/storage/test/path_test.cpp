/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: path_test.cpp,v 1.4 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>


#include "ntk/storage/path.h"
using namespace ntk;


class PathTest : public CppUnit::TestFixture {
public:
	//
	// tests
	//
  void normalize_test()
	{
		Path path("c:\\develop\\ntk\\src\\..\\readme.txt");
		path.normalize();

		CPPUNIT_ASSERT_EQUAL(path.as_string(), Path("c:\\develop\\ntk\\readme.txt").as_string());
  }

private:
	//
	// test-case registrations
	//
	CPPUNIT_TEST_SUITE(PathTest);
  CPPUNIT_TEST(normalize_test);
	CPPUNIT_TEST_SUITE_END();

};// class PathTest


CPPUNIT_TEST_SUITE_REGISTRATION(PathTest);
