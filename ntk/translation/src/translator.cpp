/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: translator.cpp,v 1.3 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/translation/translator.h"
//--------------------------------------------------------


#include <ntk/support/dataio.h>


namespace ntk {


//########################################################
// public methods

Translator::Translator(const String& name, const String& info, const Version& version)
:	m_name(name)
,	m_info(info)
,	m_version(version)
{
}

Translator::~Translator()
{
}

status_t
Translator::translate(PositionIO* input, PositionIO* output)
{
	return st::OK;
}

//--------------------------------------------------------
// public accessors

String
Translator::name() const
{
	return "NULL Translator";
}

String
Translator::info() const
{
	return "NULL Translator";
}

Version
Translator::version() const
{
	return Version(1);
}


//########################################################
}// namespace ntk
