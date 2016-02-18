## Changes for 1.1.08 (2007-02-09) ##

**Wizard for Visual C++ 6.0**

**Supported Compilers**

  1. Visual C++ 6.0
  1. Visual C++ 2003
  1. Visual C++ 2005
  1. remove support of Mingw32 temporarily, because of atl License.

**Tutorials**

  1. LookNFeel(XP Menu)					tutorials/winx/step020-xp-menu
  1. APIHook(Local)						tutorials/winx/step021-api-hook/01.basic
  1. APIHook(Global)						tutorials/winx/step021-api-hook/02.global

**WINX-Extension**

  1. LookNFeel, XPMenu (see winx/LookNFeel.h)
  * `WINX_APP_MENU`
  * `WINX_APP_LOOKNFEEL`
    * Contributor (Idea & Implementation): renfengxing@gmail.com

  1. APIHook (see winx/APIHook.h)
  * class APIHook, AutoAPIHook, etc

**WINX-Core**

  1. IDispatch call (see winx/Dispatch.h)
  * class DispatchHandle
  * class DispatchMethod, ExDispatchMethod
  * class DispatchFunction, ExDispatchFunction
  * class DispatchProperty, ExDispatchProperty

**STL-Extension**

  1. FileMapping (see stdext/FileMapping.h)
    * class FileMapping
    * class SimpleFileMapping, SimpleFileMappingRO

  1. Registry (see stdext/Registry.h)
    * class WinRegKeyHandle
    * class WinRegReadKey, WinRegWriteKey

  1. ScopeDebug (see stdext/ScopeDebug.h)
    * class CheckException
    * class ScopeLog, OutputScopeLog, ErrorScopeLog, FileScopeLog
    * class ThreadLog
    * ErrorGuard:
      * `WINX_EXCEPTION_GUARD`
      * `WINX_WIN32_ERROR_GUARD`
      * `WINX_HRESULT_ERROR_GUARD`
    * Idea Contributor: sting.feng@gmail.com
    * Article: http://blog.csdn.net/singlerace/archive/2007/01/12/1481441.aspx

  1. std::replaceText algorithm, etc.



## Changes for 1.1.07 (2007-11-29) ##

**Supported Compilers
  1. Visual C++ 6.0
  1. Visual C++ 2003
  1. Visual C++ 2005
  1. Mingw32**

**Tutorials
  1. ScrollWindow(Basic)					tutorials/winx/step019-scroll-window/1.basic
  1. A Simple ImageViewer					tutorials/winx/step019-scroll-window/2.imageview
  1. FormView (ScrollWindow of Dialog)		tutorials/winx/step019-scroll-window/3.formview
  1. ZoomView (ZoomScrollWindow)			tutorials/winx/step019-scroll-window/4.zoomview**

**WINX-Extension
  1. class ScrollWindow, ZoomScrollWindow
  1. class FormWindow, AxFormWindow**

**STL-Extension
  1. implement `__uuidof` keyword in non-VC++ compilers.**

**Port ATL 3.0 to Mingw32**



## Changes for 1.1.03 (2006-12-11) ##

**Weed out some boring warning (when vs2005 compile winx tutorials).
> And fix up an error when compiling unicode version.**

**Try to support com addins.
> This is preparatory, and its interface might be changed.**



## Changes for 1.1.02x (2006-12-03) ##

Oh, I'm sorry that I forget the most important things.

Now I unify style of map macro, such as:

  1. `WINX_TEST_SUITE/WINX_TEST/WINX_TEST_SUITE_END;`
  1. `WINX_CMDS_BEGIN/WINX_CMD/WINX_CMDS_END;`
  1. `WINX_SYSCMD_BEGIN/WINX_SYSCMD/WINX_SYSCMD_END;`
  1. `WINX_NOTIFY_BEGIN/WINX_NOTIFY/WINX_NOTIFY_END;`
  1. `WINX_REFLECT_NOTIFY_BEGIN/WINX_REFLECT_NOTIFY/WINX_REFLECT_NOTIFY_END;`

