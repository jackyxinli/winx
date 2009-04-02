/*
Filename: AtlScintilla.h
Version: 2.0
Description: Defines an easy wrapper for the Scintilla control,to be used with ATL/WTL projects.
Date: 06/01/2006

Copyright (c) 2005/2006 by Gilad Novik.  
Copyright (c) 2006 by Reece Dunn.

License Agreement (zlib license)
-------------------------
This software is provided 'as-is',without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications,and to alter it and redistribute it
freely,subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product,an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such,and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

* If you use this code in a product,I would appreciate a small email from.

Gilad Novik (Web: http://gilad.gsetup.com,Email: gilad@gsetup.com)
*/

#pragma once
#include <Platform.h>
#include <Scintilla.h>
#include <atlfile.h>

#pragma comment(lib,"Imm32.lib")

template<class TBase>
class CScintillaWindowT : public TBase
{
public:
	static LPCTSTR GetWndClassName()
	{
		return _T("Scintilla");
	}

	/** @name Text retrieval and modification */
	//@{

	int GetText(LPSTR szText,int nLength) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETTEXT,nLength,(LPARAM)szText);
	}

	void SetText(LPCSTR szText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETTEXT,0,(LPARAM)szText);
	}

	void SetSavePoint()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETSAVEPOINT,0,0L);
	}

	int GetLine(int nLine,LPSTR szText) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETLINE,nLine,(LPARAM)szText);
	}

	int GetLineLength(int nLine) const
	{
		return GetLine(nLine,NULL);
	}

	void ReplaceSel(LPCSTR szText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_REPLACESEL,0,(LPARAM)szText);
	}

	bool GetReadOnly() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETREADONLY,0,0L)!=0;
	}

	void SetReadOnly(bool bReadOnly)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETREADONLY,bReadOnly,0L);
	}

	int GetTextRange(TextRange & trRange) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETTEXTRANGE,0,(LPARAM)&trRange);
	}

	void Allocate(int nBytes)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_ALLOCATE,nBytes,0L);
	}

	void AddText(LPCSTR szText,int nLength)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_ADDTEXT,nLength,(LPARAM)szText);
	}

	void AddStyledText(LPCSTR szText,int nLength)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_ADDSTYLEDTEXT,nLength,(LPARAM)szText);
	}

	void AppendText(LPCSTR szText,int nLength)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_APPENDTEXT,nLength,(LPARAM)szText);
	}

	void InsertText(unsigned int nPosition,LPCSTR szText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_INSERTTEXT,nPosition,(LPARAM)szText);
	}

	void ClearAll()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CLEARALL,0,0L);
	}

	void ClearDocumentStyle()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CLEARDOCUMENTSTYLE,0,0L);
	}

	char GetCharAt(unsigned int nPosition) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETCHARAT,nPosition,0L) & 0xFF;
	}

	int GetStyleAt(unsigned int nPosition) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETSTYLEAT,nPosition,0L);
	}

	int GetStyledText(TextRange & trRange) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETSTYLEDTEXT,0,(LPARAM)&trRange);
	}

	int GetStyledText(LPSTR szText,long nFirst,long nLast) const
	{
		TextRange trRange={{ nFirst,nLast },szText };
		return GetStyledText(trRange);
	}

#if defined(__ATLSTR_H__)
	int GetStyledText(long nMin,long nMax,CStringA& szText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		int nLength=nMax-nMin;
		int nResult=GetStyledText(nMin,nMax,szText.GetBuffer(nLength));
		szText.ReleaseBuffer(nLength);
		return nResult;
	}
