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
// Module: TestProperty.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:57:28
// 
// $Id: TestProperty.cpp,v 1.8 2006/12/03 07:52:55 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <stdext/Basic.h>

// -------------------------------------------------------------------------
// testProperty

class FooProp
{
	WINX_THISCLASS(FooProp);

private:
	typedef std::string string;
	string m_strText;

public:
	DEFINE_PROPERTY_RW(string, Text, get_Text, put_Text);
	DEFINE_PROPERTY_RO(string, TextRO, get_Text);
	DEFINE_PROPERTY_WO(string, TextWO, put_Text);

	void put_Text(const string& value) {
		m_strText = value;
	}
	string get_Text() const {
		return m_strText;
	}
};

class FooAlias
{
	WINX_THISCLASS(FooAlias);

private:
	typedef std::string string;
	string m_strText;

public:
	DEFINE_ALIAS_RW(string, Text, m_strText);
	DEFINE_ALIAS_RO(string, TextRO, m_strText);
	DEFINE_ALIAS_WO(string, TextWO, m_strText);
};

template <class LogT>
class TestProperty : public TestCase
{
	WINX_TEST_SUITE(TestProperty);
		WINX_TEST(testProperty);
		WINX_TEST(testAlias);
	WINX_TEST_SUITE_END();
public:
	void testProperty(LogT& log)
	{
		FooProp foo;
		foo.Text = "ABC";
		AssertExp(foo.Text == "ABC");
		AssertExp(foo.Text == (std::string)foo.TextRO);

		foo.TextWO = "123";
		AssertExp(foo.Text == "123");
		AssertExp(foo.TextRO == (std::string)foo.Text);
	}

	void testAlias(LogT& log)
	{
		FooAlias foo;
		foo.Text = "ABC";
		AssertExp(foo.Text == "ABC");
		AssertExp(foo.Text == (std::string)foo.TextRO);

		foo.TextWO = "123";
		AssertExp(foo.Text == "123");
		AssertExp(foo.TextRO == (std::string)foo.Text);
	}
};

WINX_AUTORUN_CLASS(TestProperty, std::ErrorLog);

// -------------------------------------------------------------------------
// $Log: TestProperty.cpp,v $
//
