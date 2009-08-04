/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: translatorroster.cpp,v 1.2 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/translation/translatorroster.h"


#include <boost/scoped_ptr.hpp>


namespace ntk {


//########################################################
// public 


TranslatorRoster::TranslatorRoster()
{
}

TranslatorRoster::~TranslatorRoster()
{
}

status_t
TranslatorRoster::translate(
		PositionIO* destination,
		const PositionIO& source,
		const Translator::Info& source_info,
		const Message& source_extension,
		uint,
		uint,
		const String&)
{
	return st::OK;
}

status_t
TranslatorRoster::add_translator(Translator* translator)
{
	return st::OK;
}

status_t
TranslatorRoster::add_translators(const Path& path)
{
	return st::OK;
}

TranslatorRoster*
TranslatorRoster::default_roster()
{
	static boost::scoped_ptr<TranslatorRoster> roster;
	if(! roster)
		roster.reset(new TranslatorRoster);

	return roster.get();
}


//########################################################
}// namespace ntk
