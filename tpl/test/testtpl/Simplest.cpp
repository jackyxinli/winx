#include "stdafx.h"

using namespace tpl;

// -------------------------------------------------------------------------
// simplest

// A simplest example.

// What we use:
//	* Rules: /assign(), %, integer(), ch(' ')
//	* Matching: tpl::simple::match()

void simplest()
{
	std::vector<double> values; // you can change set to other stl containers.
	
	if ( simple::match("-.1 -0.1 +32. -22323.2e+12", real()/&values % ' ') )
	{
		for (std::vector<double>::iterator it = values.begin(); it != values.end(); ++it)
		{
			std::cout << *it << "\n";
		}
	}
}

// -------------------------------------------------------------------------
