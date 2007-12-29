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
// Module: Main.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 18:26:15
// 
// $Id: Main.cpp,v 1.1 2006/09/27 02:37:45 xushiwei Exp $
// -----------------------------------------------------------------------*/

#pragma warning(disable: 4819)

#include <SmartWin.h>
#include "resource.h"

using namespace SmartWin;

class MainDialog
	: public WidgetFactory< WidgetModalDialog, MainDialog, MessageMapPolicyModalDialogWidget >
{
public:
	MainDialog()
	{
		onInitDialog( & MainDialog::initDialog );
	}

private:
	WidgetButtonPtr itsOkButton;
	WidgetButtonPtr itsCancelButton;

	// Needed to allow the Widgets to be subclassed before the dialog appears,
	// but after the dialog is created.
	bool initDialog()
	{
		itsOkButton = subclassButton( IDOK );
		itsCancelButton = subclassButton( IDCANCEL );

		itsOkButton->onClicked( & MainDialog::ok_clicked );
		itsCancelButton->onClicked( & MainDialog::cancel_clicked );
		onClosing( & MainDialog::closing );

		return true;
	}

	// This one kicks in ONLY if user presses the cross in the upper right corner...
	// Behaviour then is to "cancel" the dialog...
	bool closing()
	{
		// Here we EXPLICITLY ends the dialog INSTEAD of returning true here!!
		// This means that this dialog would NOT be destroyed if it wasn't for us calling the DestroyDialog!!
		// If we were to return true from this function the result of the integer return value would NOT be the one
		// we expected it to be!!  ( we're expecting a "cancel" event)
		// Note!
		// This allows to make "pressing the cross up right" in the dialog mean "OK" instead cancel!!
		this->endDialog( IDCANCEL ); // IDOK or IDCANCEL or any other value.

		// Do NOT destroy the window... we rather want the "EndDialog" message to be dispatched...
		return false;
	}

	void ok_clicked( WidgetButtonPtr btn )
	{
		endDialog( IDOK ); // IDOK or IDCANCEL or any other value.
	}

	void cancel_clicked( WidgetButtonPtr btn )
	{
		endDialog( IDCANCEL ); // IDOK or IDCANCEL or any other value.
	}
};

int SmartWinMain( SmartWin::Application & app )
{
	MainDialog nd;
	nd.createDialog(IDD_MAINDLG);
	return 0;
}

// -------------------------------------------------------------------------
// $Log: Main.cpp,v $
// Revision 1.1  2006/09/27 02:37:45  xushiwei
// new configuration: vs2005 release multithread(static library)
//
