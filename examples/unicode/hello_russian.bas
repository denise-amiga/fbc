#ifdef __FB_WIN32__
# define unicode
# include once "windows.bi"
#endif

const LANG = "Russian"
	dim helloworld as wstring * 20 => "Привет, мир!"

	print """Hello World!"" in "; LANG; ": "; helloworld

#ifdef __FB_WIN32__
	messagebox( 0, helloworld, """Hello World!"" in " & LANG & ":", MB_OK )
#endif