#endif

	void SetStyleBits(int nBits)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETSTYLEBITS,nBits,0L);
	}

	int GetStyleBits() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETSTYLEBITS,0,0L);
	}

	int TargetAsUTF8(LPSTR szText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_TARGETASUTF8,0,(LPARAM)szText);
	}

	int EncodeFromUTF8(LPCSTR szUTF8,LPSTR szEncoded)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_ENCODEFROMUTF8,(WPARAM)szUTF8,(LPARAM)szEncoded);
	}

	void SetLengthForEncode(int nBytes)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETLENGTHFORENCODE,nBytes,0L);
	}

	//@}
	/** @name Searching */
	//@{

	int FindText(int nFlags,TextToFind& ttfText) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_FINDTEXT,nFlags,(LPARAM)&ttfText);
	}

	void SearchAnchor()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SEARCHANCHOR,0,0L);
	}

	int SearchNext(int nFlags,LPCSTR szText) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_SEARCHNEXT,nFlags,(LPARAM)szText);
	}

	int SearchPrev(int nFlags,LPCSTR szText) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_SEARCHPREV,nFlags,(LPARAM)szText);
	}

	//@}
	/** @name Search And Replace Using The Target */
	//@{

	unsigned int GetTargetStart() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETTARGETSTART,0,0L);
	}

	void SetTargetStart(unsigned int nPosition)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETTARGETSTART,nPosition,0L);
	}

	unsigned int GetTargetEnd() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETTARGETEND,0,0L);
	}

	void SetTargetEnd(unsigned int nPosition)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETTARGETEND,nPosition,0L);
	}

	void TargetFromSelection()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_TARGETFROMSELECTION,0,0L);
	}

	int GetSearchFlags() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETSEARCHFLAGS,0,0L);
	}

	void SetSearchFlags(int nFlags)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETSEARCHFLAGS,nFlags,0L);
	}

	int SearchInTarget(LPCSTR szText,int nLength)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_SEARCHINTARGET,nLength,(LPARAM)szText);
	}

	int ReplaceTarget(LPCSTR szText,int nLength)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_REPLACETARGET,nLength,(LPARAM)szText);
	}

	int ReplaceTargetRE(LPCSTR szText,int nLength)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_REPLACETARGETRE,nLength,(LPARAM)szText);
	}

	//@}
	/** @name Overtype */
	//@{

	bool GetOvertype() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETOVERTYPE,0,0L)!=0;
	}

	void SetOvertype(bool bOvertype)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETOVERTYPE,bOvertype,0L);
	}

	//@}
	/** @name Cut,Copy And Paste */
	//@{

	void Cut()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CUT,0,0L);
	}

	void Copy()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_COPY,0,0L);
	}

	void Paste()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_PASTE,0,0L);
	}

	bool CanPaste() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_CANPASTE,0,0L)!=0;
	}

	void Clear()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CLEAR,0,0L);
	}

	void CopyRange(unsigned int nFirst,unsigned int nLast)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_COPYRANGE,nFirst,nLast);
	}

	void CopyText(LPCSTR szText,int nLength)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_COPYTEXT,nLength,(LPARAM)szText);
	}

	void SetPasteConvertEndings(bool bConvert)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETPASTECONVERTENDINGS,bConvert,0L);
	}

	bool GetPasteConvertEndings() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETPASTECONVERTENDINGS,0,0L)!=0;
	}

	//@}
	/** @name Error Handling */
	//@{

	int GetStatus() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETSTATUS,0,0L);
	}

	void SetStatus(int nCode)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETSTATUS,nCode,0L);
	}

	//@}
	/** @name Undo/Redo */
	//@{

	void Undo()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_UNDO,0,0L);
	}

	bool CanUndo() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_CANUNDO,0,0L)!=0;
	}

	void Redo()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_REDO,0,0L);
	}

	bool CanRedo() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_CANREDO,0,0L)!=0;
	}

	void EmptyUndoBuffer()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_EMPTYUNDOBUFFER,0,0L);
	}

	void SetUndoCollection(bool bCollectUndo)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETUNDOCOLLECTION,bCollectUndo,0L);
	}

	bool GetUndoCollection() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETUNDOCOLLECTION,0,0L)!=0;
	}

	void BeginUndoAction()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_BEGINUNDOACTION,0,0L);
	}

	void EndUndoAction()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_ENDUNDOACTION,0,0L);
	}

	//@}
	/** @name Selection And Information */
	//@{

	int GetTextLength() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETTEXTLENGTH,0,0L);
	}

	int GetLength() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETLENGTH,0,0L);
	}

	int GetLineCount() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETLINECOUNT,0,0L);
	}

	int GetFirstVisibleLine() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETFIRSTVISIBLELINE,0,0L);
	}

	int LinesOnScreen() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_LINESONSCREEN,0,0L);
	}

	bool GetModify() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETMODIFY,0,0L)!=0;
	}

	void SetSel(unsigned int nStart,unsigned int nEnd)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETSEL,nStart,nEnd);
	}

	void GotoPos(unsigned int nPosition)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_GOTOPOS,nPosition,0L);
	}

	void GotoLine(int nLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_GOTOLINE,nLine,0L);
	}

	unsigned int GetCurrentPos() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETCURRENTPOS,0,0L);
	}

	void SetCurrentPos(unsigned int nPosition)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETCURRENTPOS,nPosition,0L);
	}

	unsigned int GetAnchor() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETANCHOR,0,0L);
	}

	void SetAnchor(unsigned int nAnchor)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETANCHOR,nAnchor,0L);
	}

	void SetSelectionStart(unsigned int nPosition)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETSELECTIONSTART,nPosition,0L);
	}

	unsigned int GetSelectionStart() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETSELECTIONSTART,0,0L);
	}

	void SetSelectionEnd(unsigned int nPosition)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETSELECTIONEND,nPosition,0L);
	}

	unsigned int GetSelectionEnd() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETSELECTIONEND,0,0L);
	}

	void SelectAll()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SELECTALL,0,0L);
	}

	int LineFromPosition(unsigned int nPosition) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_LINEFROMPOSITION,nPosition,0L);
	}

	unsigned int PositionFromLine(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_POSITIONFROMLINE,nLine,0L);
	}

	int GetLineEndPosition(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETLINEENDPOSITION,nLine,0L);
	}

	int LineLength(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_LINELENGTH,nLine,0L);
	}

	int GetColumn(unsigned int nPosition) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETCOLUMN,nPosition,0L);
	}

	int FindColumn(int nLine,int nColumn) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_FINDCOLUMN,nLine,nColumn);
	}

	unsigned int PositionFromPoint(int nX,int nY) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_POSITIONFROMPOINT,nX,nY);
	}

	unsigned int PositionFromPointClose(int nX,int nY) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_POSITIONFROMPOINTCLOSE,nX,nY);
	}

	int PointXFromPosition(unsigned int nPosition) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_POINTXFROMPOSITION,0,nPosition);
	}

	int PointYFromPosition(unsigned int nPosition) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_POINTYFROMPOSITION,0,nPosition);
	}

	void HideSelection(bool bNormal)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_HIDESELECTION,bNormal,0L);
	}

	int GetSelText(LPSTR szText) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETSELTEXT,0,(LPARAM)szText);
	}

	int GetCurLine(LPSTR szText,int nLength) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETCURLINE,nLength,(LPARAM)szText);
	}

	bool SelectionIsRectangle() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_SELECTIONISRECTANGLE,0,0L)!=0;
	}

	int GetSelectionMode() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETSELECTIONMODE,0,0L);
	}

	void SetSelectionMode(int nMode)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETSELECTIONMODE,nMode,0L);
	}

	unsigned int GetLineSelStartPosition(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETLINESELSTARTPOSITION,nLine,0L);
	}

	unsigned int GetLineSelEndPosition(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETLINESELENDPOSITION,nLine,0L);
	}

	void MoveCaretInsideView()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_MOVECARETINSIDEVIEW,0,0L);
	}

	int WordStartPosition(unsigned int nPosition,bool bOnlyWordCharacters)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_WORDSTARTPOSITION,nPosition,bOnlyWordCharacters);
	}

	int WordEndPosition(unsigned int nPosition,bool bOnlyWordCharacters)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_WORDENDPOSITION,nPosition,bOnlyWordCharacters);
	}

	unsigned int PositionBefore(unsigned int nPosition) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_POSITIONBEFORE,nPosition,0L);
	}

	unsigned int PositionAfter(unsigned int nPosition) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_POSITIONAFTER,nPosition,0L);
	}

	int TextWidth(int nStyle,LPCSTR szText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_TEXTWIDTH,nStyle,(LPARAM)szText);
	}

	int TextHeight(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_TEXTHEIGHT,nLine,0L);
	}

	void ChooseCaretX() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CHOOSECARETX,0,0L);
	}

	//@}
	/** @name Scrolling And Auto-Scrolling */
	//@{

	void LineScroll(int nColumns,int nLines)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINESCROLL,nColumns,nLines);
	}

	void ScrollToLine(int nLine)
	{
		LineScroll(0,nLine - LineFromPosition(GetCurrentPos()));
	}

	void ScrollCaret()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SCROLLCARET,0,0L);
	}

	void SetXCaretPolicy(int nPolicy,int nSlop)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETXCARETPOLICY,nPolicy,nSlop);
	}

	void SetYCaretPolicy(int nPolicy,int nSlop)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETYCARETPOLICY,nPolicy,nSlop);
	}

	void SetVisiblePolicy(int nPolicy,int nSlop)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETVISIBLEPOLICY,nPolicy,nSlop);
	}

	bool GetHScrollBar() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETHSCROLLBAR,0,0L)!=0;
	}

	void SetHScrollBar(bool bShow)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETHSCROLLBAR,bShow,0L);
	}

	bool GetVScrollBar() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETVSCROLLBAR,0,0L)!=0;
	}

	void SetVScrollBar(bool bShow)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETVSCROLLBAR,bShow,0L);
	}

	int GetXOffset() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETXOFFSET,0,0L);
	}

	void SetXOffset(int nOffset)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETXOFFSET,nOffset,0L);
	}

	int GetScrollWidth() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETSCROLLWIDTH,0,0L);
	}

	void SetScrollWidth(int nPixelWidth)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETSCROLLWIDTH,nPixelWidth,0L);
	}

	bool GetEndAtLastLine() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETENDATLASTLINE,0,0L)!=0;
	}

	void SetEndAtLastLine(bool bEndAtLastLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETENDATLASTLINE,bEndAtLastLine,0L);
	}

	//@}
	/** @name WhiteSpace */
	//@{

	int GetViewWS() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETVIEWWS,0,0L);
	}

	void SetViewWS(int nMode)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETVIEWWS,nMode,0L);
	}

	void SetWhitespaceFore(bool bUseSetting,COLORREF crForeground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETWHITESPACEFORE,bUseSetting,crForeground);
	}

	void SetWhitespaceBack(bool bUseSetting,COLORREF crBackground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETWHITESPACEBACK,bUseSetting,crBackground);
	}

	//@}
	/** @name Cursor */
	//@{

	int GetCursor() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETCURSOR,0,0L);
	}

	void SetCursor(int nType)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETCURSOR,nType,0L);
	}

	//@}
	/** @name Mouse Capture */
	//@{

	bool GetMouseDownCaptures() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETMOUSEDOWNCAPTURES,0,0L)!=0;
	}

	void SetMouseDownCaptures(bool bCaptures)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETMOUSEDOWNCAPTURES,bCaptures,0L);
	}

	//@}
	/** @name Line Endings */
	//@{

	int GetEOLMode() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETEOLMODE,0,0L);
	}

	void SetEOLMode(int nMode)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETEOLMODE,nMode,0L);
	}

	void ConvertEOLs(int nMode)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CONVERTEOLS,nMode,0L);
	}

	bool GetViewEOL() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETVIEWEOL,0,0L)!=0;
	}

	void SetViewEOL(bool bVisible)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETVIEWEOL,bVisible,0L);
	}

	//@}
	/** @name Styling */
	//@{

	unsigned int GetEndStyled() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETENDSTYLED,0,0L);
	}

	void StartStyling(unsigned int nPosition,int nMask)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STARTSTYLING,nPosition,nMask);
	}

	void SetStyling(int nLength,int nStyle)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETSTYLING,nLength,nStyle);
	}

	void SetStylingEx(int nLength,LPCSTR szStyles)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETSTYLINGEX,nLength,(LPARAM)szStyles);
	}

	int GetLineState(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETLINESTATE,nLine,0L);
	}

	void SetLineState(int nLine,int nState)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETLINESTATE,nLine,nState);
	}

	int GetMaxLineState() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETMAXLINESTATE,0,0L);
	}

	//@}
	/** @name Style Definition */
	//@{

	void StyleResetDefault()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STYLERESETDEFAULT);
	}

	void StyleClearAll()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STYLECLEARALL,0,0L);
	}

	void StyleSetFont(int nStyle,LPCSTR szFontName)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STYLESETFONT,nStyle,(LPARAM)szFontName);
	}

	void StyleSetSize(int nStyle,int nSize)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STYLESETSIZE,nStyle,nSize);
	}

	void StyleSetBold(int nStyle,bool bBold)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STYLESETBOLD,nStyle,bBold);
	}

	void StyleSetItalic(int nStyle,bool bItalic)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STYLESETITALIC,nStyle,bItalic);
	}

	void StyleSetUnderline(int nStyle,bool bUnderline)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STYLESETUNDERLINE,nStyle,bUnderline);
	}

	void StyleSetFore(int nStyle,COLORREF crForeground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STYLESETFORE,nStyle,crForeground);
	}

	void StyleSetBack(int nStyle,COLORREF crBackground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STYLESETBACK,nStyle,crBackground);
	}

	void StyleSetEOLFilled(int nStyle,bool bFilled)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STYLESETEOLFILLED,nStyle,bFilled);
	}

	void StyleSetCharacterSet(int nStyle,int nCharacterSet)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STYLESETCHARACTERSET,nStyle,nCharacterSet);
	}

	void StyleSetCase(int nStyle,int nMode)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STYLESETCASE,nStyle,nMode);
	}

	void StyleSetVisible(int nStyle,bool bVisible)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STYLESETVISIBLE,nStyle,bVisible);
	}

	void StyleSetChangeable(int nStyle,bool bChangeable)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STYLESETCHANGEABLE,nStyle,bChangeable);
	}

	void StyleSetHotSpot(int nStyle,bool bHotspot)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STYLESETHOTSPOT,nStyle,bHotspot);
	}

	//@}
	/** @name Caret,Selection and Hotspot Styles */
	//@{

	int GetControlCharSymbol() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETCONTROLCHARSYMBOL,0,0L);
	}

	void SetControlCharSymbol(int nSymbol)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETCONTROLCHARSYMBOL,nSymbol,0L);
	}

	/** @name Caret Styles */
	//@{

	void SetCaretPolicy(int nPolicy,int nSlop)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETCARETPOLICY,nPolicy,nSlop);
	}

	COLORREF GetCaretFore() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETCARETFORE,0,0L);
	}

	void SetCaretFore(COLORREF crForeground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETCARETFORE,crForeground,0L);
	}

	bool GetCaretLineVisible() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETCARETLINEVISIBLE,0,0L)!=0;
	}

	void SetCaretLineVisible(bool bShow)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETCARETLINEVISIBLE,bShow,0L);
	}

	COLORREF GetCaretLineBack() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETCARETLINEBACK,0,0L);
	}

	void SetCaretLineBack(COLORREF crBackground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETCARETLINEBACK,crBackground,0L);
	}

	int GetCaretPeriod() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETCARETPERIOD,0,0L);
	}

	void SetCaretPeriod(int nPeriodMilliseconds)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETCARETPERIOD,nPeriodMilliseconds,0L);
	}

	int GetCaretWidth() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETCARETWIDTH,0,0L);
	}

	void SetCaretWidth(int nPixelWidth)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETCARETWIDTH,nPixelWidth,0L);
	}

	bool GetCaretSticky() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETCARETSTICKY,0,0L)!=0;
	}

	void SetCaretSticky(bool bSticky)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETCARETSTICKY,bSticky,0L);
	}

	void ToggleCaretSticky()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_TOGGLECARETSTICKY,0,0L);
	}

	//@}
	/** @name Selection Styles */
	//@{

	void SetSelFore(bool bUseSetting,COLORREF crForeground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETSELFORE,bUseSetting,crForeground);
	}

	void SetSelBack(bool bUseSetting,COLORREF crBackground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETSELBACK,bUseSetting,crBackground);
	}

	//@}
	/** @name Hotspot Styles */
	//@{

	void SetHotspotActiveFore(bool bUseSetting,COLORREF crForeground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETHOTSPOTACTIVEFORE,bUseSetting,crForeground);
	}

	void SetHotspotActiveBack(bool bUseSetting,COLORREF crBackground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETHOTSPOTACTIVEBACK,bUseSetting,crBackground);
	}

	void SetHotspotActiveUnderline(bool bUnderline)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETHOTSPOTACTIVEUNDERLINE,bUnderline,0L);
	}

	void SetHotspotSingleLine(bool bSingleLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETHOTSPOTSINGLELINE,bSingleLine,0L);
	}

	//@}
	//@}
	/** @name Margins */
	//@{

	int GetMarginTypeN(int nMargin) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETMARGINTYPEN,nMargin,0L);
	}

	void SetMarginTypeN(int nMargin,int nType)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETMARGINTYPEN,nMargin,nType);
	}

	int GetMarginWidthN(int nMargin) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETMARGINWIDTHN,nMargin,0L);
	}

	void SetMarginWidthN(int nMargin,int nPixelWidth)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETMARGINWIDTHN,nMargin,nPixelWidth);
	}

	int GetMarginMaskN(int nMargin) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETMARGINMASKN,nMargin,0L);
	}

	void SetMarginMaskN(int nMargin,int nMask)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETMARGINMASKN,nMargin,nMask);
	}

	bool GetMarginSensitiveN(int nMargin) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETMARGINSENSITIVEN,nMargin,0L)!=
			0;
	}

	void SetMarginSensitiveN(int nMargin,bool bSensitive)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETMARGINSENSITIVEN,nMargin,bSensitive);
	}

	int GetMarginLeft() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETMARGINLEFT,0,0L);
	}

	void SetMarginLeft(int nPixelWidth)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETMARGINLEFT,0,nPixelWidth);
	}

	int GetMarginRight() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETMARGINRIGHT,0,0L);
	}

	void SetMarginRight(int nPixelWidth)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETMARGINRIGHT,0,nPixelWidth);
	}

	void SetFoldMarginColour(bool bUseSetting,COLORREF crBackground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETFOLDMARGINCOLOUR,bUseSetting,crBackground);
	}

	void SetFoldMarginHiColour(bool bUseSetting,COLORREF crForeground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETFOLDMARGINHICOLOUR,bUseSetting,crForeground);
	}

	//@}
	/** @name Other Settings */
	//@{

	bool GetUsePalette() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETUSEPALETTE,0,0L)!=0;
	}

	void SetUsePalette(bool bUsePalette)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETUSEPALETTE,bUsePalette,0L);
	}

	bool GetBufferedDraw() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETBUFFEREDDRAW)!=0;
	}

	void SetBufferedDraw(bool bBuffered)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETBUFFEREDDRAW,bBuffered,0L);
	}

	bool GetTwoPhaseDraw() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETTWOPHASEDRAW,0,0L)!=0;
	}

	void SetTwoPhaseDraw(bool bTwoPhase)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETTWOPHASEDRAW,bTwoPhase,0L);
	}

	int GetCodePage() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETCODEPAGE,0,0L);
	}

	void SetCodePage(int nCodePage)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETCODEPAGE,nCodePage,0L);
	}

	void SetWordChars(LPCSTR szCharacters)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETWORDCHARS,0,(LPARAM)szCharacters);
	}

	void SetWhitespaceChars(LPCSTR szCharacters)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETWHITESPACECHARS,0,(LPARAM)szCharacters);
	}

	void SetCharsDefault()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETCHARSDEFAULT,0,0L);
	}

	void GrabFocus()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_GRABFOCUS,0,0L);
	}

	bool GetFocus() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETFOCUS,0,0L)!=0;
	}

