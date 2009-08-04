/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: zfile.h,v 1.2 2003/06/16 11:28:55 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_STORAGE_ZFILE_H__
#define __NTK_STORAGE_ZFILE_H__


#include <zlib.h>

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_DATAIO_H__
#include <ntk/support/dataio.h>
#endif

#ifndef __NTK_STORAGE_NODE_H__
#include <ntk/storage/node.h>
#endif


#include <ntk/storage/file.h>
#define NtkExport_


namespace ntk {


class ZFILE : public Node, public DataIO {
public:
	//
	// types
	//
	typedef Status<FileStatus> status_t;

	//
	// methods
	//
	NtkExport_ ZFILE();
	NtkExport_ ZFILE(const String& path, const String& access_mode);
	NtkExport_ ZFILE(const Entry& entry, const String& access_mode);
	NtkExport_ ZFILE(const Directory& dir, const String& path, const String& access_mode);
	NtkExport_ virtual ~ZFILE();

	NtkExport_ virtual status_t set_to(const String& path, const String& access_mode);
	NtkExport_ virtual status_t set_to(const Entry& entry, const String& access_mode);
	NtkExport_ virtual status_t set_to(const Directory& dir, const String& path, const String& access_path);
	NtkExport_ virtual void unset();

	NtkExport_ virtual status_t init_check() const;

	NtkExport_ virtual Size read(void* buffer, Size num_bytes);
	NtkExport_ virtual Size write(const void* buffer, Size num_bytes);

	//
	// accessors
	//
	NtkExport_ virtual gzFile fp() const;
	NtkExport_ virtual const String& access_mode() const;

protected:
	//
	// methods
	//
	bool is_valid() const {return m_fp != NULL;}

private:
	//
	// data
	//
	gzFile m_fp;
	String m_access_mode;

	//
	// methods
	//

	// hidden methods
	status_t set_to(const String& path);
	status_t set_to(const Entry& entry);
	status_t set_to(const Directory& dir, const String& path);

	//
	// functions
	//
	status_t set_to_(const String& access_mode);

};// class ZFILE


} namespace Ntk = ntk;


#endif//EOH