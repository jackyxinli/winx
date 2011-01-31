#ifndef STDEXT_FILEIO_WINDOWS_H
#define STDEXT_FILEIO_WINDOWS_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class WinFile

class WinFile
{
public:
	typedef HANDLE FD;
	typedef int Mode;
	typedef int Attrs;
	typedef int Errno;
	typedef INT64 FileOffset;
	typedef FileOffset FileSize;
	typedef INT64 FileTime;
	typedef WinFile Self;

	typedef char char_type;
	typedef size_t size_type;

private:
	FD m_fd;

private:
	static Errno winx_call getErrno()
	{
		return (int)GetLastError();
	}

public:
	enum Flag
	{
		FLAG_RSHRBITS_ = 8,
		FLAG_MASKS_ = 0x00FF0000L,

		F_WRITE_THROUGH = FILE_FLAG_WRITE_THROUGH >> FLAG_RSHRBITS_,	 // 0x80000000
		F_NO_BUFFERING = FILE_FLAG_NO_BUFFERING >> FLAG_RSHRBITS_,		 // 0x20000000
		F_RANDOM_ACCESS = FILE_FLAG_RANDOM_ACCESS >> FLAG_RSHRBITS_,	 // 0x10000000
		F_SEQUENTIAL_SCAN = FILE_FLAG_SEQUENTIAL_SCAN >> FLAG_RSHRBITS_, // 0x08000000
		F_DELETE_ON_CLOSE = FILE_FLAG_DELETE_ON_CLOSE >> FLAG_RSHRBITS_, // 0x04000000
	};

	enum OpenMode
	{
		OPEN_MASKS_ = 0x0000000FL,

		O_CREATE_NEW = CREATE_NEW,				// 1: if exists, fail; if not, create
		O_CREATE_ALWAYS = CREATE_ALWAYS,		// 2: if exists, truncate; if not, create
		O_OPEN_EXISTING = OPEN_EXISTING,		// 3: if exists, open; if not, fail
		O_OPEN_ALWAYS = OPEN_ALWAYS,			// 4: if exists, open, if not, create
		O_TRUNC_EXISTING = TRUNCATE_EXISTING,	// 5: if exists, truncate; if not, fail
	};

	enum AccessMode
	{
		ACCESS_RSHRBITS_ = 16,
		ACCESS_MASKS_ = 0x0000F000L,

		O_READ = GENERIC_READ >> ACCESS_RSHRBITS_,		// 0x80000000L
		O_WRITE = GENERIC_WRITE >> ACCESS_RSHRBITS_,	// 0x40000000L
		O_EXEC = GENERIC_EXECUTE >> ACCESS_RSHRBITS_,	// 0x20000000L
		O_ACCESS_ALL = GENERIC_ALL >> ACCESS_RSHRBITS_,	// 0x10000000L
	};

	enum ShareMode
	{
		SHARE_LHRBITS_ = 8,
		SHARE_MASKS_ = 0x00000F00L,

		S_READ = FILE_SHARE_READ << SHARE_LHRBITS_,		// 0x00000001
		S_WRITE = FILE_SHARE_WRITE << SHARE_LHRBITS_,	// 0x00000002
		S_DELETE = FILE_SHARE_DELETE << SHARE_LHRBITS_, // 0x00000004
	};

	enum Attribute
	{
		A_NORMAL = FILE_ATTRIBUTE_NORMAL, // The file does not have other attributes set. This attribute is valid only if used alone.
		A_READONLY = FILE_ATTRIBUTE_READONLY,
		A_HIDDEN = FILE_ATTRIBUTE_HIDDEN,
		A_SYSTEM = FILE_ATTRIBUTE_SYSTEM,
		A_TEMPORARY = FILE_ATTRIBUTE_TEMPORARY,
		A_ARCHIVE = FILE_ATTRIBUTE_ARCHIVE, // mark files for backup or removal
		A_ENCRYPTED = FILE_ATTRIBUTE_ENCRYPTED,
		A_OFFLINE = FILE_ATTRIBUTE_OFFLINE,
	};

	enum SeekMode
	{
		BEGIN = FILE_BEGIN,
		CURRENT = FILE_CURRENT,
		END = FILE_END,
	};

	enum ModeShortcut
	{
		READ = O_OPEN_EXISTING | O_READ,
		WRITE = O_CREATE_ALWAYS | O_WRITE,
		READWRITE = O_OPEN_ALWAYS | O_READ | O_WRITE,

		READ_THROUGH = F_SEQUENTIAL_SCAN | READ,
		WRITE_THROUGH = F_WRITE_THROUGH | WRITE,
	};

public:
	FD winx_call getHandle() const
	{
		return m_fd;
	}

	bool winx_call good() const
	{
		return m_fd != INVALID_HANDLE_VALUE;
	}

public:
	WinFile()
		: m_fd(INVALID_HANDLE_VALUE)
	{
	}

	Errno winx_call open(const char* file, Mode mode, Attrs attrs = A_NORMAL)
	{
		WINX_ASSERT(m_fd == INVALID_HANDLE_VALUE);

		const DWORD dwDesiredAccess = (DWORD)(ACCESS_MASKS_ & mode) << ACCESS_RSHRBITS_;
		const DWORD dwShareMode = (DWORD)(SHARE_MASKS_ & mode) >> SHARE_LHRBITS_;
		const DWORD dwCreationDisposition = (DWORD)(OPEN_MASKS_ & mode);
		const DWORD dwFlagsAndAttributes = 
			(DWORD)(((FLAG_MASKS_ & mode) << FLAG_RSHRBITS_) | attrs);

		m_fd = CreateFileA(
			file, dwDesiredAccess, dwShareMode, NULL,
			dwCreationDisposition, dwFlagsAndAttributes, NULL);
		
		if (m_fd != INVALID_HANDLE_VALUE)
			return 0;
		return getErrno();
	}

