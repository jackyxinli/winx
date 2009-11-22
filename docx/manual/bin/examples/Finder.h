/*
@category STL-Extension(stdext) Library
@ns stdext.kmp
@*/

// -------------------------------------------------------------------------
// class Finder

/** The Finder class implements KMP string searching algorithm.
@arg _E
	The data type of a single character to be found in searching algorithm. It can be char or WCHAR.
@arg _Strategy
	The strategy of searching algorithm. Default is \<MatchCase>, and it also can be \<MatchNoCase>.
@*/
template < class _E, class _Strategy = MatchCase<_E> >
class Finder
{
	/** Default constructor.
	@see Finder, initPattern, getPattern
	@*/
	Finder();
	
	/** Construct the finder object with a pattern string.
	@arg [in] szPattern		The start address of pattern string buffer.
	@arg [in] cchLen		The length of pattern string.
	@see Finder, initPattern, getPattern
	@*/
	Finder(const char_type* szPattern, size_type cchLen);
	
	/** Construct the finder object with a null-terminated pattern string (C-Style).
	@arg [in] szPattern		The start address of pattern string buffer.
	@see Finder, initPattern, getPattern
	@*/
	Finder(const char_type* szPattern);
	
	/** Construct the finder object with a pattern string object (C++ Style).
	@arg [in] strPattern	The start address of pattern string buffer.
	@see Finder, initPattern, getPattern
	@*/
	Finder(const std::basic_string<_E>& strPattern);

	/*
	@fn initPattern(szPattern,cchLen)
	@brief					Initialize the finder object with a pattern string.
	@arg [in] szPattern		The start address of pattern string buffer.
	@arg [in] cchLen		The length of pattern string.
	@see Finder, initPattern, getPattern
	@*/
	HRESULT winx_call initPattern(const char_type* szPattern, size_type cchLen);
	
	/*
	@fn initPattern(szPattern)
	@brief					Initialize the finder object with a null-terminated pattern string (C-Style).
	@arg [in] szPattern		The start address of pattern string buffer.
	@see Finder, initPattern, getPattern
	@*/
	HRESULT winx_call initPattern(const char_type* szPattern);
	
	/*
	@fn initPattern(strPattern)
	@brief					Initialize the finder object with a pattern string object (C++ Style).
	@arg [in] strPattern	The start address of pattern string buffer.
	@see Finder, initPattern, getPattern
	@*/
	HRESULT winx_call initPattern(const std::basic_string<_E>& strPattern);

	/*
	@fn good
	@brief					Indicates the state of the finder object.
	@return
		@val true(nonzero)
			The finder object is initialized with a pattern string.
		@val false(zero)
			The finder object is uninitialized.
	@*/
	int winx_call good() const;
	
	/*
	@fn size
	@brief					Get the length of the pattern string.
	@return
		Returns the length of the pattern string if the finder object is initialized.
		Returns zero if the finder object is uninitialized.
	@see good
	@*/
	size_type winx_call size() const;
	
	/*
	@fn getPattern
	@brief					Get the pattern string.
	@arg [out] strPattern	Returns the pattern string.
	@see Finder, initPattern
	@*/
	template <class StringT>
	void winx_call getPattern(StringT& strPattern) const;
	
	/*
	@fn next
	@brief					Searching the pattern string from current position of an archive object.
	@arg [in] ar			The archive object.
	@arg [in] limit
		The limit scope of searching. Default is <em>nolimit</em>.
		The searching operation will be limited within [cp, cp+limit). Here cp means the current position
		of the archive. If limit is specified <em>nolimit</em>, that means limit to the end of the archive.
	@return
		@val S_OK
			Searching succeeds. The pattern string is found in the archive.
		@val S_FALSE
			Searching fails. The pattern string is not found.
		@val E_ACCESSDENIED
			Searching fails. The finder object is uninitialized.
	@remark
		If searching succeeds (returns S_OK), the new current position of the archive object is the end of
		pattern string found in the archive. For example, assume the archive contents are "1234abcdefg", 
		and the pattern string is "abc", then after the searching operation, the current position of the
		archive pointer to "defg", not "abcdefg". \p
		If searching fails, the new current position of the archive object is undefined.
	@*/
	template <class ArchiveT>
	HRESULT winx_call next(ArchiveT& ar, size_type limit = nolimit) const;
	
	/*
	@fn istreamNext
	@brief					Searching the pattern string from current position of an std::istream object.
	@arg [in] is			The istream object.
	@arg [in] limit
		The limit scope of searching. Default is <em>nolimit</em>.
		The searching operation will be limited within [cp, cp+limit). Here cp means the current position
		of the istream. If limit is specified <em>nolimit</em>, that means limit to the end of the istream.
	@return
		@val S_OK
			Searching succeeds. The pattern string is found in the istream.
		@val S_FALSE
			Searching fails. The pattern string is not found.
		@val E_ACCESSDENIED
			Searching fails. The finder object is uninitialized.
	@remark
		If searching succeeds (returns S_OK), the new current position of the istream object is the end of
		pattern string found in the istream. For example, assume the istream contents are "1234abcdefg", 
		and the pattern string is "abc", then after the searching operation, the current position of the
		istream pointer to "defg", not "abcdefg". \p
		If searching fails, the new current position of the istream object is undefined.
	@see next
	@*/
	template <class istream_type>
	HRESULT winx_call istreamNext(istream_type& is, size_type limit = nolimit) const;

	/*
	@fn iteratorNext
	@brief					Searching the pattern string in a text specified by an iterator scope.
	@arg [in] it			Start of the text (iterator begin).
	@arg [in] limit
		The limit scope of searching. It can't be <em>nolimit</em>.
		The searching operation will be limited within [it, it+limit).
	@arg [out] pitFind
		If searching succeeds (returns S_OK), Returns the end of pattern string found in the text.
		For example, assume the text are "1234abcdefg", and the pattern string is "abc", then after
		the searching operation, *pitFind will pointer to "defg", not "abcdefg". \p
		If searching fails, *pitFind is undefined.
	@return
		@val S_OK
			Searching succeeds. The pattern string is found in the text.
		@val S_FALSE
			Searching fails. The pattern string is not found.
		@val E_ACCESSDENIED
			Searching fails. The finder object is uninitialized.
	@see next
	@*/
	template <class iterator_type>
	HRESULT winx_call iteratorNext(iterator_type it, size_type limit, iterator_type* pitFind) const;

	/*
	@fn cstrNext
	@brief					Searching the pattern string in a text specified by a C-Style string.
	@arg [in] text			The text. It's a null-terminated string.
	@arg [out] ppFind
		If searching succeeds (returns S_OK), Returns the end of pattern string found in the text.
		For example, assume the text are "1234abcdefg", and the pattern string is "abc", then after
		the searching operation, *ppFind will pointer to "defg", not "abcdefg". \p
		If searching fails, *ppFind is undefined.
	@return
		@val S_OK
			Searching succeeds. The pattern string is found in the text.
		@val S_FALSE
			Searching fails. The pattern string is not found.
		@val E_ACCESSDENIED
			Searching fails. The finder object is uninitialized.
	@see next
	@*/
	HRESULT winx_call cstrNext(const char_type* text, const char_type** ppFind) const;
};

// -------------------------------------------------------------------------
