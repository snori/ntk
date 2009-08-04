/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: supportutility.cpp,v 1.6 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/support/supportutility.h"


#include <boost/scoped_array.hpp>
#include <ntk/windows/windows.h>


namespace ntk {


//########################################################
// functions

void
init_random(int seed)
{
	if(seed == -1)
		seed = GetTickCount();//timeGetTime();

	srand(static_cast<uint>(seed));
}

float
random()
{
	return (float)rand() / (float)RAND_MAX;
}

void
print(const char_t* str)
{
#ifdef NTK_DEBUG
	OutputDebugString(str);
#else
	message_box(str, "Alert", 0);// 保留:NTerminal ができたら それを使ってデバッグ用のターミナルを作りそこへ出力する予定
#endif
}

void
print(const String& message)
{
	print(message.c_str());// print to Output window
}

#if 0
void
print(const char_t* format, ...)
{
	enum{ BUF_SIZE = 1024 };
	char_t buf[1024];

	va_list args;
	va_start(args, format);
	_vsprintf(buf, format, args);
	va_end(args);

	print(buf);
}
#endif

int
message_box(const String& title, const String& message, uint flags)
{
	return MessageBox(NULL, message.c_str(), title.c_str(), flags);
}


//--------------------------------------------------------
// w/mb char conv


inline WString
mb2wc(const char* str)
{
	size_t len = strlen(str);
	boost::scoped_array<wchar_t> buf(new wchar_t[len +1]);

	MultiByteToWideChar(
		CP_ACP, MB_PRECOMPOSED, str, len, &buf[0], len);

	buf[len] = '\0';

	return &buf[0];
}

WString
mb2wc(const AString& str)
{
	return mb2wc(str.c_str());
}

inline AString
wc2mb(const wchar_t* str)
{
	size_t len = wcslen(str);
	boost::scoped_array<char> buf(new char[len +1]);

	WideCharToMultiByte(
		CP_ACP, WC_COMPOSITECHECK, str, len, &buf[0], len, NULL, NULL);

	buf[len] = '\0';

	return &buf[0];
}

AString
wc2mb(const WString& str)
{
	return wc2mb(str.c_str());
}


//########################################################
}// namespace ntk
