/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## scripting interface for stdlib.h
//## AH
//###########################################################################

#ifndef __SCSTDLIB_H__
#define __SCSTDLIB_H__

//#define long int  // SeeR doesn't support long type, but in VC++6.0 int = long

#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1

#define __max(a,b)  (((a) > (b)) ? (a) : (b))
#define __min(a,b)  (((a) < (b)) ? (a) : (b))

#define _MAX_PATH   260 
#define _MAX_DRIVE  3   
#define _MAX_DIR    256 
#define _MAX_FNAME  256 
#define _MAX_EXT    256 

#define _OUT_TO_DEFAULT 0
#define _OUT_TO_STDERR  1
#define _OUT_TO_MSGBOX  2
#define _REPORT_ERRMODE 3

#define RAND_MAX 0x7fff

#ifndef _SIZE_T_DEFINED
typedef unsigned int size_t;
#define _SIZE_T_DEFINED
#endif

#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif

#ifndef _ONEXIT_T_DEFINED
typedef int (* _onexit_t)();
#define onexit_t _onexit_t
#define _ONEXIT_T_DEFINED
#endif

#ifndef _DIV_T_DEFINED
typedef struct _div_t {
  int quot;
  int rem;
} div_t;
#define _DIV_T_DEFINED
#endif

import int * _errno();
#define errno       (*_errno())
#define _doserrno   (*__doserrno())

import char** _sys_errlist;     
import int _sys_nerr;

#define __argc      (*__p___argc())     
#define __argv      (*__p___argv())     
#define __wargv     (*__p___wargv())    
#define _environ    (*__p__environ())   

import char ** environ;                 
#define _wenviron   (*__p__wenviron())  

import int *          __p___argc();
import char ***       __p___argv();
import wchar_t ***    __p___wargv();
import char ***       __p__environ();
import wchar_t ***    __p__wenviron();
import char **        __p__pgmptr();
import wchar_t **     __p__wpgmptr();

import int _fmode;          
import int _fileinfo;       

// functions

import void   abort();
import void   exit(int);

import double atof(const char *);
import int    atoi(const char *);
import void * calloc(size_t, size_t);
import div_t  div(int, int);
import void   free(void *);
import char * getenv(const char *);
import char * _itoa(int, char *, int);
import void * malloc(size_t);
import int    mblen(const char *, size_t);
import size_t _mbstrlen(const char *s);
import int    mbtowc(wchar_t *, const char *, size_t);
import size_t mbstowcs(wchar_t *, const char *, size_t);
import int    rand();
import void * realloc(void *, size_t);
import int    _set_error_mode(int);
import void   srand(unsigned int);
import double strtod(const char *, char **);
import int    system(const char *);
import int    wctomb(char *, wchar_t);
import size_t wcstombs(char *, const wchar_t *, size_t);

import wchar_t * _itow (int, wchar_t *, int);
import double wcstod(const wchar_t *, wchar_t **);
import wchar_t * _wgetenv(const wchar_t *);
import int    _wsystem(const wchar_t *);
import int _wtoi(const wchar_t *);
import char * _ecvt(double, int, int *, int *);
import void   _exit(int);
import char * _fcvt(double, int, int *, int *);
import char * _fullpath(char *, const char *, size_t);
import char * _gcvt(double, int, char *);
import void   _makepath(char *, const char *, const char *, const char *, const char *);
import int    _putenv(const char *);
import unsigned int _rotl(unsigned int, int);
import unsigned int _rotr(unsigned int, int);
import void   _searchenv(const char *, const char *, char *);
import void   _splitpath(const char *, char *, char *, char *, char *);
import void   _swab(char *, char *, int);
import wchar_t * _wfullpath(wchar_t *, const wchar_t *, size_t);
import void   _wmakepath(wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *);
import int    _wputenv(const wchar_t *);
import void   _wsearchenv(const wchar_t *, const wchar_t *, wchar_t *);
import void   _wsplitpath(const wchar_t *, wchar_t *, wchar_t *, wchar_t *, wchar_t *);

import void _seterrormode(int);
import void _beep(unsigned int, unsigned int);

import int tolower(int);
import int toupper(int);

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

#define sys_errlist _sys_errlist
#define sys_nerr    _sys_nerr
#define environ     _environ

import char * ecvt(double, int, int *, int *);
import char * fcvt(double, int, int *, int *);
import char * gcvt(double, int, char *);
import char * itoa(int, char *, int);
import onexit_t onexit(onexit_t);
import int    putenv(const char *);
import void   swab(char *, char *, int);

#endif  // __SCSTDLIB_H__
