/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## scripting interface for string.h functions
//## AH
//###########################################################################

#ifndef __SCSTRING_H__
#define __SCSTRING_H__

#ifndef _SIZE_T_DEFINED
typedef unsigned int size_t;
#define _SIZE_T_DEFINED
#endif

#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif

#ifndef _NLSCMP_DEFINED
#define _NLSCMPERROR    2147483647
#define _NLSCMP_DEFINED
#endif

// functions

import void *  memcpy(void *, const void *, size_t);
import int     memcmp(const void *, const void *, size_t);
import void *  memset(void *, int, size_t);
import char *  _strset(char *, int);
import char *  strcpy(char *, const char *);
import char *  strcat(char *, const char *);
import int     strcmp(const char *, const char *);
import size_t  strlen(const char *);
import void *  _memccpy(void *, const void *, int, unsigned int);
import void *  memchr(const void *, int, size_t);
import int     _memicmp(const void *, const void *, unsigned int);

import void *  memmove(void *, const void *, size_t);

import char *  strchr(const char *, int);
import int     _strcmpi(const char *, const char *);
import int     _stricmp(const char *, const char *);
import int     strcoll(const char *, const char *);
import int     _stricoll(const char *, const char *);
import int     _strncoll(const char *, const char *, size_t);
import int     _strnicoll(const char *, const char *, size_t);
import size_t  strcspn(const char *, const char *);
import char *  _strdup(const char *);
import char *  _strerror(const char *);
import char *  strerror(int);
import char *  _strlwr(char *);
import char *  strncat(char *, const char *, size_t);
import int     strncmp(const char *, const char *, size_t);
import int     _strnicmp(const char *, const char *, size_t);
import char *  strncpy(char *, const char *, size_t);
import char *  _strnset(char *, int, size_t);
import char *  strpbrk(const char *, const char *);
import char *  strrchr(const char *, int);
import char *  _strrev(char *);
import size_t  strspn(const char *, const char *);
import char *  strstr(const char *, const char *);
import char *  strtok(char *, const char *);
import char *  _strupr(char *);
import size_t  strxfrm (char *, const char *, size_t);

import void * memccpy(void *, const void *, int, unsigned int);
import int memicmp(const void *, const void *, unsigned int);
import int strcmpi(const char *, const char *);
import int stricmp(const char *, const char *);
import char * strdup(const char *);
import char * strlwr(char *);
import int strnicmp(const char *, const char *, size_t);
import char * strnset(char *, int, size_t);
import char * strrev(char *);
import char * strset(char *, int);
import char * strupr(char *);

#ifndef _WSTRING_DEFINED

import wchar_t * wcscat(wchar_t *, const wchar_t *);
import wchar_t * wcschr(const wchar_t *, wchar_t);
import int wcscmp(const wchar_t *, const wchar_t *);
import wchar_t * wcscpy(wchar_t *, const wchar_t *);
import size_t wcscspn(const wchar_t *, const wchar_t *);
import size_t wcslen(const wchar_t *);
import wchar_t * wcsncat(wchar_t *, const wchar_t *, size_t);
import int wcsncmp(const wchar_t *, const wchar_t *, size_t);
import wchar_t * wcsncpy(wchar_t *, const wchar_t *, size_t);
import wchar_t * wcspbrk(const wchar_t *, const wchar_t *);
import wchar_t * wcsrchr(const wchar_t *, wchar_t);
import size_t wcsspn(const wchar_t *, const wchar_t *);
import wchar_t * wcsstr(const wchar_t *, const wchar_t *);
import wchar_t * wcstok(wchar_t *, const wchar_t *);

import wchar_t * _wcsdup(const wchar_t *);
import int _wcsicmp(const wchar_t *, const wchar_t *);
import int _wcsnicmp(const wchar_t *, const wchar_t *, size_t);
import wchar_t * _wcsnset(wchar_t *, wchar_t, size_t);
import wchar_t * _wcsrev(wchar_t *);
import wchar_t * _wcsset(wchar_t *, wchar_t);

import wchar_t * _wcslwr(wchar_t *);
import wchar_t * _wcsupr(wchar_t *);
import size_t wcsxfrm(wchar_t *, const wchar_t *, size_t);
import int wcscoll(const wchar_t *, const wchar_t *);
import int _wcsicoll(const wchar_t *, const wchar_t *);
import int _wcsncoll(const wchar_t *, const wchar_t *, size_t);
import int _wcsnicoll(const wchar_t *, const wchar_t *, size_t);

#define wcswcs wcsstr

import wchar_t * wcsdup(const wchar_t *);
import int wcsicmp(const wchar_t *, const wchar_t *);
import int wcsnicmp(const wchar_t *, const wchar_t *, size_t);
import wchar_t * wcsnset(wchar_t *, wchar_t, size_t);
import wchar_t * wcsrev(wchar_t *);
import wchar_t * wcsset(wchar_t *, wchar_t);
import wchar_t * wcslwr(wchar_t *);
import wchar_t * wcsupr(wchar_t *);
import int wcsicoll(const wchar_t *, const wchar_t *);

#define _WSTRING_DEFINED
#endif

#endif  // __SCSTRING_H__