#ifdef SCINTILLA_USE_FOCUS
	void SetFocus(bool bFocus)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETFOCUS,bFocus,0L);
	}
#endif

	//@}
	/** @name Brace Highlighting */
	//@{

	void BraceHighlight(unsigned int nPosition1,unsigned int nPosition2)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_BRACEHIGHLIGHT,nPosition1,nPosition2);
	}

	void BraceBadLight(unsigned int nPosition)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_BRACEBADLIGHT,nPosition,0L);
	}

	unsigned int BraceMatch(unsigned int nPosition)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_BRACEMATCH,nPosition,0L);
	}

	//@}
	/** @name Tabs and Indentation Guides */
	//@{

	int GetTabWidth() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETTABWIDTH);
	}

	void SetTabWidth(int nWidth)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETTABWIDTH,nWidth);
	}

	bool GetUseTabs() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETUSETABS,0,0L)!=0;
	}

	void SetUseTabs(bool bTabs)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETUSETABS,bTabs,0L);
	}

	int GetIndent() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETINDENT,0,0L);
	}

	void SetIndent(int nSize)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETINDENT,nSize,0L);
	}

	bool GetTabIndents() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETTABINDENTS,0,0L)!=0;
	}

	void SetTabIndents(bool bIndents)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETTABINDENTS,bIndents,0L);
	}

	bool GetBackSpaceUnIndents() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETBACKSPACEUNINDENTS,0,0L)!=0;
	}

	void SetBackSpaceUnIndents(bool bUnIndents)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETBACKSPACEUNINDENTS,bUnIndents,0L);
	}

	int GetLineIndentation(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETLINEINDENTATION,nLine,0L);
	}

	void SetLineIndentation(int nLine,int nSize)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETLINEINDENTATION,nLine,nSize);
	}

	unsigned int GetLineIndentPosition(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETLINEINDENTPOSITION,nLine,0L);
	}

	bool GetIndentationGuides() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETINDENTATIONGUIDES,0,0L)!=0;
	}

	void SetIndentationGuides(bool bShow)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETINDENTATIONGUIDES,bShow,0L);
	}

	int GetHighlightGuide() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETHIGHLIGHTGUIDE,0,0L);
	}

	void SetHighlightGuide(int nColumn)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETHIGHLIGHTGUIDE,nColumn,0L);
	}

	//@}
	/** @name Markers */
	//@{

	void MarkerDefine(int nMarker,int nSymbol)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_MARKERDEFINE,nMarker,nSymbol);
	}

	void MarkerDefinePixmap(int nMarker,LPCSTR szPixmap)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_MARKERDEFINEPIXMAP,nMarker,(LPARAM)szPixmap);
	}

	void MarkerSetFore(int nMarker,COLORREF crForeground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_MARKERSETFORE,nMarker,crForeground);
	}

	void MarkerSetBack(int nMarker,COLORREF crBackground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_MARKERSETBACK,nMarker,crBackground);
	}

	int MarkerAdd(int nLine,int nMarker)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_MARKERADD,nLine,nMarker);
	}

	int MarkerAddSet(int nLine,int nMarker)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_MARKERADDSET,nLine,nMarker);
	}

	void MarkerDelete(int nLine,int nMarker)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_MARKERDELETE,nLine,nMarker);
	}

	void MarkerDeleteAll(int nMarker)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_MARKERDELETEALL,nMarker,0L);
	}

	int MarkerGet(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_MARKERGET,nLine,0L);
	}

	int MarkerNext(int nStart,int nMask) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_MARKERNEXT,nStart,nMask);
	}

	int MarkerPrevious(int nStart,int nMask) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_MARKERPREVIOUS,nStart,
			nMask);
	}

	int MarkerLineFromHandle(int nHandle) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_MARKERLINEFROMHANDLE,nHandle,0L);
	}

	void MarkerDeleteHandle(int nHandle)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_MARKERDELETEHANDLE,nHandle,0L);
	}

	//@}
	/** @name Indicators */
	//@{

	int IndicGetStyle(int nIndic) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_INDICGETSTYLE,nIndic,0L);
	}

	void IndicSetStyle(int nIndic,int nStyle)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_INDICSETSTYLE,nIndic,nStyle);
	}

	COLORREF IndicGetFore(int nIndic) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_INDICGETFORE,nIndic,0L);
	}

	void IndicSetFore(int nIndic,COLORREF crForeground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_INDICSETFORE,nIndic,crForeground);
	}

	//@}
	/** @name Autocomplete */
	//@{

	void AutoCShow(int nLength,LPCSTR szList)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_AUTOCSHOW,nLength,(LPARAM)szList);
	}

	void AutoCCancel()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_AUTOCCANCEL,0,0L);
	}

	bool AutoCActive() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_AUTOCACTIVE,0,0L)!=0;
	}

	unsigned int AutoCPosStart() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_AUTOCPOSSTART,0,0L);
	}

	void AutoCComplete()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_AUTOCCOMPLETE,0,0L);
	}

	void AutoCStops(LPCSTR szSet)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_AUTOCSTOPS,0,(LPARAM)szSet);
	}

	int AutoCGetSeparator() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_AUTOCGETSEPARATOR,0,0L);
	}

	void AutoCSetSeparator(int nCharacter)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_AUTOCSETSEPARATOR,nCharacter,0L);
	}

	void AutoCSelect(LPCSTR szText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_AUTOCSELECT,0,(LPARAM)szText);
	}

	int AutoCGetCurrent() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_AUTOCGETCURRENT,0,0L);
	}

	bool AutoCGetCancelAtStart() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_AUTOCGETCANCELATSTART,0,0L)!=0;
	}

	void AutoCSetCancelAtStart(bool bCancel)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_AUTOCSETCANCELATSTART,bCancel,0L);
	}

	void AutoCSetFillUps(LPCSTR szSet)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_AUTOCSETFILLUPS,0,(LPARAM)szSet);
	}

	bool AutoCGetChooseSingle() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_AUTOCGETCHOOSESINGLE,0,0L)!=0;
	}

	void AutoCSetChooseSingle(bool bSingle)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_AUTOCSETCHOOSESINGLE,bSingle,0L);
	}

	bool AutoCGetIgnoreCase() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_AUTOCGETIGNORECASE,0,0L)!=0;
	}

	void AutoCSetIgnoreCase(bool bIgnoreCase)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_AUTOCSETIGNORECASE,bIgnoreCase,0L);
	}

	bool AutoCGetAutoHide() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_AUTOCGETAUTOHIDE,0,0L)!=0;
	}

	void AutoCSetAutoHide(bool bAutoHide)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_AUTOCSETAUTOHIDE,bAutoHide,0L);
	}

	bool AutoCGetDropRestOfWord() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_AUTOCGETDROPRESTOFWORD,0,0L)!=0;
	}

	void AutoCSetDropRestOfWord(bool bDropRestOfWord)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_AUTOCSETDROPRESTOFWORD,bDropRestOfWord,0L);
	}

	void RegisterImage(int nType,LPCSTR szXPMData)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_REGISTERIMAGE,nType,(LPARAM)szXPMData);
	}

	void ClearRegisteredImages()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CLEARREGISTEREDIMAGES,0,0L);
	}

	int AutoCGetTypeSeparator() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_AUTOCGETTYPESEPARATOR,0,0L);
	}

	void AutoCSetTypeSeparator(int nCharacter)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_AUTOCSETTYPESEPARATOR,nCharacter,
			0L);
	}

	int AutoCGetMaxWidth() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_AUTOCGETMAXWIDTH,0,0L);
	}

	void AutoCSetMaxWidth(int nCount)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_AUTOCSETMAXWIDTH,nCount,0L);
	}

	int AutoCGetMaxHeight() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_AUTOCGETMAXHEIGHT,0,0L);
	}

	void AutoCSetMaxHeight(int nRowCount)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_AUTOCSETMAXHEIGHT,nRowCount,0L);
	}

	//@}
	/** @name User Lists */
	//@{

	void UserListShow(int nType,LPCSTR szList)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_USERLISTSHOW,nType,(LPARAM)szList);
	}

	//@}
	/** @name Call tips */
	//@{

	void CallTipShow(unsigned int nPosition,LPCSTR szTip)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CALLTIPSHOW,nPosition,(LPARAM)szTip);
	}

	void CallTipCancel()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CALLTIPCANCEL,0,0L);
	}

	bool CallTipActive()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_CALLTIPACTIVE,0,0L)!=0;
	}

	unsigned int CallTipPosStart() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_CALLTIPPOSSTART,0,0L);
	}

	void CallTipSetHlt(int nStart,int nEnd)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CALLTIPSETHLT,nStart,nEnd);
	}

	void CallTipSetBack(COLORREF crBackground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CALLTIPSETBACK,crBackground,0L);
	}

	void CallTipSetFore(COLORREF crForeground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CALLTIPSETFORE,crForeground,0L);
	}

	void CallTipSetForeHlt(COLORREF crForeground)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CALLTIPSETFOREHLT,crForeground,0L);
	}

	//@}
	/** @name Keyboard Commands */
	//@{

	void LineDown()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINEDOWN,0,0L);
	}

	void LineDownExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINEDOWNEXTEND,0,0L);
	}

	void LineUp()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINEUP,0,0L);
	}

	void LineUpExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINEUPEXTEND,0,0L);
	}

	void LineDownRectExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINEDOWNRECTEXTEND,0,0L);
	}

	void LineUpRectExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINEUPRECTEXTEND,0,0L);
	}

	void LineScrollDown()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINESCROLLDOWN,0,0L);
	}

	void LineScrollUp()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINESCROLLUP,0,0L);
	}

	void ParaDown()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_PARADOWN,0,0L);
	}

	void ParaDownExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_PARADOWNEXTEND,0,0L);
	}

	void ParaUp()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_PARAUP,0,0L);
	}

	void ParaUpExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_PARAUPEXTEND,0,0L);
	}

	void CharLeft()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CHARLEFT,0,0L);
	}

	void CharLeftExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CHARLEFTEXTEND,0,0L);
	}

	void CharRight()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CHARRIGHT,0,0L);
	}

	void CharRightExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CHARRIGHTEXTEND,0,0L);
	}

	void CharLeftRectExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CHARLEFTRECTEXTEND,0,0L);
	}

	void CharRightRectExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CHARRIGHTRECTEXTEND,0,0L);
	}

	void WordLeft()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_WORDLEFT,0,0L);
	}

	void WordLeftExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_WORDLEFTEXTEND,0,0L);
	}

	void WordRight()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_WORDRIGHT,0,0L);
	}

	void WordRightExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_WORDRIGHTEXTEND,0,0L);
	}

	void WordLeftEnd()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_WORDLEFTEND,0,0L);
	}

	void WordLeftEndExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_WORDLEFTENDEXTEND,0,0L);
	}

	void WordRightEnd()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_WORDRIGHTEND,0,0L);
	}

	void WordRightEndExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_WORDRIGHTENDEXTEND,0,0L);
	}

	void WordPartLeft()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_WORDPARTLEFT,0,0L);
	}

	void WordPartLeftExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_WORDPARTLEFTEXTEND,0,0L);
	}

	void WordPartRight()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_WORDPARTRIGHT,0,0L);
	}

	void WordPartRightExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_WORDPARTRIGHTEXTEND,0,0L);
	}

	void Home()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_HOME,0,0L);
	}

	void HomeExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_HOMEEXTEND,0,0L);
	}

	void HomeRectExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_HOMERECTEXTEND,0,0L);
	}

	void HomeDisplay()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_HOMEDISPLAY,0,0L);
	}

	void HomeDisplayExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_HOMEDISPLAYEXTEND,0,0L);
	}

	void HomeWrap()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_HOMEWRAP,0,0L);
	}

	void HomeWrapExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_HOMEWRAPEXTEND,0,0L);
	}

	void VCHome()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_VCHOME,0,0L);
	}

	void VCHomeExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_VCHOMEEXTEND,0,0L);
	}

	void VCHomeRectExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_VCHOMERECTEXTEND,0,0L);
	}

	void VCHomeWrap()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_VCHOMEWRAP,0,0L);
	}

	void VCHomeWrapExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_VCHOMEWRAPEXTEND,0,0L);
	}

	void LineEnd()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINEEND,0,0L);
	}

	void LineEndExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINEENDEXTEND,0,0L);
	}

	void LineEndRectExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINEENDRECTEXTEND,0,0L);
	}

	void LineEndDisplay()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINEENDDISPLAY,0,0L);
	}

	void LineEndDisplayExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINEENDDISPLAYEXTEND,0,0L);
	}

	void LineEndWrap()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINEENDWRAP,0,0L);
	}

	void LineEndWrapExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINEENDWRAPEXTEND,0,0L);
	}

	void DocumentStart()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_DOCUMENTSTART,0,0L);
	}

	void DocumentStartExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_DOCUMENTSTARTEXTEND,0,0L);
	}

	void DocumentEnd()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_DOCUMENTEND,0,0L);
	}

	void DocumentEndExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_DOCUMENTENDEXTEND,0,0L);
	}

	void PageUp()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_PAGEUP,0,0L);
	}

	void PageUpExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_PAGEUPEXTEND,0,0L);
	}

	void PageUpRectExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_PAGEUPRECTEXTEND,0,0L);
	}

	void PageDown()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_PAGEDOWN,0,0L);
	}

	void PageDownExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_PAGEDOWNEXTEND,0,0L);
	}

	void PageDownRectExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_PAGEDOWNRECTEXTEND,0,0L);
	}

	void StutteredPageUp()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STUTTEREDPAGEUP,0,0L);
	}

	void StutteredPageUpExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STUTTEREDPAGEUPEXTEND,0,0L);
	}

	void StutteredPageDown()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STUTTEREDPAGEDOWN,0,0L);
	}

	void StutteredPageDownExtend()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STUTTEREDPAGEDOWNEXTEND,0,0L);
	}

	void DeleteBack()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_DELETEBACK,0,0L);
	}

	void DeleteBackNotLine()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_DELETEBACKNOTLINE,0,0L);
	}

	void DelWordLeft()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_DELWORDLEFT,0,0L);
	}

	void DelWordRight()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_DELWORDRIGHT,0,0L);
	}

	void DelLineLeft()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_DELLINELEFT,0,0L);
	}

	void DelLineRight()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_DELLINERIGHT,0,0L);
	}

	void LineDelete()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINEDELETE,0,0L);
	}

	void LineCut()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINECUT,0,0L);
	}

	void LineCopy()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINECOPY,0,0L);
	}

	void LineTranspose()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINETRANSPOSE,0,0L);
	}

	void LineDuplicate()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINEDUPLICATE,0,0L);
	}

	void LowerCase()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LOWERCASE,0,0L);
	}

	void UpperCase()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_UPPERCASE,0,0L);
	}

	void Cancel()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CANCEL,0,0L);
	}

	void EditToggleOvertype()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_EDITTOGGLEOVERTYPE,0,0L);
	}

	void NewLine()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_NEWLINE,0,0L);
	}

	void FormFeed()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_FORMFEED,0,0L);
	}

	void Tab()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_TAB,0,0L);
	}

	void BackTab()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_BACKTAB,0,0L);
	}

	void SelectionDuplicate()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SELECTIONDUPLICATE,0,0L);
	}

	//@}
	/** @name Key Bindings */
	//@{

	void AssignCmdKey(DWORD dwKey,int nCommand)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_ASSIGNCMDKEY,dwKey,nCommand);
	}

	void AssignCmdKey(WORD nVirtualKey,WORD nMode,int nCommand)
	{
		AssignCmdKey(nVirtualKey + (nMode << 16),nCommand);
	}

	void ClearCmdKey(DWORD dwKey)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CLEARCMDKEY,dwKey);
	}

	void ClearAllCmdKeys()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_CLEARALLCMDKEYS);
	}

	void Null()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_NULL,0,0L);
	}

	//@}
	/** @name Popup Edit Menu */
	//@{

	void UsePopUp(bool bAllowPopUp)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_USEPOPUP,bAllowPopUp,0L);
	}

	//@}
	/** @name Macro Recording */
	//@{

	void StartRecord()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STARTRECORD,0,0L);
	}

	void StopRecord()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_STOPRECORD,0,0L);
	}

	//@}
	/** @name Printing */
	//@{

	unsigned int FormatRange(bool bDraw,RangeToFormat& rfRange)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_FORMATRANGE,bDraw,(LPARAM)&rfRange);
	}

	int GetPrintMagnification() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETPRINTMAGNIFICATION,0,0L);
	}

	void SetPrintMagnification(int nMagnification)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETPRINTMAGNIFICATION,nMagnification,0L);
	}

	int GetPrintColourMode() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETPRINTCOLOURMODE,0,0L);
	}

	void SetPrintColourMode(int nMode)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETPRINTCOLOURMODE,nMode,0L);
	}

	int GetPrintWrapMode() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETPRINTWRAPMODE,0,0L);
	}

	void SetPrintWrapMode(int nMode)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETPRINTWRAPMODE,nMode,0L);
	}

	//@}
	/** @name Direct Access */
	//@{

	SciFnDirect GetDirectFunction() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (SciFnDirect)::SendMessage(m_hWnd,SCI_GETDIRECTFUNCTION,0,
			0L);
	}

	void * GetDirectPointer() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (void *)::SendMessage(m_hWnd,SCI_GETDIRECTPOINTER,0,0L);
	}

	//@}
	/** @name Multiple Views */
	//@{

	void* GetDocPointer() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETDOCPOINTER,0,0L);
	}

	void SetDocPointer(void* pPointer)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETDOCPOINTER,0,(LPARAM)pPointer);
	}

	void* CreateDocument()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (void*)::SendMessage(m_hWnd,SCI_CREATEDOCUMENT,0,0L);
	}

	void AddRefDocument(void* pDocument)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_ADDREFDOCUMENT,0,(LPARAM)pDocument);
	}

	void ReleaseDocument(void* pDocument)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_RELEASEDOCUMENT,0,(LPARAM)pDocument);
	}

	//@}
	/** @name Folding */
	//@{

	int VisibleFromDocLine(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_VISIBLEFROMDOCLINE,nLine,0L);
	}

	int DocLineFromVisible(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_DOCLINEFROMVISIBLE,nLine,0L);
	}

	void ShowLines(int nStart,int nEnd)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SHOWLINES,nStart,nEnd);
	}

	void HideLines(int nStart,int nEnd)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_HIDELINES,nStart,nEnd);
	}

	bool GetLineVisible(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETLINEVISIBLE,nLine,0L)!=0;
	}

	int GetFoldLevel(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETFOLDLEVEL,nLine,0L);
	}

	void SetFoldLevel(int nLine,int nLevel)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETFOLDLEVEL,nLine,nLevel);
	}

	void SetFoldFlags(int nFlags)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETFOLDFLAGS,nFlags,0L);
	}

	int GetLastChild(int nLine,int nLevel) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETLASTCHILD,nLine,nLevel);
	}

	int GetFoldParent(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETFOLDPARENT,nLine,0L);
	}

	bool GetFoldExpanded(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETFOLDEXPANDED,nLine,0L)!=0;
	}

	void SetFoldExpanded(int nLine,bool bExpanded)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETFOLDEXPANDED,nLine,bExpanded);
	}

	void ToggleFold(int nLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_TOGGLEFOLD,nLine,0L);
	}

	void EnsureVisible(int nLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_ENSUREVISIBLE,nLine,0L);
	}

	void EnsureVisibleEnforcePolicy(int nLine)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_ENSUREVISIBLEENFORCEPOLICY,nLine,0L);
	}

	//@}
	/** @name Line Wrapping */
	//@{

	int GetWrapMode() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETWRAPMODE,0,0L);
	}

	void SetWrapMode(int nMode)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETWRAPMODE,nMode,0L);
	}

	int GetWrapVisualFlags() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETWRAPVISUALFLAGS,0,0L);
	}

	void SetWrapVisualFlags(int nFlags)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETWRAPVISUALFLAGS,nFlags,0L);
	}

	void SetWrapVisualFlagsLocation(int nFlagsLocation)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETWRAPVISUALFLAGSLOCATION,nFlagsLocation,0L);
	}

	int GetWrapVisualFlagsLocation() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETWRAPVISUALFLAGSLOCATION,0,0L);
	}

	int GetWrapStartIndent() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETWRAPSTARTINDENT,0,0L);
	}

	void SetWrapStartIndent(int nIndent)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETWRAPSTARTINDENT,nIndent,0L);
	}

	int GetLayoutCache() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETLAYOUTCACHE,0,0L);
	}

	void SetLayoutCache(int nMode)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETLAYOUTCACHE,nMode,0L);
	}

	void LinesSplit(int nPixelWidth)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINESSPLIT,nPixelWidth,0L);
	}

	void LinesJoin()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LINESJOIN,0,0L);
	}

	int WrapCount(int nLine) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_WRAPCOUNT,nLine,0L);
	}

	//@}
	/** @name Zooming */
	//@{

	void ZoomIn()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_ZOOMIN,0,0L);
	}

	void ZoomOut()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_ZOOMOUT,0,0L);
	}

	int GetZoom() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETZOOM,0,0L);
	}

	void SetZoom(int zoom)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETZOOM,zoom,0L);
	}

	//@}
	/** @name Long Lines */
	//@{

	int GetEdgeMode() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETEDGEMODE,0,0L);
	}

	void SetEdgeMode(int nMode)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETEDGEMODE,nMode,0L);
	}

	int GetEdgeColumn() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETEDGECOLUMN,0,0L);
	}

	void SetEdgeColumn(int nColumn)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETEDGECOLUMN,nColumn,0L);
	}

	COLORREF GetEdgeColour() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETEDGECOLOUR,0,0L);
	}

	void SetEdgeColour(COLORREF crEdge)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETEDGECOLOUR,crEdge,0L);
	}

	//@}
	/** @name Lexer */
	//@{

	int GetLexer() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETLEXER,0,0L);
	}

	void SetLexer(int nLexer)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETLEXER,nLexer,0L);
	}

	void SetLexerLanguage(LPCSTR szLanguage)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETLEXERLANGUAGE,0,(LPARAM)szLanguage);
	}

	void LoadLexerLibrary(LPCSTR szPath)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_LOADLEXERLIBRARY,0,(LPARAM)szPath);
	}

	void Colourise(unsigned int nStart,unsigned int nEnd)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_COLOURISE,nStart,nEnd);
	}

	int GetProperty(LPCSTR dwKey,LPSTR szBuffer) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETPROPERTY,(WPARAM)dwKey,(LPARAM)szBuffer);
	}

	void SetProperty(LPCSTR dwKey,LPCSTR szValue)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETPROPERTY,(WPARAM)dwKey,(LPARAM)szValue);
	}

	int GetPropertyExpanded(LPCSTR dwKey,LPSTR szBuffer) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETPROPERTYEXPANDED,(WPARAM)dwKey,(LPARAM)szBuffer);
	}

	int GetPropertyInt(LPCSTR dwKey) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETPROPERTYINT,(WPARAM)dwKey,0L);
	}

	void SetKeyWords(int nSet,LPCSTR szKeyWords)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETKEYWORDS,nSet,(LPARAM)szKeyWords);
	}

	int GetStyleBitsNeeded() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETSTYLEBITSNEEDED,0,0L);
	}

	//@}
	/** @name Notifications */
	//@{

	int GetModEventMask() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETMODEVENTMASK,0,0L);
	}

	void SetModEventMask(int nMask)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETMODEVENTMASK,nMask,0L);
	}

	int GetMouseDwellTime() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_GETMOUSEDWELLTIME,0,0L);
	}

	void SetMouseDwellTime(int nPeriodMilliseconds)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETMOUSEDWELLTIME,nPeriodMilliseconds,0L);
	}

