## Changes for 1.0.041 (2006-09-14) ##

Released mainly for bugfix.

**Bugfix:**

  1. ActiveX support bugfix (Visual Studio 2005)
    * Failed to create ActiveX controls in dialog resource template.
    * Reason: AtlAxDialogBox/AtlAxCreateDialog implementation was changed in high version ATL.

  1. step009-mousewheel
{{{	  change the following statement (in WinMain)
> > dlg.DoModal(NULL, hInstance);
> > to
> > dlg.DoModal();
> > ����winx
> > ��  ����include
> > ����winsdk
> > ��  ����include
> > ����wtl
> > > ����include}}}

* Tutorials

	# DDX(DataExchange)								tutorials/winx/step017-ddx

* WINX-Core

	# DDX(DataExchange)



== Changes for 1.0.04 (2006-09-12) ==

* Tutorials

	# Window behaviors(AutoHidden, etc)				tutorials/winx/step014-behavior
	# Property Sheet								tutorials/winx/step015-property-sheet
	# WINX together with MFC 						tutorials/winx/step016-mfc-together
	# Save images(include multiframe tiff image)	tutorials/gdiplus/step001-save-image

* WINX-Extension

	# PropertySheet, PropertyPage
	# Gdiplus
	  * Image Encoders/Image Decoders/MultiFrameImageCreator
	  * EncoderParameters: ColorDepth, Transformation, SaveFlag, Compression, Quality
	  * Initialize Class: GdiplusAppInit

* WINX-Core
	
	# Behavior: BehaviorPermit, BehaviorSwitch, AutoHidden, etc
	
* WINX-Basic
	
	# Log: TraceWindow
	# Storage: TraceStorage

* STL-Extension (stdext)

	# Container: TypedPtrArray, InterfaceArray
	# Memory Management: HeapMemAlloc
	# Log: print RECT, SIZE

* Documents (chinese version)

	# WINX Preview						docs(zh-CN)/WinxPreview.ppt
	# Garbage collection allocator		docs(zh-CN)/stdext/001-memory/*.htm
	# VC++ 6.0 Tips						docs(zh-CN)/stdext/000-vc6tips/*.htm

* Links

	# winx official blog (english)	- http://winxhome.blogspot.com/
	# winx official blog (chinese)	- http://winxcn.blogspot.com/
	# winx official blog (big5)		- http://winxtw.blogspot.com/
	# winx group (english) 			- http://groups.google.com/group/winx/
	# winx group (chinese)			- http://groups.google.com/group/winxcn/
	# winx download page			- http://sourceforge.net/projects/winx/



== Changes for 1.0.03 (2006-08-26) ==

* Supported Compilers

	# Visual C++ 6.0
	# Visual C++ 2005

* Tutorials

	# MouseWheel Message:				tutorials/winx/step009-mousewheel
	# XP Style(Include OwnerDraw):		tutorials/winx/step010-xptheme
	# Link Control(url, mailto, etc):	tutorials/winx/step011-linkctrl
	# ActiveX Control(flash, IE, etc):	tutorials/winx/step012-activex
	# AutoComplete of Edit Control:		tutorials/winx/step013-autocomplete
	
  *** NOTE *** 

	To build these tutorials(except step009-mousewheel), You must 
	update Windows Platform SDK if you use Visual C++ 6.0
	
	You can download winsdk.zip from http://sourceforge.net/projects/winx/
	And the directory tree looks like this:
{{{
}}}

* WINX-Extension

	# ActiveX support, such as flash, IE, etc.
	
	# WebBrowser(IE) ConnectPoint support.
	  You'll find it is easy for you to connect events from IE.
	  
	# Theme(XPStyle) support, including owner-draw support.
	
	# AutoComplete support, for Edit/ComboBox controls.
	  You can save histroy to windows registry, or a file.
	
	# DropFileHandle(simple wrapper for HDROP)

* WINX-Core

	# DispEventSimpleImpl: Dispatch Event Implementation(ConnectPoint support)
	  It's easier to use than IDispEventSimpleImpl(ATL's implementation).

* WINX-Basic

	# IEnumOnSTLImpl, EnumStringSTLImpl
	  * IEnumXXX(IEnumString, etc) Implementation on STL container.

*) STL-Extension

	# Archive
	  * SimpleWriteArchive/SimpleReadArchive: Archive based on FILE.
	  * MemSeqWriteArchive: Archive based on Memory.
	  * RegWriteArchive/RegReadArchive: Archive based on Windows Registry.

	# RecordArchive
	  * MemRecordWriter, RecordReader: Record files(Excel BIFF files, etc) support.



== Changes for 1.0.02 (2006-08-23) ==

