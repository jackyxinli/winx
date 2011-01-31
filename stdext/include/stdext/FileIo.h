#ifndef STDEXT_FILEIO_H
#define STDEXT_FILEIO_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// update checksum

template <class CheckSumT, class FileT>
inline int winx_call update(
	CheckSumT& checksum, FileT& file, char* buf, size_t bufSize)
{
	for (;;)
	{
		size_t getten;
		const int err = file.get(buf, bufSize, getten);
		if (err == 0)
		{
			checksum.Update((const BYTE*)buf, getten);
			if (bufSize != getten)
				return 0;
		}
		else
		{
			return err;
		}
	}
}

template <class CheckSumT, class FileT>
inline int winx_call update(
	CheckSumT& checksum, FileT& file, size_t limit, char* buf, size_t bufSize)
{
	size_t getten;
	while (bufSize < limit)
	{
		const int err = file.get(buf, bufSize, getten);
		if (err == 0)
		{
			checksum.Update((const BYTE*)buf, getten);
			if (bufSize != getten)
				return 0;
			limit -= getten;
		}
		else
		{
			return err;
		}
	}
	const int err = file.get(buf, limit, getten);
	if (err == 0)
		checksum.Update((const BYTE*)buf, getten);
	return err;
}

// -------------------------------------------------------------------------

NS_STDEXT_END

#if defined(X_OS_WINDOWS)
#include "fileio/windows.h"
#else
#endif

#endif // STDEXT_FILEIO_H
