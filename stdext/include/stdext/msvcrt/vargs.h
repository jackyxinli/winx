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
// Module: stdext/msvcrt/vargs.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-2-7 15:25:27
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __STDEXT_MSVCRT_VARGS_H__
#define __STDEXT_MSVCRT_VARGS_H__

#ifndef _INC_LIMITS
#include <limits.h>
#endif

// =========================================================================
// _alloca hellper

#ifndef _alloca_array
#define _alloca_array(Type, count)	((Type*)_alloca(sizeof(Type)*(count)))
#endif

#ifndef _alloca_type
#define _alloca_type(Type)		((Type*)_alloca(sizeof(Type)))
#endif

// =========================================================================

#if (INT_MAX == 2147483647)
#define WINX_CB_PTR				4
#elif (INT_MAX == 9223372036854775807i64)
#define WINX_CB_PTR				8
#endif

#define _WINX_CB_EBP			WINX_CB_PTR
#define _WINX_CB_CALLFRAME		(WINX_CB_PTR+WINX_CB_PTR)

// -------------------------------------------------------------------------

#if defined(X_CC_VC6)

//
// vargs template fuction: 1 extra parameter, n parameters
//
#define _WINX_VARGS_TFUNC_EP1(RetType, func, TempParaT, ExParaT, ParaT, call, cbExPara)	\
	template <class TempParaT>												\
	RetType __cdecl func(ExParaT exarg, ParaT& a1, ParaT& a2)				\
	{																		\
		ParaT** args;														\
		__asm { lea eax, [ebp+cbExPara+_WINX_CB_CALLFRAME+_WINX_CB_EBP] }	\
		__asm { mov args, eax }												\
		return call(exarg, args, 2);										\
	}																		\
	template <class TempParaT>												\
	RetType __cdecl func(ExParaT exarg, ParaT& a1, ParaT& a2, ParaT& a3)	\
	{																		\
		ParaT** args;														\
		__asm { lea eax, [ebp+cbExPara+_WINX_CB_CALLFRAME+_WINX_CB_EBP] }	\
		__asm { mov args, eax }												\
		return call(exarg, args, 3);										\
	}																		\
	template <class TempParaT>												\
	RetType __cdecl func(ExParaT exarg, ParaT& a1, ParaT& a2, ParaT& a3, ParaT& a4)	\
	{																		\
		ParaT** args;														\
		__asm { lea eax, [ebp+cbExPara+_WINX_CB_CALLFRAME+_WINX_CB_EBP] }	\
		__asm { mov args, eax }												\
		return call(exarg, args, 4);										\
	}																		\
	template <class TempParaT>												\
	RetType __cdecl func(ExParaT exarg, ParaT& a1, ParaT& a2, ParaT& a3, ParaT& a4, ParaT& a5) \
	{																		\
		ParaT** args;														\
		__asm { lea eax, [ebp+cbExPara+_WINX_CB_CALLFRAME+_WINX_CB_EBP] }	\
		__asm { mov args, eax }												\
		return call(exarg, args, 5);										\
	}																		\
	template <class TempParaT>												\
	RetType __cdecl func(ExParaT exarg, ParaT& a1, ParaT& a2, ParaT& a3, ParaT& a4, ParaT& a5, ParaT& a6) \
	{																		\
		ParaT** args;														\
		__asm { lea eax, [ebp+cbExPara+_WINX_CB_CALLFRAME+_WINX_CB_EBP] }	\
		__asm { mov args, eax }												\
		return call(exarg, args, 6);										\
	}

#else

//
// vargs template fuction: 1 extra parameter, n parameters
//
#define _WINX_VARGS_TFUNC_EP1(RetType, func, TempParaT, ExParaT, ParaT, call, cbExPara)	\
	template <class TempParaT>												\
	RetType __cdecl func(ExParaT exarg, ParaT& a1, ParaT& a2)				\
	{																		\
		ParaT** args = _alloca_array(ParaT*, 2);							\
		args[0] = &a1;														\
		args[1] = &a2;														\
		return call(exarg, args, 2);										\
	}																		\
	template <class TempParaT>												\
	RetType __cdecl func(ExParaT exarg, ParaT& a1, ParaT& a2, ParaT& a3)	\
	{																		\
		ParaT** args = _alloca_array(ParaT*, 3);							\
		args[0] = &a1;														\
		args[1] = &a2;														\
		args[2] = &a3;														\
		return call(exarg, args, 3);										\
	}																		\
	template <class TempParaT>												\
	RetType __cdecl func(ExParaT exarg, ParaT& a1, ParaT& a2, ParaT& a3, ParaT& a4)	\
	{																		\
		ParaT** args = _alloca_array(ParaT*, 4);							\
		args[0] = &a1;														\
		args[1] = &a2;														\
		args[2] = &a3;														\
		args[3] = &a4;														\
		return call(exarg, args, 4);										\
	}																		\
	template <class TempParaT>												\
	RetType __cdecl func(ExParaT exarg, ParaT& a1, ParaT& a2, ParaT& a3, ParaT& a4, ParaT& a5) \
	{																		\
		ParaT** args = _alloca_array(ParaT*, 5);							\
		args[0] = &a1;														\
		args[1] = &a2;														\
		args[2] = &a3;														\
		args[3] = &a4;														\
		args[4] = &a5;														\
		return call(exarg, args, 5);										\
	}																		\
	template <class TempParaT>												\
	RetType __cdecl func(ExParaT exarg, ParaT& a1, ParaT& a2, ParaT& a3, ParaT& a4, ParaT& a5, ParaT& a6) \
	{																		\
		ParaT** args = _alloca_array(ParaT*, 6);							\
		args[0] = &a1;														\
		args[1] = &a2;														\
		args[2] = &a3;														\
		args[3] = &a4;														\
		args[4] = &a5;														\
		args[5] = &a6;														\
		return call(exarg, args, 6);										\
	}

#endif

// -------------------------------------------------------------------------

#define WINX_VARGS_TFUNC_EP1_REF(RetType, func, TempParaT, ParaT, call, cbExPara)	\
	_WINX_VARGS_TFUNC_EP1(RetType, func, TempParaT, TempParaT&, ParaT, call, cbExPara)

#define WINX_VARGS_TFUNC_EP1_CREF(RetType, func, TempParaT, ParaT, call, cbExPara)	\
	_WINX_VARGS_TFUNC_EP1(RetType, func, TempParaT, const TempParaT&, ParaT, call, cbExPara)

// =========================================================================
// $Log: $

#endif /* __STDEXT_MSVCRT_VARGS_H__ */
