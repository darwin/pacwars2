/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## scripting interface for stdlib.h
//## AH
//###########################################################################
//                        ## SEER - C-Scripting engine - Przemyslaw Podsiadly

/***
*stdlib.h - declarations/definitions for commonly used library functions
*
*       Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This include file contains the function declarations for commonly
*       used library functions which either don't fit somewhere else, or,
*       cannot be declared in the normal place for other reasons.
*       [ANSI]
*
*       [Public]
*
****/

#ifndef _INC_STDLIB
#define _INC_STDLIB

#ifdef  __cplusplus
extern "C" {
#endif

//#define long int  // SeeR doesn't support long type, but in VC++6.0 int = long

#ifndef _SIZE_T_DEFINED
typedef unsigned int size_t;
#define _SIZE_T_DEFINED
#endif

#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif

/* Definition of the argument values for the exit() function */

#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1


#ifndef _ONEXIT_T_DEFINED
typedef int (* _onexit_t)();
#define onexit_t _onexit_t
#define _ONEXIT_T_DEFINED
#endif


/* Data structure definitions for div and ldiv runtimes. */

#ifndef _DIV_T_DEFINED

typedef struct _div_t {
        int quot;
        int rem;
} div_t;

//l typedef struct _ldiv_t {
//l         long quot;
//l         long rem;
//l} ldiv_t;

#define _DIV_T_DEFINED
#endif


/* Maximum value that can be returned by the rand function. */

#define RAND_MAX 0x7fff

/*
 * Maximum number of bytes in multi-byte character in the current locale
 * (also defined in ctype.h).
 */
#ifndef MB_CUR_MAX
#define MB_CUR_MAX __mb_cur_max
//import int __mb_cur_max;
#endif  /* MB_CUR_MAX */


/* Minimum and maximum macros */

#define __max(a,b)  (((a) > (b)) ? (a) : (b))
#define __min(a,b)  (((a) < (b)) ? (a) : (b))

/*
 * Sizes for buffers used by the _makepath() and _splitpath() functions.
 * note that the sizes include space for 0-terminator
 */
#define _MAX_PATH   260 /* max. length of full pathname */
#define _MAX_DRIVE  3   /* max. length of drive component */
#define _MAX_DIR    256 /* max. length of path component */
#define _MAX_FNAME  256 /* max. length of file name component */
#define _MAX_EXT    256 /* max. length of extension component */

/*
 * Argument values for _set_error_mode().
 */
#define _OUT_TO_DEFAULT 0
#define _OUT_TO_STDERR  1
#define _OUT_TO_MSGBOX  2
#define _REPORT_ERRMODE 3


/* External variable declarations */

import int * _errno();
//l import unsigned long * __doserrno();
#define errno       (*_errno())
#define _doserrno   (*__doserrno())


import char** _sys_errlist;     /* perror error message table */
import int _sys_nerr;           /* # of entries in sys_errlist table */


#define __argc      (*__p___argc())     /* count of cmd line args */
#define __argv      (*__p___argv())     /* pointer to table of cmd line args */
#define __wargv     (*__p___wargv())    /* pointer to table of wide cmd line args */
#define _environ    (*__p__environ())   /* pointer to environment table */

import char ** environ;                 /* pointer to environment table */
#define _wenviron   (*__p__wenviron())  /* pointer to wide environment table */

import int *          __p___argc();
import char ***       __p___argv();
import wchar_t ***    __p___wargv();
import char ***       __p__environ();
import wchar_t ***    __p__wenviron();
import char **        __p__pgmptr();
import wchar_t **     __p__wpgmptr();

import int _fmode;          /* default file translation mode */
import int _fileinfo;       /* open file info mode (for spawn) */


/* Windows major/minor and O.S. version numbers */

//import unsigned int _osver;
//import unsigned int _winver;
//import unsigned int _winmajor;
//import unsigned int _winminor;


/* function prototypes */

import void   abort();
import void   exit(int);

//import int    abs(int);
import double atof(const char *);
//! import int    atexit(void (*)());
import int    atoi(const char *);
//l import long   atol(const char *);
//#ifdef  _M_M68K
//l import long double _atold(const char *);
//#endif
//! import void * bsearch(const void *, const void *, size_t, size_t, int (*)(const void *, const void *));
import void * calloc(size_t, size_t);
import div_t  div(int, int);
import void   free(void *);
import char * getenv(const char *);
import char * _itoa(int, char *, int);
//64 import char * _i64toa(int, char *, int);
//64import char * _ui64toa(unsigned int, char *, int);
//64 import int _atoi64(const char *);
//l import long labs(long);
//l import ldiv_t ldiv(long, long);
//l import char * _ltoa(long, char *, int);
import void * malloc(size_t);
import int    mblen(const char *, size_t);
import size_t _mbstrlen(const char *s);
import int    mbtowc(wchar_t *, const char *, size_t);
import size_t mbstowcs(wchar_t *, const char *, size_t);
//! import void   qsort(void *, size_t, size_t, int (*) (const void *, const void *));
import int    rand();
import void * realloc(void *, size_t);
import int    _set_error_mode(int);
import void   srand(unsigned int);
import double strtod(const char *, char **);
//l import long   strtol(const char *, char **, int);
//#ifdef  _M_M68K
//l import long double _strtold(const char *, char **);
//#endif
//l import unsigned long strtoul(const char *, char **, int);
import int    system(const char *);
//l import char * _ultoa(unsigned long, char *, int);
import int    wctomb(char *, wchar_t);
import size_t wcstombs(char *, const wchar_t *, size_t);


/* wide function prototypes, also declared in wchar.h  */

import wchar_t * _itow (int, wchar_t *, int);
//l import wchar_t * _ltow (long, wchar_t *, int);
//l import wchar_t * _ultow (unsigned long, wchar_t *, int);
import double wcstod(const wchar_t *, wchar_t **);
//l import long   wcstol(const wchar_t *, wchar_t **, int);
//l import unsigned long wcstoul(const wchar_t *, wchar_t **, int);
import wchar_t * _wgetenv(const wchar_t *);
import int    _wsystem(const wchar_t *);
import int _wtoi(const wchar_t *);
//l import long _wtol(const wchar_t *);
//64 import wchar_t * _i64tow(int, wchar_t *, int);
//64 import wchar_t * _ui64tow(unsigned int, wchar_t *, int);
//64 import int   _wtoi64(const wchar_t *);

import char * _ecvt(double, int, int *, int *);
import void   _exit(int);
import char * _fcvt(double, int, int *, int *);
import char * _fullpath(char *, const char *, size_t);
import char * _gcvt(double, int, char *);
//l         unsigned long _lrotl(unsigned long, int);
//l         unsigned long _lrotr(unsigned long, int);
import void   _makepath(char *, const char *, const char *, const char *, const char *);

//! import void   perror(const char *);
import int    _putenv(const char *);
import unsigned int _rotl(unsigned int, int);
import unsigned int _rotr(unsigned int, int);
import void   _searchenv(const char *, const char *, char *);
import void   _splitpath(const char *, char *, char *, char *, char *);
import void   _swab(char *, char *, int);


/* wide function prototypes, also declared in wchar.h  */

import wchar_t * _wfullpath(wchar_t *, const wchar_t *, size_t);
import void   _wmakepath(wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *);
//! import void   _wperror(const wchar_t *);
import int    _wputenv(const wchar_t *);
import void   _wsearchenv(const wchar_t *, const wchar_t *, wchar_t *);
import void   _wsplitpath(const wchar_t *, wchar_t *, wchar_t *, wchar_t *, wchar_t *);

/* --------- The following functions are OBSOLETE --------- */
/* The Win32 API SetErrorMode, Beep and Sleep should be used instead. */
import void _seterrormode(int);
import void _beep(unsigned int, unsigned int);
//l import void _sleep(unsigned long);
/* --------- The preceding functions are OBSOLETE --------- */


/* --------- The declarations below should not be in stdlib.h --------- */
/* --------- and will be removed in a future release. Include --------- */
/* --------- ctype.h to obtain these declarations.            --------- */
//#ifndef tolower     /* tolower has been undefined - use function */
import int tolower(int);
//#endif  /* tolower */
//#ifndef toupper     /* toupper has been undefined - use function */
import int toupper(int);
//#endif  /* toupper */
/* --------- The declarations above will be removed.          --------- */


/* Non-ANSI names for compatibility */

#ifndef __cplusplus
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

#define sys_errlist _sys_errlist
#define sys_nerr    _sys_nerr
#define environ     _environ

import char * ecvt(double, int, int *, int *);
import char * fcvt(double, int, int *, int *);
import char * gcvt(double, int, char *);
import char * itoa(int, char *, int);
//l import char * ltoa(long, char *, int);
import onexit_t onexit(onexit_t);
import int    putenv(const char *);
import void   swab(char *, char *, int);
//l import char * ultoa(unsigned long, char *, int);

#ifdef  __cplusplus
}
#endif

#endif  /* _INC_STDLIB */
