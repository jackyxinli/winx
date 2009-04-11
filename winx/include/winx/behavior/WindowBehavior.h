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
// Module: winx/behavior/WindowBehavior.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-2 20:42:59
// 
// $Id: WindowBehavior.h,v 1.2 2006/11/28 12:45:51 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef WINX_BEHAVIOR_WINDOWBEHAVIOR_H
#define WINX_BEHAVIOR_WINDOWBEHAVIOR_H

#ifndef WINX_WIN_BASIC_H
#include "../win/Basic.h"
#endif

NS_WINX_BEGIN

// =========================================================================
// IWindowBehavior

struct IWindowBehavior
{
	virtual BOOL winx_msg PreHandleMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) = 0;
	
	virtual VOID winx_msg EnableBehavior() = 0;
	virtual VOID winx_msg DisableBehavior() = 0;
};

// =========================================================================
// WINX_BEHAVIOR - Window Behavior Support

#define WINX_BEHAVIOR_EX(Behavior)											\
	public:																	\
		typedef Behavior BehaviorClass;										\
		enum { EnableWindowBehavior = TRUE }

#define WINX_BEHAVIOR()														\
	WINX_BEHAVIOR_EX(BehaviorImplClass)

#define WINX_BEHAVIOR_SWITCH()												\
	WINX_BEHAVIOR_EX(winx::BehaviorSwitch)

// -------------------------------------------------------------------------
// class WindowBehavior - Window Behavior Support

template <class BehaviorClass>
class WindowBehavior : public IWindowBehavior
{
public:
	typedef BehaviorClass BehaviorImplClass;
	
public:
	BOOL winx_msg PreHandleMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		return FALSE;
	}

	VOID winx_msg EnableBehavior() {} // 可以默认Enable
	VOID winx_msg DisableBehavior() {} // 可以不支持
};

// =========================================================================
// BehaviorPermit - 可禁止的Behavior

template <class BehaviorClass>
class BehaviorPermit : public BehaviorClass
{
public:
	typedef BehaviorPermit<BehaviorClass> BehaviorImplClass;
	
private:
	BOOL m_fDisable;
	
public:
	BOOL winx_msg PreHandleMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		if (m_fDisable)
			return FALSE;
		return BehaviorClass::PreHandleMessage(hWnd, message, wParam, lParam, pResult);
	}
	
public:
	BehaviorPermit() : m_fDisable(FALSE) {}
	
	VOID winx_call EnableBehavior(BOOL fEnable)
	{
		if (fEnable)
			EnableBehavior();
		else
			DisableBehavior();
	}
	
	VOID winx_call EnableBehavior()
	{
		if (m_fDisable)
		{
			BehaviorClass::EnableBehavior();
			m_fDisable = FALSE;
		}
	}
	
	VOID winx_call DisableBehavior()
	{
		if (!m_fDisable)
		{
			BehaviorClass::DisableBehavior();
			m_fDisable = TRUE;
		}
	}
};

// =========================================================================
// class BehaviorSwitch

class BehaviorSwitch : public WindowBehavior<BehaviorSwitch>
{
private:
	IWindowBehavior* m_currBehavior;
	
public:
	BOOL winx_msg PreHandleMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		if (NULL == m_currBehavior)
			return FALSE;
		return m_currBehavior->PreHandleMessage(hWnd, message, wParam, lParam, pResult);
	}
	
public:
	BehaviorSwitch() : m_currBehavior(NULL) {}
	
	VOID winx_call SwitchBehavior(IWindowBehavior* newBehavior)
	{
		if (m_currBehavior)
			m_currBehavior->DisableBehavior();
		m_currBehavior = newBehavior;
		if (m_currBehavior)
			m_currBehavior->EnableBehavior();
	}
};

// =========================================================================
// class DefaultBehavior - 无行为

class DefaultBehavior : public WindowBehavior<DefaultBehavior>
{
	// nothing to do...
};

// =========================================================================
// class Moveless - 不可移动的窗口

class Moveless : public WindowBehavior<Moveless>
{
private:
	BOOL winx_msg PreWindowPosChanging(
		HWND hWnd, WINDOWPOS& wndPos)
	{
		wndPos.flags |= SWP_NOMOVE;
		return FALSE;
	}
	
public:
	BOOL winx_msg PreHandleMessage(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		switch (message)
		{
		case WM_WINDOWPOSCHANGING:
			return PreWindowPosChanging(hWnd, *(LPWINDOWPOS)lParam);
		}
		return FALSE;
	}
};

// =========================================================================
// $Log: WindowBehavior.h,v $
// Revision 1.2  2006/11/28 12:45:51  xushiwei
// simplify WindowBehavior interface
//
// Revision 1.1  2006/09/03 04:32:56  xushiwei
// WINX-Core: Behavior(BehaviorPermit, BehaviorSwitch, AutoHidden, etc)
// WINX-Extension: PropertySheet, PropertyPage
//

NS_WINX_END

#endif /* WINX_BEHAVIOR_WINDOWBEHAVIOR_H */
