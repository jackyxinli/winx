/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/text/Append.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Append.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_APPEND_H
#define STDEXT_TEXT_APPEND_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

#ifndef STD_VECTOR_H
#include "../../std/vector.h"
#endif

#ifndef STD_STRING_H
#include "../../std/string.h"
#endif

#ifndef NS_STDEXT_TEXT
#define NS_STDEXT_TEXT				NS_STDEXT::ns_text
#define NS_STDEXT_TEXT_BEGIN		namespace NS_STDEXT { namespace ns_text {
#define NS_STDEXT_TEXT_END			} }
#endif

NS_STDEXT_TEXT_BEGIN

// -------------------------------------------------------------------------

template <class CharT, class AllocT>
inline void winx_call clear(std::vector<CharT, AllocT>& dest)
{
	dest.clear();
}

template <class CharT, class Tr, class AllocT>
inline void winx_call clear(std::basic_string<CharT, Tr, AllocT>& dest)
{
	dest.erase();
}

inline void winx_call clear(FILE& dest)
{
}

// -------------------------------------------------------------------------

template <class CharT, class AllocT>
inline void winx_call append(std::vector<CharT, AllocT>& dest, const CharT* val, const CharT* valEnd)
{
	dest.insert(dest.end(), val, valEnd);
}

template <class CharT, class AllocT>
inline void winx_call append(std::vector<CharT, AllocT>& dest, const size_t count, const CharT val)
{
	dest.insert(dest.end(), count, val);
}

template <class CharT, class AllocT>
inline void winx_call append(std::vector<CharT, AllocT>& dest, const CharT val)
{
	dest.push_back(val);
}

// -------------------------------------------------------------------------

template <class Tr, class AllocT>
inline void winx_call append(std::basic_string<char, Tr, AllocT>& dest, const char* val)
{
	dest.append(val);
}

template <class Tr, class AllocT>
inline void winx_call append(std::basic_string<wchar_t, Tr, AllocT>& dest, const wchar_t* val)
{
	dest.append(val);
}

template <class CharT, class Tr, class AllocT>
inline void winx_call append(std::basic_string<CharT, Tr, AllocT>& dest, const CharT* val, const CharT* valEnd)
{
	dest.append(val, valEnd);
}

template <class CharT, class Tr, class AllocT>
inline void winx_call append(std::basic_string<CharT, Tr, AllocT>& dest, const size_t count, const CharT val)
{
	dest.append(count, val);
}

template <class CharT, class Tr, class AllocT>
inline void winx_call append(std::basic_string<CharT, Tr, AllocT>& dest, const CharT val)
{
	dest.append(1, val);
}

// -------------------------------------------------------------------------

template <class CharT>
inline void winx_call append(FILE& fp, const CharT* val, const CharT* valEnd)
{
	fwrite(val, sizeof(CharT), valEnd - val, &fp);
}

inline void winx_call append(FILE& fp, const char val)
{
	putc(val, &fp);
}

// -------------------------------------------------------------------------

template <class StringT>
inline void winx_call append(StringT& dest, const TempString<char>& val)
{
	append(dest, val.begin(), val.end());
}

template <class StringT>
inline void winx_call append(StringT& dest, const TempString<wchar_t>& val)
{
	append(dest, val.begin(), val.end());
}

// -------------------------------------------------------------------------

NS_STDEXT_TEXT_END

#endif /* STDEXT_TEXT_APPEND_H */