#if defined(__ATLSTR_H__)
	int GetCurLine(CStringA& szText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		int nLength=GetCurLine(0,0);
		int nResult=GetCurLine(szText.GetBuffer(nLength-1),nLength);
		szText.ReleaseBuffer(nLength-1);
		return nResult;
	}

	int GetLine(int nLine,CStringA& szText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		int nLength=GetLine(nLine,0);
		int nResult=GetLine(nLine,szText.GetBuffer(nLength));
		szText.ReleaseBuffer(nLength);
		return nResult;
	}

	int GetSimpleLine(int nLine,CStringA& szText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		TextRange trRange;
		trRange.chrg.cpMin=PositionFromLine(nLine);
		trRange.chrg.cpMax=GetLineEndPosition(nLine);
		int nLength=trRange.chrg.cpMax-trRange.chrg.cpMin;
		trRange.lpstrText=szText.GetBuffer(nLength);
		int nResult=GetTextRange(trRange);
		szText.ReleaseBuffer(nLength);
		return nResult;
	}

	int GetSelText(CStringA& szText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		int nLength=GetSelText(0)-1;
		int nResult=GetSelText(szText.GetBuffer(nLength));
		szText.ReleaseBuffer(nLength);
		return nResult;
	}

	int GetText(CStringA& szText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		int nLength=GetLength();
		int nResult=GetText(szText.GetBuffer(nLength),nLength+1);
		szText.ReleaseBuffer(nLength);
		return nResult;
	}
