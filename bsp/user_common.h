#ifndef USERCOMMONH
#define USERCOMMONH
typedef unsigned char uchar;
typedef unsigned int uint;
typedef signed int USER_RETURN_VALUE;
typedef unsigned int uint32;
typedef signed long long int64;
typedef unsigned long long uint64;

/*-------------------------------------------*/
/* Integer type definitions for FatFs module */
/*-------------------------------------------*/
#ifdef _WIN32	/* FatFs development platform */

#include <windows.h>
#include <tchar.h>
typedef unsigned __int64 QWORD;


#else			/* Embedded platform */

/* These types MUST be 16-bit or 32-bit */
typedef int				INT;
typedef unsigned int	UINT;

/* This type MUST be 8-bit */
typedef unsigned char	BYTE;

/* These types MUST be 16-bit */
typedef short			SHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;

/* These types MUST be 32-bit */
typedef long			LONG;
typedef unsigned long	DWORD;

/* This type MUST be 64-bit (Remove this for C89 compatibility) */
typedef unsigned long long QWORD;

#endif

#endif