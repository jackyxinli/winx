#define TPL_USE_AUTO_ALLOC
#include <tpl/c/Lex.h>

using namespace tpl;

// -------------------------------------------------------------------------
// json print

// A json print example.

int main()
{
	const char source[] = "\
		class Foo // Foo comment\n\
		  : public Base1, Base2\n\
		{};\
	";
	
	Mark<> tagName("name");	
	Mark<> tagAccess("access");
	NodeMark<> tagBase("base");
	
	impl::Allocator alloc;
	impl::Document doc(alloc);

	source >> cpp_skip_
		[
			gr(c_symbol_("class")) + c_symbol()/tagName +
			!(':' +
				(
					!gr(c_symbol_("public")/tagAccess) +
					c_symbol()/tagName
				)/tagBase % ','
			) +
			'{' + '}' + ';'
		]/doc;

	std::OutputLog log;
	json_print(log, doc);
	return 0;
}

// -------------------------------------------------------------------------