I recommend you use them in this style:
```
	class MyDialog : public winx::ModalDialog<MyDialog>
	{
		WINX_CMDS_BEGIN()
			WINX_CMD(ID_TEST1, OnCmdTest1)
			WINX_CMD(ID_TEST2, OnCmdTest2)
		WINX_CMDS_END();
	};
```
Don't use them in old style like the following example (Though it works):
```
	class MyDialog : public winx::ModalDialog<MyDialog>
	{
		WINX_CMDS_BEGIN();
			WINX_CMD(ID_TEST1, OnCmdTest1);
			WINX_CMD(ID_TEST2, OnCmdTest2);
		WINX_CMDS_END();
	};
```


## Changes for 1.1.02 (2006-12-02) ##

This release focus:

  1. Port mfc applications to winx. Some examples were provided.

  1. Some useful components required by the DocX project.
> > see http://sourceforge.net/projects/docx.

  1. DocX will be used to make documentation for WINX. I will release
> > it as soon as possible.

**stdext(STL-Extension) as independent component**


> stdext(STL-Extension) is released as part of WINX. And now it is
> treated as independent component.

> I change directory tree from this:
```
		����winx
		��  ����include
		��  ��  ����winx
		��  ��  ����stdext
		��  ����docs(zh-CN)
		��  ����docs(en)
		��  ����samples
		��  ����tutorials
		��  ����lib
		����winsdk
		��  ����bin
		��  ����include
		��  ����lib
		����wtl
		    ����include
		    ����Notes
		    ����Samples
		    ����Wizards
```
> to this:
```
		����winx
		��  ����include
		��  ��  ����winx
		��  ����docs(zh-CN)
		��  ����docs(en)
		��  ����samples
		��  ����tutorials
		��  ����lib
		����stdext
		��  ����include
		��     ����stdext
		����winsdk
		��  ����bin
		��  ����include
		��  ����lib
		����wtl
		    ����include
		    ����Notes
		    ����Samples
		    ����Wizards
```

**Example: port MFC applications to WINX**

  1. samples/mfc-port/LCDMatrix(MFC)
    * summary: the Digital Display Control.
    * author: Nic Wilson, 2002.
    * url: http://www.codeproject.com/staticctrl/digidisplay.asp

  1. samples/mfc-port/LCDMatrix
    * summary: port MFC application: LCDMatrix to WINX.

  1. samples/mfc-port/AboutCtrl(MFC)
    * summary: Aboutbox Credits Control.
    * author: Pablo van der Meer, 2002.
    * url: http://www.codeproject.com/staticctrl/aboutctrl.asp
    * todo: I doesn't provide the WINX version of the application because I'd like to
> > > know that you have a try to do so. And I will provide one in next release.

**STL-Extension (stdext)**

  1. TestCase: CppUnit like style macro (but more light weight) for unit test.
    * `WINX_TEST_APP, WINX_TEST_CLASS, WINX_TEST_SUITE, WINX_TEST, WINX_TEST_SUITE_END`
    * UnitTestAssert(AssertExp, AssertEq, AssertEqBuf)

  1. Archive classes:
    * StreamArchive(StreamWriteArchive, StreamReadArchive, MemStreamWriteArchive, MemStreamReadArchive)
    * StdioArchive(StdioWriteArchive, StdioReadArchive)
    * ReadArchiveImpl, WriteArchiveImpl:
    * BinaryRW: put16i/get16i, put32i/get32i, put\_struct/get\_struct, read/write, etc
    * TextRW: scan\_uint, get\_uint, get\_not\_if/getnws, skip\_if/skipws, etc.

  1. DigitTable, ULargeInteger, LargeInteger
    * some little classes required by Archive.

  1. Algorithm: set\_intersection\_do (see std::set\_intersection)

  1. KMP String Search Algorithm:
    * std::kmp::Finder, std::kmp::NoCaseFinder

  1. Pool:
    * boost::pool, boost::object\_pool, etc.
    * these components require boost. see http://www.boost.org.