#endif

#if defined(GTK)
	int TargetAsUTF8(LPSTR szText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_TARGETASUTF8,0,(LPARAM)szText);
	}
	void SetLengthForEncode(int nBytes)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETLENGTHFORENCODE,nBytes);
	}
	int EncodedFromUTF8(LPCSTR szUTF8,LPSTR szEncoded)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,SCI_ENCODEDFROMUTF8,(WPARAM)szUTF8,(LPARAM)szEncoded);
	}
#endif

#if defined(__ATLSTR_H__)
	int GetProperty(LPCSTR dwKey,CStringA& szValue)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		int nLength=GetProperty(dwKey,0);
		int nResult=GetProperty(dwKey,szValue.GetBuffer(nLength));
		szValue.ReleaseBuffer(nLength);
		return nResult;
	}

	int GetPropertyExpanded(LPCSTR dwKey,CStringA& szValue)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		int nLength=GetPropertyExpanded(dwKey,0);
		int nResult=GetPropertyExpanded(dwKey,szValue.GetBuffer(nLength));
		szValue.ReleaseBuffer(nLength);
		return nResult;
	}
#endif

#ifdef INCLUDE_DEPRECATED_FEATURES
	void SetCaretPolicy(int nPolicy,int nSlop)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd,SCI_SETCARETPOLICY,nPolicy,nSlop);
	}
