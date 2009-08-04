/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: message_test.cpp,v 1.3 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>


#include "ntk/application/message.h"
using namespace ntk;


class MessageTest : public CppUnit::TestFixture {
public:
	//
	// tests
	//
	void value_test1()
	{
		Message m(1);
		CPPUNIT_ASSERT_EQUAL(m.what, (uint)1);
	}
	void value_test2()
	{
		MSG msg;
		msg.message = 100;
		Message m(msg);
		CPPUNIT_ASSERT_EQUAL(m.what, (uint)100);
	}
	void value_test3()
	{
		MSG msg;
		msg.wParam = 1;
		msg.lParam = 10;
		Message m(msg);
		CPPUNIT_ASSERT_EQUAL(m.wparam() + m.lparam(), (uint)11);
	}

	void add_data_test1()
	{
		Message a;
		a.add_int("int", 100);
		CPPUNIT_ASSERT_EQUAL(a.find_int("int"), 100);
	}

	void copy_test1()
	{
		Message a(99);
		Message b = a;
		CPPUNIT_ASSERT_EQUAL(a.what, b.what);
	}
	void copy_test2()
	{
		Message a;
		a.add_int("int", 100);
		Message b = a;
		CPPUNIT_ASSERT_EQUAL(b.find_int("int"), 100);
	}
	void copy_test3()
	{
		Message a;
//		a.add_int("int", 100);
		Message b = a;
		b.add_int("int", 200);
		CPPUNIT_ASSERT_EQUAL(b.find_int("int"), 200);
	}

private:
	//
	// test-case registrations
	//
	CPPUNIT_TEST_SUITE(MessageTest);
	CPPUNIT_TEST(value_test1);
	CPPUNIT_TEST(value_test2);
	CPPUNIT_TEST(value_test3);

	CPPUNIT_TEST(add_data_test1);

	CPPUNIT_TEST(copy_test1);
	CPPUNIT_TEST(copy_test2);
	CPPUNIT_TEST(copy_test3);
	CPPUNIT_TEST_SUITE_END();

};// class MessageTest


CPPUNIT_TEST_SUITE_REGISTRATION(MessageTest);
