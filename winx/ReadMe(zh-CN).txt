您好！ 欢迎使用 WINX ！

WINX关注一个目标：简单而高效。
它是一个C++界面开发库，容易使用，兼容WTL。WTL是高效的，然而，它并不容易上手。


*) WINX相关的链接

	winx official blog (英文)	- http://winxhome.blogspot.com/
	winx official blog (中文)	- http://winxcn.blogspot.com/
	winx official blog (繁体)	- http://winxtw.blogspot.com/
	winx group (英文) 			- http://groups.google.com/group/winx/
	winx group (中文)			- http://groups.google.com/group/winxcn/
	winx at sourceforge 		- http://sourceforge.net/projects/winx/
	winx at google code			- http://code.google.com/p/winx/


*) 整个WINX的目录树看起来是这样的：

	├─winx
	│  ├─include
	│  │  └─winx
	│  ├─lib
	│  ├─src
	│  └─test
	├─winxdoc
	│  ├─docs(zh-CN)
	│  └─docs(en)
	├─winxsamp
	│  ├─samples
	│  └─tutorials
	├─stdext
	│  └─include
	│     └─stdext
	├─winsdk
	│  ├─bin
	│  ├─include
	│  └─lib
	└─wtl
	    ├─include
	    ├─Notes
	    ├─Samples
	    └─Wizards

winx/include/
  WINX界面开发库源代码。

winx/docs(zh-CN)/
  简体中文版本的文档，包括：
  *) 授权（License）
  *) 更新历史（ChangeLog）
  *) 使用手册（The user's manual）

winx/docs(en)/
  英文版本的文档。

winx/samples/
  WINX样例程序。

winx/tutorials/
  使用指南。一步步介绍你如何使用WINX。

winx/lib/
  WINX静态库。因为WINX多数代码均在include目录，故此通常情况下并不需要它。

winx/src/
  WINX静态库的源代码。对WINX的使用者而言并不需要。

winx/test/
  测试案例。对WINX的使用者而言并不需要。

stdext/
  C++标准库（STL）扩展。用于一般性用途。

winsdk/
  Windows平台SDK，来自Visual Studio .NET 2005的版本。
  它不是必须的，只是WINX的部分代码依赖高版本的Windows SDK，故此带上。
  默认WINX不依赖于它，你可以include <winx.h>前define WINX_USE_WINSDK来让WINX使用它。
  如果不这样做，并且你使用VC++ 6.0，可能会有部分功能不能使用。

wtl/
  第三方代码：WTL界面开发库。
  由于WINX是基于WTL上开发的，为了方便，我们将WTL纳入WINX中。
  你可以独立更新WTL。下载地址：http://sourceforge.net/projects/wtl/
