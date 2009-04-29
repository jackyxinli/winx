#include <stdext/AOP.h>
#include <stdext/LinkLib.h>

// -------------------------------------------------------------------------
// interface

typedef int Text;

DEFINE_EVENT(TextChanged, (Text text)); // define event type 'TextChanged'

DEFINE_EID(TextChanged, "FDA4D6CB-91E1-47a0-9D85-E200110EA0E3", 
		0xfda4d6cb, 0x91e1, 0x47a0, 0x9d, 0x85, 0xe2, 0x0, 0x11, 0xe, 0xa0, 0xe3);

// -------------------------------------------------------------------------
// implementation

//
// Edit component
//

class Edit : public std::EventContainer<1>
{
private:
	typedef EVENT_TYPE(TextChanged, (Text text), (text)) TextChangedEvent;

	Text m_text;
	TextChangedEvent m_changed;

public:
	Edit(NS_STDEXT::ScopedAlloc& alloc) : m_changed(alloc)
	{
		addEvent(EID(TextChanged), m_changed);
	}

	void putText(Text text)
	{
		m_text = text;		
		m_changed.fire(text); // fire 'changed' event when the content of Edit is altered
	}
};

//
// Dialog component
//

class Dialog
{
private:
	NS_STDEXT::ScopedAlloc m_alloc;
	Edit m_edit;
	
	NS_STDEXT::Connection m_editChanged;
	NS_STDEXT::Connection m_editChanged2;
	// NOTE: even you don't need to disconnect, you must hold the connection handle.

public:
	Dialog() 
		: m_edit(m_alloc)
	{
		TextChanged* textChanged = NULL;
		m_edit.findEvent(EID(TextChanged), (void**)&textChanged);
		// NOTE: I doesn't do failure handling.

		m_editChanged = textChanged->addListener(this, &Dialog::onEditChanged);
		m_editChanged2 = textChanged->addListener(this, &Dialog::onEditChanged2);
	}

	void __stdcall onEditChanged(Text text)
	{
		printf("onEditChanged: text=%d\n", text);
	}

	void __stdcall onEditChanged2(Text text)
	{
		printf("onEditChanged2: text=%d\n", text);
	}

	void input(Text text)
	{
		m_edit.putText(text);
	}

	void disconnectEditChanged()
	{
		m_editChanged.disconnect();
	}
};

// -------------------------------------------------------------------------
// main

int main()
{
	Dialog dlg;
	dlg.input(11);
	dlg.input(12);
	dlg.disconnectEditChanged();
	dlg.input(13);
	return 0;
}

// -------------------------------------------------------------------------