#endif

    BOOL Save(LPCTSTR szFilename)
	{
		CAtlFile File;
		if (FAILED(File.Create(szFilename,GENERIC_WRITE,FILE_SHARE_READ,CREATE_ALWAYS,0)))
			return FALSE;
		char szBuffer[8192];
		TextRange trRange;
		trRange.lpstrText=szBuffer;
		DWORD dwWritten;
		if (GetCodePage()==SC_CP_UTF8)
		{
			static BYTE UTF8[]={0xEF,0xBB,0xBF};
			if (FAILED(File.Write(UTF8,sizeof(UTF8),&dwWritten)) || dwWritten!=sizeof(UTF8))
				return FALSE;
		}
		for (int nIndex=0,nLength=GetLength();nIndex<nLength;nIndex+=(int)dwWritten)
		{
			dwWritten=(DWORD)(nLength-nIndex)>sizeof(szBuffer) ? sizeof(szBuffer) : (DWORD)(nLength-nIndex);
			trRange.chrg.cpMin=nIndex;
			trRange.chrg.cpMax=nIndex+(long)dwWritten;
			GetTextRange(trRange);
			if (FAILED(File.Write(szBuffer,dwWritten,&dwWritten)))
				return FALSE;
		}
		SetSavePoint();
		return TRUE;
	}

    BOOL Load(LPCTSTR szFilename)
	{
		CAtlFile File;
		if (FAILED(File.Create(szFilename,GENERIC_READ,FILE_SHARE_READ,OPEN_EXISTING,0)))
			return FALSE;
		ClearAll();
		SetUndoCollection(false);
		static BYTE UTF8[]={0xEF,0xBB,0xBF};
		char szBuffer[8192];
		DWORD dwRead;
		if (SUCCEEDED(File.Read(szBuffer,sizeof(UTF8),dwRead)) && dwRead>0)
		{
            if (dwRead!=sizeof(UTF8) || memcmp(szBuffer,UTF8,sizeof(UTF8)))
			{
				SetCodePage(0);
				AddText(szBuffer,(int)dwRead);
			}
			else
				SetCodePage(SC_CP_UTF8);
		}
		for (;SUCCEEDED(File.Read(szBuffer,sizeof(szBuffer),dwRead)) && dwRead>0;AddText(szBuffer,(int)dwRead));
		SetSel(0,0);
		SetUndoCollection(true);
		SetSavePoint();
		SetReadOnly((GetFileAttributes(szFilename) & FILE_ATTRIBUTE_READONLY) ? true : false);
		return TRUE;
	}
};

