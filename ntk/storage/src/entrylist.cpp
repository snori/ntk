/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: entrylist.cpp,v 1.4 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/storage/entrylist.h"


namespace ntk {


//########################################################
// public methods

String
EntryList::status::default_message(int what)
{
	switch(what)
	{
	case ENTRY_NOT_FOUND_ERROR:
		return "ƒGƒ“ƒgƒŠ‚ªŒ©‚Â‚©‚è‚Ü‚¹‚ñ‚Å‚µ‚½B";
	
	default:
		return st::default_message(what);
	}
}


//########################################################
// public methods

Entry
EntryList::next_entry(bool traverse, status_t* status)
{
	status_t sts_, &sts = status ? *status : sts_;

	Entry entry;
	sts = get_next_entry(&entry, traverse);

	return entry;
}

Entry::Ref
EntryList::next_ref(status_t* status)
{
	status_t sts_, &sts = status ? *status : sts_;

	Entry::Ref entry_ref;
	sts = get_next_ref(&entry_ref);

	return entry_ref;
}


//########################################################
}// namespace ntk