* Tutorials

	# Hello:					tutorials/winx/step001
	# Window Background: 		tutorials/winx/step002-bkgnd
	# Command Dispatch & State:	tutorials/winx/step003-command
	# User-defined Controls: 	tutorials/winx/step004-user-ctrl
	# Accelerator: 				tutorials/winx/step005-accel
	# Dialog Layout: 			tutorials/winx/step006-layout
	# Window Icon: 				tutorials/winx/step007-icon
	# OpenFile Dialog: 			tutorials/winx/step008-openfiles

* WINX-Core

	# Window::Create, for creating windows dynamicly.
	
	# class MainFrame, for SDI applications.
	
	# macro `WINX_STACK_OBJECT`, for creating stack-based window objects.
	  The current stack-based window objects are ModalDialog and MainFrame.
	  The common characteristic is that they both are top level windows.
	  So in the future, MDIMainFrame is also a stack-based window object.
	
	# Window Property (IMPORTANT!!!)
	  * Bkgrnd: to set window background. see macro WINX_BKGND_XXX.
	  * CtlBkgrnd: to set window class background. see macro WINX_CTLBKGND_XXX.
	  * Accel: to set window accelerator. see macro WINX_ACCEL, WINX_DLG_ACCEL.
	  * Icon: to set window icon. see WINX_ICON.
	  * AppIcon: to set default icon of all windows. see WINX_APP_ICON.
	 
	# Command Dispatch
	
	# Command State(UpdateUI Component)
	
	# Layout(DialogResize Component)
	
* Common Dialogs

	# OpenFileDialog/SaveFileDialog
	# OpenMultiFilesDialog/SaveMultiFilesDialog
	
* WINX-Basic

	# MFC Compatibility: 
	  * Diagnost(ASSERT, VERIFY, etc)
	  * GdiObject(CDC, CClientDC, CPaintDC, CBitmap, CPalette, etc)
	  
	# Initialize Class: CAppModuleInit

* STL-Extension (stdext)

	# namespace std::tchar, eg. std::tchar::strlen, std::tchar::strlwr, etc.
	# String Algorithm: std::strecpy



== Changes for 1.0.01 (2006-08-28) ==

The initial version of the WINX library.

* Example: port MFC controls to WINX

	# samples/mfc-port/CreditStatic
	# samples/mfc-port/RoundButton
	
* Hello:
	
	# tutorials/winx/winx/step001/hello,sdk
	# tutorials/winx/winx/step001/hello,wtl
	# tutorials/winx/winx/step001/hello,winx

* WINX-Core

	# Message: 
		* class WindowMessage
		* macro: WINX_REFLECT, WINX_CMD, etc.
		
	# Window:
		* Window/SubclassWindow
		* LightSubclassWindow
		* LightSuperclassWindow
	
	# Dialog: 
		* ModalDialog/ModalessDialog
		* SimpleDialog
	
	# Control: 
		* Static/Button/Edit/ComboBox
		* ScrollBar/FlatScrollBar
		* ListBox/DragListBox
		* ListCtrl/HeaderCtrl/TreeCtrl
		* ToolBarCtrl/ReBarCtrl
		* SliderCtrl(TrackBarCtrl)
		* SpinButtonCtrl(UpDownCtrl)
		* TabCtrl/ToolTipCtrl/StatusBarCtrl/ProgressCtrl
		* HotKeyCtrl/AnimateCtrl/IPAddressCtrl
		* DateTimeCtrl/MonthCalCtrl
		* ComboBoxEx/RichEdit
		* LinkCtrl

* WINX-Basic

	# Basic Types: CString, CFindFile, CRecentDocumentList, CComBSTR, etc.
	# Helper: MsgBox, GetThisModule, InitCommonControls, etc.
	# Initialize Class: CComAppInit, COleAppInit, CDebugAppInit, CComModuleInit, etc.
	# Resource: GetModuleIniPath, Bitmap, Icon, Menu, Brush, Accelerator, etc.
	# Gdi Object: Pen, Brush, Font, Bitmap, DC(PaintDC, ClientDC, etc), Rgn, etc.
	# Gdi Helper: DoubleBuffer, etc.
	# Handle Class: WindowHandle
	# COM Support: Object, StackObject, FakeObject, COLESTR, etc.
	
* STL-Extension (stdext)

	# Memory Management: AutoFreeAlloc, RecycleBuffer, AutoArray, etc.
	# String Algorithm: trim, match, compare, etc.
	# CharType: isCSymbolFirstChar, isCSymbolNextChar, etc.
	# Container: SimpleMultiMap
	# Log: OutputLog, ErrorLog, FileLog, StringLog
	# Performance: PerformanceCounter
	# Diagnost: `WINX_ASSERT, WINX_TRACE`, etc.
```