typedef CScintillaWindowT<ATL::CWindow> CScintillaWindow;

template <class T>
class CScintillaWindowCommands : public CMessageFilter
{
public:
	BEGIN_MSG_MAP(CScintillaWindowCommands<T>)
	ALT_MSG_MAP(1)
		MESSAGE_HANDLER(CFindReplaceDialog::GetFindReplaceMsg(), OnFindReplace)
		COMMAND_ID_HANDLER(ID_EDIT_CLEAR, OnEditClear)
		COMMAND_ID_HANDLER(ID_EDIT_CLEAR_ALL, OnEditClearAll)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnEditCopy)
		COMMAND_ID_HANDLER(ID_EDIT_CUT, OnEditCut)
		COMMAND_ID_HANDLER(ID_EDIT_PASTE, OnEditPaste)
		COMMAND_ID_HANDLER(ID_EDIT_SELECT_ALL, OnEditSelectAll)
		COMMAND_ID_HANDLER(ID_EDIT_UNDO, OnEditUndo)
		COMMAND_ID_HANDLER(ID_EDIT_REDO, OnEditRedo)
		COMMAND_ID_HANDLER(ID_EDIT_FIND, OnFind)
		COMMAND_ID_HANDLER(ID_EDIT_REPLACE, OnFind)
		COMMAND_ID_HANDLER(ID_EDIT_REPEAT, OnRepeat)
	END_MSG_MAP()

	LRESULT OnFindReplace(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		T* pT=static_cast<T*>(this);
		CFindReplaceDialog* pDlg=CFindReplaceDialog::GetNotifier(lParam);
		ATLASSERT(pDlg);
		if (pDlg->IsTerminating())
		{
			m_pFindReplaceDlg=NULL;
			return 0;
		}
		if (pDlg->FindNext())
		{
			FindNext();
		}
		else if (pDlg->ReplaceCurrent())
		{
			if (HasSelection())
			{
				/*
				CStringA szText;
				pT->GetSelText(szText);
				if ((m_bFindCase && szText.Compare(m_szFind)==0) || (!m_bFindCase && szText.CompareNoCase(m_szFind)==0))
				*/
					pT->ReplaceSel(CT2CA(pDlg->GetReplaceString()));
			}
			FindNext();
		}
		else if (pDlg->ReplaceAll())
		{
			CWaitCursor Wait;
			while (FindNext(FALSE))
				pT->ReplaceSel(CT2CA(pDlg->GetReplaceString()));
		}
		return 0;
	}

	LRESULT OnEditClear(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		T* pT=static_cast<T*>(this);
		pT->Clear();
		return 0;
	}

	LRESULT OnEditClearAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		T* pT=static_cast<T*>(this);
		pT->SetSel(0,-1);
		pT->Clear();
		return 0;
	}

	LRESULT OnEditCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		T* pT=static_cast<T*>(this);
		pT->Copy();
		return 0;
	}

	LRESULT OnEditCut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		T* pT=static_cast<T*>(this);
		pT->Cut();
		return 0;
	}

	LRESULT OnEditPaste(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		T* pT=static_cast<T*>(this);
		pT->Paste();
		return 0;
	}

	LRESULT OnEditSelectAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		T* pT=static_cast<T*>(this);
		pT->SetSel(0,-1);
		return 0;
	}

	LRESULT OnEditUndo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		T* pT=static_cast<T*>(this);
		pT->Undo();
		return 0;
	}

	LRESULT OnEditRedo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		T* pT=static_cast<T*>(this);
		pT->Redo();
		return 0;
	}

	LRESULT OnFind(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		T* pT=static_cast<T*>(this);
		if (m_pFindReplaceDlg)
			m_pFindReplaceDlg->DestroyWindow();
		m_pFindReplaceDlg=new CFindReplaceDialog;
		if (HasSelection())
		{
			char szSelection[CFindReplaceDialog::_cchFindReplaceBuffer];
			TextRange Tr;
			Tr.lpstrText=szSelection;
			Tr.chrg.cpMin=pT->GetSelectionStart();
			Tr.chrg.cpMax=pT->GetSelectionEnd();
			if (Tr.chrg.cpMax-Tr.chrg.cpMin>=CFindReplaceDialog::_cchFindReplaceBuffer)
				Tr.chrg.cpMax=Tr.chrg.cpMin+CFindReplaceDialog::_cchFindReplaceBuffer-1;
			pT->GetTextRange(Tr);
			lstrcpyn(m_pFindReplaceDlg->m_fr.lpstrFindWhat,CA2CT(szSelection),CFindReplaceDialog::_cchFindReplaceBuffer);
		}
		m_pFindReplaceDlg->Create(wID==ID_EDIT_FIND,NULL,NULL,FR_HIDEUPDOWN,pT->GetTopLevelParent());
		m_pFindReplaceDlg->ShowWindow(SW_NORMAL);
		return 0;
	}

	LRESULT OnRepeat(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		FindNext();
		return 0;
	}

	// State (update UI) helpers
	BOOL CanCut() const
	{ return !IsReadOnly() && HasSelection(); }

	BOOL CanCopy() const
	{ return HasSelection(); }

	BOOL CanClear() const
	{ return !IsReadOnly() && HasSelection(); }

	BOOL CanSelectAll() const
	{ return HasText(); }

	BOOL CanFind() const
	{ return HasText(); }

	BOOL CanRepeat() const
	{ return m_pFindReplaceDlg && HasText(); }

	BOOL CanReplace() const
	{ return !IsReadOnly() && HasText(); }

	BOOL CanClearAll() const
	{ return !IsReadOnly() && HasText(); }

	// Implementation
	BOOL HasSelection() const
	{
		const T* pT=static_cast<const T*>(this);
		ATLASSERT(pT->m_hWnd);
		return (::SendMessage(pT->m_hWnd,SCI_GETSELECTIONSTART,0,0)!=::SendMessage(pT->m_hWnd,SCI_GETSELECTIONEND,0,0));
	}

	BOOL HasText() const
	{
		const T* pT=static_cast<const T*>(this);
		ATLASSERT(pT->m_hWnd);
		return ::SendMessage(pT->m_hWnd,SCI_GETLENGTH,0,0)!=0;
	}

	BOOL IsReadOnly() const
	{
		const T* pT=static_cast<const T*>(this);
		ATLASSERT(pT->m_hWnd);
		return ::SendMessage(pT->m_hWnd,SCI_GETREADONLY,0,0);
	}

