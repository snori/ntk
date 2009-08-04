/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: stdmenuitem.h,v 1.5 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_STD_STDMENUITEM_H__
#define __NTK_INTERFACE_STD_STDMENUITEM_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_APPLICATION_HANDLER_H__
#include <ntk/application/handler.h>
#endif

#ifndef __NTK_APPLICATION_INVOKER_H__
#include <ntk/application/invoker.h>
#endif


namespace ntk {
namespace standard {


class Menu;

class MenuItem : public Handler {//, public Invoker {
public:
	//
	// methods
	//
	NtkExport MenuItem(
		const String& label,
		uint id,
		uint shortcut = 0,
		uint modifiers = 0);
	NtkExport virtual ~MenuItem();

	//
	// accessors
	//
	NtkExport virtual String label() const;
	NtkExport virtual uint id() const;

private:
	//
	// data
	//
	uint m_id;

};// class MenuItem


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef MenuItem menu_item_t;
#endif


} namespace Standard = standard;


typedef standard::MenuItem StdMenuItem;

#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef StdMenuItem std_menu_item_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::StdMenuItem NStdMenuItem;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::StdMenuItem ntk_std_menu_item;
	#endif
#endif


#endif//EOH