## Changes for 1.1.01 (2006-10-14) ##

**Tutorials**

  1. Hello, SmartWin!					tutorials/winx/step001/hello,smartwin		(a)
  1. XSL Transformations (XSLT)		tutorials/winx/step018-xslt					(b)
  1. histgram(OpenCV example)			tutorials/opencv/step001-histgram			(c)

  * **NOTE**


> To build tutorials marked with (a), You must download SmartWin++ from sourceforge:
> http://sourceforge.net/projects/smartwin/

> And the directory tree looks like this:
```
		����winx
		��  ����include
		����wtl
		��  ����include
		����smartwin
		   ����lib
		   ����include
```
> To build tutorials marked with (b), You must update Windows Platform SDK
> if you use Visual C++ 6.0

> You can download winsdk.zip from http://sourceforge.net/projects/winx/
> And the directory tree looks like this:
```
		����winx
		��  ����include
		����wtl
		��  ����include
		����winsdk
		   ����include
```
> To build tutorials marked with (c), You must download OpenCV from sourceforge:
> > http://sourceforge.net/projects/winx/

> or:
> > http://sourceforge.net/projects/opencv/


> And the directory tree looks like this:
```
		����winx
		��  ����include
		����opencv
		   ����bin
		   ����lib
		   ����include
```

**WINX-Extension**

  1. msxml
    * XSL Transformations (XSLT)

  1. OpenCV
    * class CvWindowT, CvWindow, CvMainFrame



## Changes for 1.1 (2006-09-18) ##

Port many useful WTL/MFC controls to WINX.

**Example: port MFC controls to WINX**

  1. samples/mfc-port/CreditStatic
    * summary: Control that print credit information as 'animated caption'.
    * author: CaiYin(shuker@263.net)

  1. samples/mfc-port/RoundButton
    * summary: A round button control.
    * author: Chris Maunder (chrismaunder@codeguru.com), 1997.

  1. samples/mfc-port/DimEditCtrl
    * summary: An edit control that can provide visual cues in its text area.
    * author: James R. Twine, TransactionWorks, Inc. 2000.
    * url: http://www.codeproject.com/editctrl/dimedit.asp

**Example: use WTL extension controls in a WINX application**

  1. samples/wtlext/BitmapButton
    * summary: to use the Windows Template Library CBitmapButton.
    * author: WTL
    * author: Ed Gadziemski, 2002.
    * url: http://www.codeproject.com/wtl/bmpbtn.asp

  1. samples/wtlext/BitmapHyperLink
    * summary: Control that displays a bitmap next to the link and allows a different color when hover.
    * author: Igor Vigdorchik, 2005.
    * url: http://www.codeproject.com/wtl/CBitmapHyperLink.asp

  1. samples/wtlext/ColorButton
    * summary: Color picker control for WTL applications including optional support for XP themes.
    * author: Tim Smith, 2002.
    * url: http://www.codeproject.com/wtl/wtlcolorbutton.asp

  1. samples/wtlext/DimEdit
    * summary: A WTL adaptation of James Twine's Dim Edit control.
    * author: Paul A. Howes, 2001.
    * url: http://www.codeproject.com/wtl/wtldimedit.asp

  1. samples/wtlext/IconEdit
    * summary: edit control with Icon.
    * author: JaeWook Choi, 2004.
    * url: http://www.codeproject.com/wtl/JaeWTLIconEdit.asp

**Example: port WTL application to WINX**

  1. samples/wtl-port/IEHost(WTL)
  1. samples/wtl-port/IEHost
    * summary: A tutorial on hosting ActiveX controls in WTL dialogs.
    * author: Michael Dunn, 2003.
    * url: http://www.codeproject.com/wtl/wtl4mfc6.asp

**WINX-Core**

  1. Message: OnIdle, OnSettingChange support