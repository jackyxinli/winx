## Changes for 1.2 (2007-06-10) ##

**Wizard for Visual C++ 6.0**

  1. Enable/Disable warning of directory tree.
  1. L10n: support non-english language resources.
  1. Change wizard icon.
  1. Support modeless dialog.
  1. Bugfix - uninitial variable: CComModule `_`Module.

**Tutorials**

  1. Modeless Dialog Application: [tutorials/winx/step022-modeless](http://winxsamp.googlecode.com/svn/trunk/winxsamp/tutorials/winx/step022-modeless/)

**WINX-Core**

  1. MessageFilter
    * property: [WINX\_MSGFILTER](WINX_MSGFILTER.md), [WINX\_MSGFILTER\_NULL](WINX_MSGFILTER.md)
    * function: RunMsgLoop
    * class: MessageHook, [AutoMessageHook](MessageHook.md) (remove SimpleHook)
    * class: MessageFilter
    * class ModelessDialog support MessageFilter in default
  1. MainFrame
    * ModalDialog -> ModelDialog
    * ModalessDialog -> ModelessDialog
    * class: MainDlgFrame, [AxMainDlgFrame](MainDlgFrame.md)
    * property: [WINX\_MAINFRAME](WINX_MAINFRAME.md)
    * message: OnMainFrameTerm
  1. Modeless support (function: RunMsgLoop)
  1. [WINX\_DLG\_ACCEL](WINX_DLG_ACCEL.md) -> [WINX\_ACCELFRAME](WINX_ACCELFRAME.md)
  1. Accel bugfix: Menu can't receive its keyboard messages after hooking.

**STL-Extension**

  1. Remove STD`_`DELETE macro. Use alloc.destroy(obj), instead of STD`_`DELETE(alloc, obj).