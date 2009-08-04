/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: directory_test.cpp,v 1.4 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>


#include "ntk/storage/directory.h"
using namespace ntk;


class DirectoryTest : public CppUnit::TestFixture {
public:
	//
	// tests
	//
	void is_root_directory_test_0()
	{
		CPPUNIT_ASSERT(Directory("c:\\").is_root_directory());
	}
	void is_root_directory_test_1()
	{
		CPPUNIT_ASSERT(Directory("c:/").is_root_directory());
	}
	void is_root_directory_test_2()
	{
		CPPUNIT_ASSERT_EQUAL(Directory("c:\\nszk\\").is_root_directory(), false);
	}
  void is_root_directory_test_3()
	{
		CPPUNIT_ASSERT_EQUAL(Directory("c:/nszk/").is_root_directory(), false);
	}
  void is_root_directory_test_4()
	{
		CPPUNIT_ASSERT_EQUAL(Directory("\\nszk\\").is_root_directory(), false);
	}
  void is_root_directory_test_5()
	{
		CPPUNIT_ASSERT_EQUAL(Directory("nszk/").is_root_directory(), false);
	}
  void is_root_directory_test_6()
	{
		Directory dir("c:/nszk/");
		CPPUNIT_ASSERT_EQUAL(dir.entry().path().as_string(), String("c:\\nszk"));
	}

private:
	//
	// test-case registrations
	//
	CPPUNIT_TEST_SUITE(DirectoryTest);
  CPPUNIT_TEST(is_root_directory_test_0);
  CPPUNIT_TEST(is_root_directory_test_1);
  CPPUNIT_TEST(is_root_directory_test_2);
  CPPUNIT_TEST(is_root_directory_test_3);
  CPPUNIT_TEST(is_root_directory_test_4);
  CPPUNIT_TEST(is_root_directory_test_5);
  CPPUNIT_TEST(is_root_directory_test_6);
	CPPUNIT_TEST_SUITE_END();

};// class DirectoryTest


CPPUNIT_TEST_SUITE_REGISTRATION(DirectoryTest);
