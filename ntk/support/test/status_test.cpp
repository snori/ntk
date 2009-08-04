/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: status_test.cpp,v 1.7 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>


#include "ntk/support/status.h"
#include <iostream>
using namespace ntk;


class StatusTest : public CppUnit::TestFixture {
public:
	//
	// tests
	//
	void as_value_test1()
	{
		status_t st0;
		status_t st1 = st0;

		CPPUNIT_ASSERT_EQUAL(st0.what(), (int)st::OK);
	}
	void as_value_test2()
	{
		status_t st0;
		status_t st1 = st0;

		CPPUNIT_ASSERT_EQUAL(st1.what(), (int)st::OK);
	}
	void as_value_test3()
	{
		status_t st0;
		status_t st1 = st0;

		CPPUNIT_ASSERT_EQUAL(st0.message(), String(""));
	}
	void as_value_test4()
	{
		status_t st0;
		status_t st1 = st0;

		CPPUNIT_ASSERT_EQUAL(st1.message(), String(""));
	}
	void as_value_test5()
	{
		status_t sts(st::ERR);
		sts.reset(st::OK);

		CPPUNIT_ASSERT_EQUAL(sts.message(), String(""));
	}
	void as_value_test6()
	{
		status_t sts(st::ERR, "test message");

		CPPUNIT_ASSERT_EQUAL(sts.message(), String("test message"));
	}
	void as_value_test7()
	{
		status_t sts(st::ERR);
		sts = st::OK;

		CPPUNIT_ASSERT_EQUAL(sts.what(), (int)st::OK);
	}

	void validation_test1()
	{
		status_t def, ok(st::OK), no_err(st::NO_ERR), succeeded(st::SUCCEEDED);
		status_t positive_num(100, "適当なステータス（成功）");

		CPPUNIT_ASSERT(
			def.is_valid() && ok.is_valid() &&
			no_err.is_valid() && succeeded.is_valid() &&
			positive_num.is_valid());
	}
	void validation_test2()
	{
		status_t err(st::ERR), failed(st::FAILED);
		status_t sys_error(st::SYSTEM_ERROR), ansi_error(st::ANSI_ERROR);
		status_t no_mem(st::NO_MEMORY_ERROR), bad_val(st::BAD_VALUE_ERROR);
		status_t no_init(st::NOT_INITIALIZED_ERROR);

		CPPUNIT_ASSERT(
			err.is_valid() == false && failed.is_valid() == false &&
			sys_error.is_valid() == false && ansi_error.is_valid() == false &&
			no_mem.is_valid() == false && bad_val.is_valid() == false &&
			no_init.is_valid() == false);
	}

	void evaluation_test1()
	{
		status_t ok, err(st::ERR);
		CPPUNIT_ASSERT(ok.is_evaluated() == true &&
			err.is_evaluated() == false);
	}
	void evaluation_test2()
	{
		status_t sts(1);
		CPPUNIT_ASSERT(sts.is_evaluated());
	}
	void evaluation_test3()
	{
		status_t evaluated(st::ERR), not_evaluated(st::ERR), dont_evaluate(st::ERR);
		evaluated.what();
		dont_evaluate.what(false);

		CPPUNIT_ASSERT(evaluated.is_evaluated() == true &&
			not_evaluated.is_evaluated() == false &&
			dont_evaluate.is_evaluated() == false);
	}

private:
	//
	// test-case registrations
	//
	CPPUNIT_TEST_SUITE(StatusTest);
	CPPUNIT_TEST(as_value_test1);
	CPPUNIT_TEST(as_value_test2);
	CPPUNIT_TEST(as_value_test3);
	CPPUNIT_TEST(as_value_test4);
	CPPUNIT_TEST(as_value_test5);
	CPPUNIT_TEST(as_value_test6);

	CPPUNIT_TEST(validation_test1);
	CPPUNIT_TEST(validation_test2);

	CPPUNIT_TEST(evaluation_test1);
	CPPUNIT_TEST(evaluation_test2);
	CPPUNIT_TEST(evaluation_test3);
	CPPUNIT_TEST_SUITE_END();

};// class StatusTest


CPPUNIT_TEST_SUITE_REGISTRATION(StatusTest);