	Errno winx_call open(const wchar_t* file, Mode mode, Attrs attrs = A_NORMAL)
	{
		WINX_ASSERT(m_fd == INVALID_HANDLE_VALUE);

		const DWORD dwDesiredAccess = (DWORD)(ACCESS_MASKS_ & mode) << ACCESS_RSHRBITS_;
		const DWORD dwShareMode = (DWORD)(SHARE_MASKS_ & mode) >> SHARE_LHRBITS_;
		const DWORD dwCreationDisposition = (DWORD)(OPEN_MASKS_ & mode);
		const DWORD dwFlagsAndAttributes = 
			(DWORD)(((FLAG_MASKS_ & mode) << FLAG_RSHRBITS_) | attrs);

		m_fd = CreateFileW(
			file, dwDesiredAccess, dwShareMode, NULL,
			dwCreationDisposition, dwFlagsAndAttributes, NULL);

		if (m_fd != INVALID_HANDLE_VALUE)
			return 0;
		return getErrno();
	}

	Errno winx_call put(char ch)
	{
		DWORD bytes;
		if (WriteFile(m_fd, &ch, 1, &bytes, NULL))
			return 0;
		return getErrno();		
	}

	Errno winx_call put(size_t count, char ch)
	{
		DWORD bytes;
		while (count--)
		{
			if (!WriteFile(m_fd, &ch, 1, &bytes, NULL))
				return getErrno();
		}
		return 0;
	}

	Errno winx_call put(const char* s)
	{
		return put(s, strlen(s));
	}

	Errno winx_call put(const char* buf, size_t bytes)
	{
		if (WriteFile(m_fd, buf, (DWORD)bytes, (DWORD*)&bytes, NULL))
			return 0;
		return getErrno();
	}

	Errno winx_call put(const char* buf, size_t bytes, size_t& putten)
	{
		if (sizeof(DWORD) == sizeof(size_t))
		{
			if (WriteFile(m_fd, buf, (DWORD)bytes, (DWORD*)&putten, NULL))
				return 0;
		}
		else
		{
			DWORD tmp;
			if (WriteFile(m_fd, buf, (DWORD)bytes, &tmp, NULL)) {
				putten = tmp;
				return 0;
			}
		}
		return getErrno();
	}

	Errno winx_call get(char* buf, size_t bytes)
	{
		if (ReadFile(m_fd, buf, (DWORD)bytes, (DWORD*)&bytes, NULL))
			return 0;
		return getErrno();
	}

	Errno winx_call get(char* buf, size_t bytes, size_t& getten)
	{
		if (sizeof(DWORD) == sizeof(size_t))
		{
			if (ReadFile(m_fd, buf, (DWORD)bytes, (DWORD*)&getten, NULL))
				return 0;
		}
		else
		{
			DWORD tmp;
			if (ReadFile(m_fd, buf, (DWORD)bytes, (DWORD*)&tmp, NULL)) {
				getten = tmp;
				return 0;
			}
		}
		return getErrno();
	}

	Errno winx_call close()
	{
		if (m_fd == INVALID_HANDLE_VALUE)
			return 0;

		const Errno e = (CloseHandle(m_fd) ? 0 : getErrno());
		m_fd = INVALID_HANDLE_VALUE;
		return e;
	}

	void winx_call release()
	{
		close();
	}

	Errno winx_call seek(
		const FileOffset& dist, SeekMode mode = BEGIN, FileOffset* pos = NULL)
	{
		if (SetFilePointerEx(m_fd, *(PLARGE_INTEGER)&dist, (PLARGE_INTEGER)pos, mode))
			return 0;
		return getErrno();
	}

	Errno winx_call tell(FileOffset& pos)
	{
		const FileOffset zero = 0;
		if (SetFilePointerEx(m_fd, *(PLARGE_INTEGER)&zero, (PLARGE_INTEGER)&pos, CURRENT))
			return 0;
		return getErrno();
	}

	Errno winx_call getFileSize(FileSize& fileSize)
	{
		if (GetFileSizeEx(m_fd, (PLARGE_INTEGER)&fileSize))
			return 0;
		return getErrno();
	}

	Errno winx_call getEditTime(FileTime& editTime)
	{
		if (GetFileTime(m_fd, NULL, NULL, (LPFILETIME)&editTime))
			return 0;
		return getErrno();
	}

	Errno winx_call setEditTime(FileTime editTime)
	{
		if (SetFileTime(m_fd, NULL, NULL, (LPFILETIME)&editTime))
			return 0;
		return getErrno();
	}

	Errno winx_call lock(FileOffset offset, FileSize bytes)
	{
		if (LockFile(m_fd, (DWORD)offset, (DWORD)(offset >> 32), (DWORD)bytes, (DWORD)(bytes >> 32)))
			return 0;
		return getErrno();
	}

	Errno winx_call unlock(FileOffset offset, FileSize bytes)
	{
		if (UnlockFile(m_fd, (DWORD)offset, (DWORD)(offset >> 32), (DWORD)bytes, (DWORD)(bytes >> 32)))
			return 0;
		return getErrno();
	}

	Errno winx_call dup(Self& newFile)
	{
		const HANDLE p = GetCurrentProcess();
		if (DuplicateHandle(p, m_fd, p, &newFile.m_fd, 0, TRUE, DUPLICATE_SAME_ACCESS))
			return 0;
		return getErrno();
	}
};

typedef WinFile File;
typedef Resource<File> FilePtr;

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif // STDEXT_FILEIO_WINDOWS_H
