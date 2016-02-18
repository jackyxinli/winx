# Introduction #

[WinxGui](http://www.winxgui.com/winxgui) (abbreviated to [WINX](http://www.winxgui.com/winxgui)) focus the goal: [MOST SIMPLE BUT EFFECTIVE](http://www.winxgui.com/). It is a C++ template GUI library that easy to use, and compatible to WTL. WTL is an effective Windows GUI library. However, it is not SIMPLE. WINX support WYSIWYG window GUI development.

If you want to contribute to [WinxGui](http://www.winxgui.com/) Library, Take a look at [our task list](http://code.google.com/p/winx/issues/list).

# News #

## 2009-04-19: Release [winx-2.3.01](http://winx.googlecode.com/svn/tags/winx-2.3.01/), [boost-memory-2.3.01](http://winx.googlecode.com/svn/tags/boost-memory-2.3.01/), [stdext-2.3.01](http://winx.googlecode.com/svn/tags/stdext-2.3.01/), [tpl-2.3.01](http://winx.googlecode.com/svn/tags/tpl-2.3.01/) ##

Please download winx-2.3.01 from [here](http://code.google.com/p/winx/downloads/list), download boost-memory-2.3.01, stdext-2.3.01 from [here](http://code.google.com/p/stdext/downloads/list) and download tpl-2.3.01 from [here](http://code.google.com/p/libtpl/downloads/list).

## 2009-04-12: Release [boost-memory-2.3.00](http://winx.googlecode.com/svn/tags/boost-memory-2.3.00/), [stdext-2.3.00](http://winx.googlecode.com/svn/tags/stdext-2.3.00/), [tpl-2.3.00](http://winx.googlecode.com/svn/tags/tpl-2.3.00/) ##

Please download boost-memory-2.3.00, stdext-2.3.00 from [here](http://code.google.com/p/stdext/downloads/list) and download tpl-2.3.00 from [here](http://code.google.com/p/libtpl/downloads/list).

See ChangeLog for [stdext-2.3.00](http://winx.googlecode.com/svn/trunk/changelog/stdext-2.3.00.txt), [tpl-2.3.00](http://winx.googlecode.com/svn/trunk/changelog/tpl-2.3.00.txt).

For more news, please refer [here](News_1_2_00.md).

# Framework #

[WinxGui](http://www.winxgui.com/winxgui) is composed of some sub-libraries. They are _winsdk_, _wtl_, _stdext_, _winx_.

_winsdk_, _wtl_ are 3rdparty libraries. _winsdk_ is Windows Platform SDK, which is provided by Microsoft. _wtl_ is Windows Template Library. It is a C++ library for developing Windows applications and UI components. It extends ATL (Active Template Library) and provides a set of classes for controls, dialogs, frame windows, GDI objects, and more.

_[stdext](http://code.google.com/p/stdext/)_ is a GUI-less library. _stdext_ means STL (Standard Template Library) extensions. It extends STL and provides a set of classes for diagnosis, memory management, unit tests, file io, etc. _stdext_ is self systematic. It depends nothing else, and you can use it freely.

_winx_ is the core of [WinxGui](http://www.winxgui.com/winxgui) library, of course. It depends _winsdk_(optional), _wtl_, and _stdext_. You may ask me why you based on WTL. In [Dive into WinxGui](http://www.winxgui.com/dive-into-winxgui) I will answer the question.

In fact, _[stdext](http://code.google.com/p/stdext/)_ library is more general and useful than _winx_. So, we decided to release _stdext_ library independently. Its homepage is http://code.google.com/p/stdext/. The wiki of _stdext_ library is http://cpp.winxgui.com/.

# FAQ #

**How to get
[WinxGui](http://www.winxgui.com/winxgui)?**
  * Checkout from SVN.
    * Shell Command: svn co http://winx.googlecode.com/svn/trunk/
    * Visual Client: [TortoiseSVN](http://sourceforge.net/projects/TortoiseSVN)
  * Download zipped packages from following sites:
    * http://code.google.com/p/winx/
    * http://sourceforge.net/projects/winx
    * http://www.winxgui.com/download/

**How to get help?**
  * Post your question to [WinxGui](http://www.winxgui.com/winxgui) forum, such as http://groups.google.com/group/winx.
  * For more information, see [WinxGui mailing list](http://www.winxgui.com/mailing-list).

**I want to learn
[WinxGui](http://www.winxgui.com/winxgui), but how to start?**
  * See [Start to learn WinxGui](http://www.winxgui.com/beginning).

**What components are included in
[WinxGui](http://www.winxgui.com/winxgui)?**
  * See [WinxGui Developer Manual](http://www.winxgui.com/winxgui).

**How does
[WinxGui](http://www.winxgui.com/winxgui) work?**
  * See [WinxGui Implementation](http://www.winxgui.com/dive-into-winxgui).

**I got some bugs or suggestions, where can I give feedbacks?**
  * Thank you. Please commit to http://code.google.com/p/winx/issues/list.

**I want join in
[WinxGui](http://www.winxgui.com/winxgui) team. How can I?**
  * To join us, Go to http://groups.google.com/group/winx-dev to apply.
  * For more information, see [WinxGui mailing list](http://www.winxgui.com/mailing-list).