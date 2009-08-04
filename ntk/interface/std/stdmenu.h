/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: stdmenu.h,v 1.6 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_STD_STDMENU_H__
#define __NTK_INTERFACE_STD_STDMENU_H__


#ifndef __NTK_DEFS_H__
#include <ntk/Defs.h>
#endif

#ifndef __NTK_INTERFACE_VIEW_H__
#include <ntk/interface/view.h>
#endif


namespace ntk {
namespace standard {


class Menu;
class MenuItem;


class MenuOperator {
public:
	//
	// methods
	//
	NtkExport MenuOperator(HMENU hmenu);
	NtkExport virtual ~MenuOperator();

	NtkExport virtual bool add_item(const Menu& submenu);
	NtkExport virtual bool add_item(const MenuItem& item);

	//
	// accessors and manipulators
	//
	NtkExport virtual HMENU handle() const;

private:
	//
	// data
	//
	HMENU m_handle;

};// class MenuOperator


class Menu : public MenuOperator {
public:
	//
	// methods
	//
	NtkExport Menu(const String& label);
	NtkExport virtual ~Menu();

	NtkExport const String& label() const;

private:
	//
	// data
	//
	String m_label;

};// class Menu


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Menu menu_t;
#endif


} namespace Standard = standard;


typedef standard::Menu StdMenu;

#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef StdMenu std_menu_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::StdMenu NStdMenu;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::StdMenu ntk_std_menu;
	#endif
#endif


#endif//EOH
