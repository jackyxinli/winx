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
// Module: tpl/OpType.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 10:08:49
// 
// $Id: OpType.h,v 1.4 2006/12/03 07:51:57 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef TPL_OPTYPE_H
#define TPL_OPTYPE_H

#ifndef TPL_BASIC_H
#include "Basic.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// TPL_OPTYPE_XXX

#define TPL_OPTYPE_EXCLAMATION		0x01,		/* [!] */
#define TPL_OPTYPE_QUESTION			0x02,		/* [?] */
#define TPL_OPTYPE_SHARP			0x04,		/* [#] */
#define TPL_OPTYPE_UNDERLINE		0x08		/* underline[_] */
#define TPL_OPTYPE_PARENTHESIS_L	0x10,		/* [(] */
#define TPL_OPTYPE_PARENTHESIS_R	0x20,		/* [)] */
#define TPL_OPTYPE_ADD				0x40		/* [+] */
#define TPL_OPTYPE_SUB				0x80		/* [-], dash/hyphen */
#define TPL_OPTYPE_MUL				0x100		/* [*] */
#define TPL_OPTYPE_DIV				0x200		/* [/] */
#define TPL_OPTYPE_LT				0x400		/* [<] */
#define TPL_OPTYPE_GT				0x800		/* [>] */
#define TPL_OPTYPE_EQ				0x1000		/* [=] */
#define TPL_OPTYPE_RDIV				0x2000		/* [\\], right-division, anti-slash */
#define TPL_OPTYPE_DOT				0x4000		/* [.] */
#define TPL_OPTYPE_COLON			0x8000		/* [:], colon */
#define TPL_OPTYPE_PERCENT			0x10000		/* [%] */
#define TPL_OPTYPE_AND				0x20000		/* [&] */
#define TPL_OPTYPE_OR				0x40000		/* [|] */
#define TPL_OPTYPE_XOR				0x80000		/* [^] */
#define TPL_OPTYPE_SQUARE_BRACKET_L	0x100000	/* [\[] */
#define TPL_OPTYPE_SQUARE_BRACKET_R	0x200000	/* [\]] */
#define TPL_OPTYPE_BRACE_L			0x400000	/* [{] */
#define TPL_OPTYPE_BRACE_R			0x800000	/* [}] */
#define TPL_OPTYPE_SEMICOLON		0x1000000	/* [;] */
#define TPL_OPTYPE_COMMA			0x2000000	/* [,] */
#define TPL_OPTYPE_REVERSE			0x4000000	/* [~] */
#define TPL_OPTYPE_DOLLAR			0x8000000	/* [$] */
#define TPL_OPTYPE_QUOTE			0x10000000	/* ["] */
#define TPL_OPTYPE_SINGLE_QUOTE		0x20000000	/* ['] */
#define TPL_OPTYPE_AT				0x40000000,	/* [@] */
#define TPL_OPTYPE_MAX_CHAR			128

// -------------------------------------------------------------------------

#define TPL_OPTYPE_IS_(mask, c)												\
	(TPL_OPTYPE_MAX_CHAR > (unsigned)(c) ? ((mask) & data[c]) : 0)

// -------------------------------------------------------------------------
// class OpType

template <class Unused>
struct OpTypeT
{
	static unsigned data[TPL_OPTYPE_MAX_CHAR];

	static unsigned winx_call is(unsigned typeMask, int c) {
		return TPL_OPTYPE_IS_(typeMask, c);
	}
};

