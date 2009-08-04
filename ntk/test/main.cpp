/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: main.cpp,v 1.4 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#include <stdio.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/CompilerOutputter.h>
#include <ntk/defs.h>
#include <ntk/test/config.h>


// include test //
#include <ntk/ntk.h>


int
main(int argc, char** argv)
{
	using namespace CppUnit;

	TextUi::TestRunner runner;
	runner.addTest(TestFactoryRegistry::getRegistry().makeTest());

	#if TEST_MODE == CONSOLE_OUTPUT_MODE
			runner.setOutputter(new TextOutputter(&runner.result(), std::cout));
	#elif TEST_MODE == GRAPHICAL_OUTPUT_MODE
			runner.setOutputter(new ***Outputter(&runner.result(), std::cout));
	#elif TEST_MODE == COMPILER_OUTPUT_MODE
			runner.setOutputter(new CompilerOutputter(&runner.result(), std::cout));
	#endif

	bool ret = runner.run() ? 0 : 1;

	getchar();
	return ret;
}
