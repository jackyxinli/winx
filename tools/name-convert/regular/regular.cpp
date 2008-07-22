#include <stdio.h>
#include <algorithm>
#include <vector>

#define ADD_REGULAR_CH(c)	\
	regular_char_table[c] = true
	
#define ADD_REGULAR_CHRANGE(first, last)	\
	std::fill_n(regular_char_table + (first), (last) - (first) + 1, true)
	
inline char x_digit(int c)
{
	return (c < 10 ? '0' : 'A'-10) + c;
}

int main(int argc, const char* argv[])
{
	if (argc < 2) {
		puts(
			"Usage: regular <path> ...\n"
			"If <path> has invalid characters, we will change it in %xx form.\n"
			);
		return -1;
	}

	bool regular_char_table[256] = { 0 };
	
	ADD_REGULAR_CHRANGE('0', '9');
	ADD_REGULAR_CHRANGE('a', 'z');
	ADD_REGULAR_CHRANGE('A', 'Z');
	ADD_REGULAR_CH('~');
	ADD_REGULAR_CH('_');
	ADD_REGULAR_CH('-');
	ADD_REGULAR_CH('+');
	ADD_REGULAR_CH('=');
	ADD_REGULAR_CH('(');
	ADD_REGULAR_CH(')');
	ADD_REGULAR_CH('{');
	ADD_REGULAR_CH('}');
	ADD_REGULAR_CH('[');
	ADD_REGULAR_CH(']');
	ADD_REGULAR_CH('/');
	ADD_REGULAR_CH('.');
	ADD_REGULAR_CH('$');
	ADD_REGULAR_CH('%');
	
	for (int i = 1; i < argc; ++i)
	{
		bool f = false;
		std::vector<char> re;
		for (const unsigned char* p = (const unsigned char*)argv[i]; *p; ++p)
		{
			if (regular_char_table[*p]) {
				re.push_back(*p);
			}
			else {
				re.push_back('%');
				re.push_back(x_digit(*p >> 4));
				re.push_back(x_digit(*p & 0xf));
				f = true;
			}
		}
		if (f) {
			re.push_back('\0');
			printf("rename '%s' to '%s' ...\n", argv[i], &re[0]);
			rename(argv[i], &re[0]);
		}
	}
}