protected:
	static CFindReplaceDialog* m_pFindReplaceDlg;

private:
	BOOL FindNext(BOOL bNext=TRUE)
	{
		T* pT=static_cast<T*>(this);
		if (*m_pFindReplaceDlg->GetFindString()=='\0')
			return FALSE;
		TextToFind Tf;
		Tf.chrg.cpMin=bNext ? pT->GetSelectionEnd() : pT->GetSelectionStart();
		Tf.chrg.cpMax=pT->GetLength();
		Tf.lpstrText=CT2A(m_pFindReplaceDlg->m_fr.lpstrFindWhat);
		int nFlags=0;
		if (m_pFindReplaceDlg->MatchCase())
			nFlags|=SCFIND_MATCHCASE;
		if (m_pFindReplaceDlg->MatchWholeWord())
			nFlags|=SCFIND_WHOLEWORD;
		if (pT->FindText(nFlags,Tf)<0)
			return FALSE;
		pT->SetSel(Tf.chrgText.cpMin,Tf.chrgText.cpMax);
		return TRUE;
	}
};

template<class T> __declspec(selectany) CFindReplaceDialog* CScintillaWindowCommands<T>::m_pFindReplaceDlg=NULL;

class CScintillaAutoRegister
{
	static long    m_nRefCount;
#ifndef STATIC_SCILEXER
	static HMODULE m_hScintilla;
#endif
public:
#ifdef STATIC_SCILEXER
	CScintillaAutoRegister(HINSTANCE hInstance=ATL::_AtlBaseModule.GetModuleInstance())
#else
	CScintillaAutoRegister()
#endif
	{
		if(m_nRefCount==0)
		{
#ifndef STATIC_SCILEXER
			m_hScintilla=::LoadLibrary(_T("SciLexer.dll"));
#else
			Scintilla_RegisterClasses(hInstance);
#endif
		}
		++m_nRefCount;
	}

	~CScintillaAutoRegister()
	{
		--m_nRefCount;
		if(m_nRefCount==0)
		{
#ifndef STATIC_SCILEXER
			::FreeLibrary(m_hScintilla);
			m_hScintilla=NULL;
#else
			Scintilla_ReleaseResources();
#endif
		}
	}
};

__declspec(selectany) long    CScintillaAutoRegister::m_nRefCount=0;
#ifndef STATIC_SCILEXER
__declspec(selectany) HMODULE CScintillaAutoRegister::m_hScintilla=NULL;
#endif

class XPMImages
{
public:
	enum image
	{
		xpmMethod=0,
		xpmMember=23
	};

	static LPCSTR Image(image nImage)
	{
		static LPCSTR Images[]=
		{
			"16 16 6 1"," 	c #FFFFFF",".	c #000000","+	c #FF00FF","@	c #800080","#	c #808080","$	c #C0C0C0","                ","                ","                ","           .    ","          .+.   ","         .+@+.  ","        .+@+@+. "," ## ##  ..+@+@+.","        .+.+@+..","  $#$## .@+.+.@.","         .@+.@@.","   #$###  .@.@. ","           ...  ","            .   ","                ","                ",
			"16 16 5 1"," 	c #FFFFFF",".	c #808080","+	c #00FFFF","@	c #008080","#	c #000000","                ","                ","                ","           .    ","          .+.   ","         .+@+.  ","        .+@+@+# ","       .+@+@+## ","       ..+@+#.# ","       .+.+#..# ","       .++#..#  ","        .+#.#   ","         .##    ","          #     ","                ","                "
		};
		return (LPCSTR)(Images+nImage);
	}
};