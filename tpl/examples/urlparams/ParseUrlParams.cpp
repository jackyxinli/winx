#define TPL_USE_AUTO_ALLOC
#include <map>			// std::map
#include <iostream> 	// std::cout
#include <tpl/RegExp.h>

using namespace tpl;

// -------------------------------------------------------------------------
// parse_url_params

#define OVERWRITE		true
#define RULE			*(	\
	*ch('&') + find('=')/assign(key) + '=' + find_set<'&', EOF>()/assign<OVERWRITE>(values, key) )

int main()
{
	std::string key;
	std::map<std::string, std::string> values;
	
	if ( simple::match("&page=1&x=2&&lang=en&x=3", RULE) )
	{
		for (std::map<std::string, std::string>::iterator it = values.begin(); it != values.end(); ++it)
		{
			std::cout << it->first << " = " << it->second << "\n";
		}
	}
	return 0;
}

// -------------------------------------------------------------------------

