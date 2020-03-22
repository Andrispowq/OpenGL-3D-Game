#pragma once

#if defined(_WIN32)
	#if defined(_WIN64)
		#define PR_WINDOWS_64
	#else
		#error "Only 64 bit Windows is supported!"
	#endif
#elif defined(unix) || defined(_unix) || defined(_unix_)
	#error "Linux is not supported!"
#else
	#error "This OS is currently not supported!"
#endif