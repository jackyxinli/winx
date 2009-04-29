
activeobject2ps.dll: dlldata.obj activeobject2_p.obj activeobject2_i.obj
	link /dll /out:activeobject2ps.dll /def:activeobject2ps.def /entry:DllMain dlldata.obj activeobject2_p.obj activeobject2_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del activeobject2ps.dll
	@del activeobject2ps.lib
	@del activeobject2ps.exp
	@del dlldata.obj
	@del activeobject2_p.obj
	@del activeobject2_i.obj
