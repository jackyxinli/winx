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
// Module: TestArchive.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:57:28
// 
// $Id: TestArchive.cpp,v 1.8 2006/12/03 07:52:55 xushiwei Exp $
// -----------------------------------------------------------------------*/

#define STD_UNITTEST
#include <stdext/Archive.h>

// -------------------------------------------------------------------------

using namespace std;

WINX_AUTORUN_CLASS(TestRecord, std::ErrorLog);
WINX_AUTORUN_CLASS(TestMemArchive, std::ErrorLog);
WINX_AUTORUN_CLASS(TestStdioArchive, std::ErrorLog);
WINX_AUTORUN_CLASS(TestPosixArchive, std::ErrorLog);

// -------------------------------------------------------------------------
// $Log: TestArchive.cpp,v $
//

