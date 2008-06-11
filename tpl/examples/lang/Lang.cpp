#define TPL_USE_AUTO_ALLOC
#include <set>
#include <iostream> 	// std::cout
#include <tpl/RegExp.h>
#include <tpl/c/Lex.h>

using namespace tpl;

template <class CharT>
class IsNotKeywords
{
private:
	typedef std::basic_string<CharT> key_t;
	typedef std::set<key_t> set_t;
	
	set_t m_set;
	
public:
	IsNotKeywords(const CharT* keywords[], size_t n) {
		while (n--) {
			m_set.insert(keywords[n]);
		}
	}
	
	template <class Iterator>
	bool TPL_CALL operator()(Iterator pos, Iterator pos2) const {
		key_t k(pos, pos2);
		return m_set.find(k) == m_set.end();
	}
};

int main()
{
	typedef SimpleImplementation impl;
	
	const char* keywords[] = {
		"if", "elif", "else",
		"case", "of", "default",
	};
	IsNotKeywords<char> not_keywords(keywords, countof(keywords));

	impl::Allocator alloc;

	impl::Grammar::Var rFactor;

	// <term2> ::= <factor> ('*' <factor> | '/' <factor>)*
	impl::Grammar rTerm2( alloc,
		rFactor + *(
			'*' + rFactor | 
			'/' + rFactor)
		);

	// <term> ::= <term2> ('+' <term2> | '-' <term2>)*
	impl::Grammar rTerm( alloc,
		rTerm2 + *(
			'+' + rTerm2 |
			'-' + rTerm2)
		);

	// <compare> ::= '<=' <term> | '<' <term> | '>=' <term> | '>' <term> | '==' <term> | '!=' <term>
	impl::Grammar rCompare( alloc,
		"<=" + rTerm |
		'<' + rTerm |
		">=" + rTerm |
		'>' + rTerm |
		"==" + rTerm |
		"!=" + rTerm
		);
		
	// <andexpr> ::= <term> <compare>* ('&&' <term> <compare>*)*
	impl::Grammar rAndExpr( alloc,
		rTerm + *rCompare + *(
			"&&" + rTerm + *rCompare)
		);
	
	// <expr> ::= <andexpr> ('||' <andexpr>)*
	impl::Grammar rExpr( alloc,
		rAndExpr + *(
			"||" + rAndExpr)
		);

	// <assignment> ::= 
	//		c_symbol() (
	//			'=' <assignment> |
	//			'(' c_symbol() % ',' ')' '=' <assignment> ) | 
	//		<expr>
	impl::Grammar::Var rAssignment;
	rAssignment.assign( alloc,
		c_symbol() + (
			'=' + rAssignment |
			'(' + c_symbol() % gr(',') + ')' + '=' + rAssignment) |
		rExpr
		);

	// <lang> ::= (<assignment> ';')*
	impl::Grammar rLang( alloc,
		*(rAssignment + ';')
		);
		
	// <factor> ::=
	//		real() |
	//		c_string() |
	//		'(' <assignment> % ',' ')' |
	//		'-' <factor> |
	//		'+' <factor> |
	//		'if' '(' <expr> ')' <assignment> ('elif' '(' <expr> ')' <assignment>)* 'else' <assignment> |
	//		'case' <expr> 'of'
	//			((<compare> | <expr>) ':' <assignment>)*
	//			'default' ':' <assignment> |
	//		c_symbol() ('(' <assignment> % ',' ')')?
	rFactor.assign( alloc,
		real() |
		c_string() |
		'-' + rFactor |
		'+' + rFactor |
		'(' + rAssignment % ',' + ')' |
		c_symbol()
		[
			case_(not_keywords)
			[
				!('(' + rAssignment % ',' + ')')
			],
			case_("if")
			[
				'(' + rExpr + ')' + rAssignment +
				*(gr("elif") + '(' + rExpr + ')' + rAssignment) +
				c_symbol("else") + rAssignment
			],
			case_("case")
			[
				rExpr + "of" +
				*((rCompare | rExpr) + ':' + rAssignment + ',') +
				gr("default") + ':' + rAssignment
			]
		]);

	// ---- do match ----
	
	for (;;)
	{
		std::string strExp;
		std::cout << "input an expression (q to quit): ";
		if (!std::getline(std::cin, strExp) || strExp == "q") {
			std::cout << '\n';
			return 0;
		}

		try {
			if ( !impl::match(strExp.c_str(), rLang + eos(), skipws() % ("//" + find_eol<true>())) )
				std::cout << ">>> ERROR: invalid expression!\n";
			else
				std::cout << "passed!" << '\n';
		}
		catch (const std::logic_error& e) {
			std::cout << ">>> ERROR: " << e.what() << '\n';
		}
	}
}

// -------------------------------------------------------------------------