template <class Unused>
unsigned OpTypeT<Unused>::data[TPL_OPTYPE_MAX_CHAR] =
{
	0,	//   [0]
	0,	//   [1]
	0,	//   [2]
	0,	//   [3]
	0,	//   [4]
	0,	//   [5]
	0,	//   [6]
	0,	//   [7]
	0,	//   [8]
	0,	//   [9]
	0,	//   [10]
	0,	//   [11]
	0,	//   [12]
	0,	//   [13]
	0,	//   [14]
	0,	//   [15]
	0,	//   [16]
	0,	//   [17]
	0,	//   [18]
	0,	//   [19]
	0,	//   [20]
	0,	//   [21]
	0,	//   [22]
	0,	//   [23]
	0,	//   [24]
	0,	//   [25]
	0,	//   [26]
	0,	//   [27]
	0,	//   [28]
	0,	//   [29]
	0,	//   [30]
	0,	//   [31]
	0,	//   [32]
	TPL_OPTYPE_EXCLAMATION,				// ! [33]
	TPL_OPTYPE_QUOTE,					// " [34]
	TPL_OPTYPE_SHARP,					// # [35]
	TPL_OPTYPE_DOLLAR,					// $ [36]
	TPL_OPTYPE_PERCENT,					// % [37]
	TPL_OPTYPE_AND,						// & [38]
	TPL_OPTYPE_SINGLE_QUOTE,			// ' [39]
	TPL_OPTYPE_PARENTHESIS_L,			// ( [40]
	TPL_OPTYPE_PARENTHESIS_R,			// ) [41]
	TPL_OPTYPE_MUL,						// * [42]
	TPL_OPTYPE_ADD,						// + [43]
	TPL_OPTYPE_COMMA,					// , [44]
	TPL_OPTYPE_SUB,						// - [45]
	TPL_OPTYPE_DOT,						// . [46]
	TPL_OPTYPE_DIV,						// / [47]
	0,	// 0 [48]
	0,	// 1 [49]
	0,	// 2 [50]
	0,	// 3 [51]
	0,	// 4 [52]
	0,	// 5 [53]
	0,	// 6 [54]
	0,	// 7 [55]
	0,	// 8 [56]
	0,	// 9 [57]
	TPL_OPTYPE_COLON,					// : [58]
	TPL_OPTYPE_SEMICOLON,				// ; [59]
	TPL_OPTYPE_LT,						// < [60]
	TPL_OPTYPE_EQ,						// = [61]
	TPL_OPTYPE_GT,						// > [62]
	TPL_OPTYPE_QUESTION,				// ? [63]
	TPL_OPTYPE_AT,						// @ [64]
	0,	// A [65]
	0,	// B [66]
	0,	// C [67]
	0,	// D [68]
	0,	// E [69]
	0,	// F [70]
	0,	// G [71]
	0,	// H [72]
	0,	// I [73]
	0,	// J [74]
	0,	// K [75]
	0,	// L [76]
	0,	// M [77]
	0,	// N [78]
	0,	// O [79]
	0,	// P [80]
	0,	// Q [81]
	0,	// R [82]
	0,	// S [83]
	0,	// T [84]
	0,	// U [85]
	0,	// V [86]
	0,	// W [87]
	0,	// X [88]
	0,	// Y [89]
	0,	// Z [90]
	TPL_OPTYPE_SQUARE_BRACKET_L,	// [ [91]
	TPL_OPTYPE_RDIV,				// \ [92]
	TPL_OPTYPE_SQUARE_BRACKET_R,	// ] [93]
	TPL_OPTYPE_XOR,					// ^ [94]
	TPL_OPTYPE_UNDERLINE,			// _ [95]
	0,	// ` [96]
	0,	// a [97]
	0,	// b [98]
	0,	// c [99]
	0,	// d [100]
	0,	// e [101]
	0,	// f [102]
	0,	// g [103]
	0,	// h [104]
	0,	// i [105]
	0,	// j [106]
	0,	// k [107]
	0,	// l [108]
	0,	// m [109]
	0,	// n [110]
	0,	// o [111]
	0,	// p [112]
	0,	// q [113]
	0,	// r [114]
	0,	// s [115]
	0,	// t [116]
	0,	// u [117]
	0,	// v [118]
	0,	// w [119]
	0,	// x [120]
	0,	// y [121]
	0,	// z [122]
	TPL_OPTYPE_BRACE_L,	// { [123]
	TPL_OPTYPE_OR,		// | [124]
	TPL_OPTYPE_BRACE_R,	// } [125]
	TPL_OPTYPE_REVERSE,	// ~ [126]
	0,	// del [127]
};

typedef OpTypeT<void> OpType;

// -------------------------------------------------------------------------
// class IsOpType

template <unsigned typeMask>
struct IsOpType {
	unsigned TPL_CALL operator()(int c) const {
		return OpType::is(typeMask, c);
	}
};

// -------------------------------------------------------------------------
// $Log: OpType.h,v $

NS_TPL_END

#endif /* TPL_OPTYPE_H */
