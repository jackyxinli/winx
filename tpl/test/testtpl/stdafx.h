
#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <tpl/RegExp.h>

class My_ : public tpl::policy::Default
{
public:
	typedef std::AutoFreeAlloc Allocator;
};

typedef tpl::Customization<My_> impl_;
typedef tpl::Customization<My_, false> simple_;

#define impl impl_
#define simple simple_
#define SimpleImplementation simple_
