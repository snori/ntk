#define NTK_BUILD
#include "ntk/application/clipboard.h"


#include <ntk/support/utility.h>


namespace ntk {


//########################################################
// public methods

Clipboard::Clipboard(HWND hwnd)
:	m_hwnd(hwnd)
,	m_status(false)
{
}

Clipboard::~Clipboard()
{
	if(is_locked())
		unlock();
}

Message*
Clipboard::data()
{
	if(is_locked() == false, m_status == false)
		return NULL;

	return &m_data;
}

const Message*
Clipboard::data() const
{
	if(is_locked() == false, m_status == false)
		return NULL;

	return &m_data;
}

status_t
Clipboard::clear()
{
	if(is_locked() == false || m_status == false)
		return false;

	m_status = to_bool(EmptyClipboard());

	return m_status;
}

status_t
Clipboard::commit()
{
	if(is_locked() == false)
		return false;

	int type;
	status_t sts = m_data.find_int("type", &type);
	if(sts == false)
		return sts;

	if(type != CF_BITMAP)
		return false;

	HANDLE handle;
	sts = m_data.find_pointer("data", &handle);
	if(sts == false)
		return sts;

	SetClipboardData(type, handle);

	return true;
}

status_t
Clipboard::revert()
{
	return false;
}

bool
Clipboard::lock()
{
	bool ret = m_locker.lock();

	m_status = to_bool(OpenClipboard(m_hwnd));

	return ret;
}

void
Clipboard::unlock()
{
	CloseClipboard();
	m_locker.unlock();
}

bool
Clipboard::is_locked() const
{
	return m_locker.is_locked();
}

String
Clipboard::name() const
{
	return "no name";
}


//########################################################
}// namespace ntk
