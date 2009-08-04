#include "document.h"


#include <ntk/application/message.h>


//########################################################


Document::Document()
:	m_modified(false)
,	m_path()
{
}

Document::Document(const NPath& path)
:	m_modified(false)
,	m_path(path)
{
}

Document::~Document()
{
}

void
Document::set_modification_flag(bool flag)
{
	if(flag == m_modified)
		return;

	m_modified = flag;

	NMessage message(MODIFICATION_FLAG_CHANGED);
	message.add_bool("flag", m_modified);

	send_message_to_observers(message);
}

void
Document::set_path(const NPath& path)
{
	if(path == m_path || path.init_check() == false)
		return;

	m_path = path;

	NMessage message(PATH_CHANGED);
	message.add_string("path", m_path.as_string());
	message.add_string("parent", m_path.parent());
	message.add_string("leaf", m_path.leaf());

	send_message_to_observers(message);
}


//########################################################
