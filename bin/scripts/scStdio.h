/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## scripting interface for stdio.h functions
//## AH
//###########################################################################

#ifndef __SCSTDIO_H__
#define __SCSTDIO_H__

#define _IOREAD         0x0001
#define _IOWRT          0x0002
#define _IOFBF          0x0000
#define _IOLBF          0x0040
#define _IONBF          0x0004
#define _IOMYBUF        0x0008
#define _IOEOF          0x0010
#define _IOERR          0x0020
#define _IOSTRG         0x0040
#define _IORW           0x0080
#define _IOAPPEND       0x0200

#ifndef _SIZE_T_DEFINED
typedef unsigned int size_t;
#define _SIZE_T_DEFINED
#endif

#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif

#ifndef _WCTYPE_T_DEFINED
typedef wchar_t wint_t;
typedef wchar_t wctype_t;
#define _WCTYPE_T_DEFINED
#endif

#ifndef _VA_LIST_DEFINED
typedef char *  va_list;
#define _VA_LIST_DEFINED
#endif

#define BUFSIZ        512
#define _NFILE        _NSTREAM_
#define _NSTREAM_     512
#define _IOB_ENTRIES  20
#define EOF           (-1)

#ifndef _FILE_DEFINED
struct _iobuf {
  char *_ptr;
  int   _cnt;
  char *_base;
  int   _flag;
  int   _file;
  int   _charbuf;
  int   _bufsiz;
  char *_tmpfname;
};
typedef _iobuf FILE;
#define _FILE_DEFINED
#endif

#define _P_tmpdir   "/"
#define _wP_tmpdir  L"/"
#define L_tmpnam sizeof(_P_tmpdir)+12

#define L_ctermid   9
#define L_cuserid   32

#define SEEK_CUR    1
#define SEEK_END    2
#define SEEK_SET    0

#define FILENAME_MAX    260
#define FOPEN_MAX       20
#define _SYS_OPEN       20
#define TMP_MAX         32767

#ifndef _STDIO_DEFINED
import FILE* _iob;
#endif  

typedef struct fpos_t {
  unsigned int lopart;
  int          hipart;
} fpos_t;

#define stdin  (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

// functions

#ifndef _STDIO_DEFINED

import int _filbuf(FILE *);
import int _flsbuf(int, FILE *);

import FILE * _fsopen(const char *, const char *, int);

import void clearerr(FILE *);
import int fclose(FILE *);
import int _fcloseall(void);

import FILE * _fdopen(int, const char *);

import int feof(FILE *);
import int ferror(FILE *);
import int fflush(FILE *);
import int fgetc(FILE *);
import int _fgetchar(void);
import int fgetpos(FILE *, fpos_t *);
import char * fgets(char *, int, FILE *);

import int _fileno(FILE *);

import int _flushall(void);
import FILE * fopen(const char *, const char *);
import int fprintf(FILE *, const char *, ...);
import int fputc(int, FILE *);
import int _fputchar(int);
import int fputs(const char *, FILE *);
import size_t fread(void *, size_t, size_t, FILE *);
import FILE * freopen(const char *, const char *, FILE *);
import int fscanf(FILE *, const char *, ...);
import int fsetpos(FILE *, const fpos_t *);
import int fseek(FILE *, unsigned int, unsigned int); //!
import unsigned int ftell(FILE *); //!
import size_t fwrite(const void *, size_t, size_t, FILE *);
import int getc(FILE *);
import int getchar(void);
import int _getmaxstdio(void);
import char * gets(char *);
import int _getw(FILE *);
import void perror(const char *);
import int _pclose(FILE *);
import FILE * _popen(const char *, const char *);
import int printf(const char *, ...);
import int putc(int, FILE *);
import int putchar(int);
import int puts(const char *);
import int _putw(int, FILE *);
import int remove(const char *);
import int rename(const char *, const char *);
import void rewind(FILE *);
import int _rmtmp(void);
import int scanf(const char *, ...);
import void setbuf(FILE *, char *);
import int _setmaxstdio(int);
import int setvbuf(FILE *, char *, int, size_t);
import int _snprintf(char *, size_t, const char *, ...);
import int sprintf(char *, const char *, ...);
import int sscanf(const char *, const char *, ...);
import char * _tempnam(const char *, const char *);
import FILE * tmpfile(void);
import char * tmpnam(char *);
import int ungetc(int, FILE *);
import int _unlink(const char *);
import int vfprintf(FILE *, const char *, va_list);
import int vprintf(const char *, va_list);
import int _vsnprintf(char *, size_t, const char *, va_list);
import int vsprintf(char *, const char *, va_list);

#ifndef WEOF
#define WEOF (wint_t)(0xFFFF)
#endif

import FILE * _wfsopen(const wchar_t *, const wchar_t *, int);

import wint_t fgetwc(FILE *);
import wint_t _fgetwchar(void);
import wint_t fputwc(wint_t, FILE *);
import wint_t _fputwchar(wint_t);
import wint_t getwc(FILE *);
import wint_t getwchar(void);
import wint_t putwc(wint_t, FILE *);
import wint_t putwchar(wint_t);
import wint_t ungetwc(wint_t, FILE *);

import wchar_t * fgetws(wchar_t *, int, FILE *);
import int fputws(const wchar_t *, FILE *);
import wchar_t * _getws(wchar_t *);
import int _putws(const wchar_t *);

import int fwprintf(FILE *, const wchar_t *, ...);
import int wprintf(const wchar_t *, ...);
import int _snwprintf(wchar_t *, size_t, const wchar_t *, ...);
import int swprintf(wchar_t *, const wchar_t *, ...);
import int vfwprintf(FILE *, const wchar_t *, va_list);
import int vwprintf(const wchar_t *, va_list);
import int _vsnwprintf(wchar_t *, size_t, const wchar_t *, va_list);
import int vswprintf(wchar_t *, const wchar_t *, va_list);
import int fwscanf(FILE *, const wchar_t *, ...);
import int swscanf(const wchar_t *, const wchar_t *, ...);
import int wscanf(const wchar_t *, ...);

#define getwchar()              fgetwc(stdin)
#define putwchar(_c)            fputwc((_c),stdout)
#define getwc(_stm)             fgetwc(_stm)
#define putwc(_c,_stm)          fputwc(_c,_stm)

import FILE * _wfdopen(int, const wchar_t *);
import FILE * _wfopen(const wchar_t *, const wchar_t *);
import FILE * _wfreopen(const wchar_t *, const wchar_t *, FILE *);
import void _wperror(const wchar_t *);
import FILE * _wpopen(const wchar_t *, const wchar_t *);
import int _wremove(const wchar_t *);
import wchar_t * _wtempnam(const wchar_t *, const wchar_t *);
import wchar_t * _wtmpnam(wchar_t *);

// macros

#define feof(_stream)     ((_stream)->_flag & _IOEOF)
#define ferror(_stream)   ((_stream)->_flag & _IOERR)
#define _fileno(_stream)  ((_stream)->_file)
#define getc(_stream)     (--(_stream)->_cnt >= 0 \
                ? 0xff & *(_stream)->_ptr++ : _filbuf(_stream))
#define putc(_c,_stream)  (--(_stream)->_cnt >= 0 \
                ? 0xff & (*(_stream)->_ptr++ = (char)(_c)) :  _flsbuf((_c),(_stream)))
#define getchar()         getc(stdin)
#define putchar(_c)       putc((_c),stdout)


#define P_tmpdir  _P_tmpdir
#define SYS_OPEN  _SYS_OPEN

import int fcloseall(void);
import FILE * fdopen(int, const char *);
import int fgetchar(void);
import int fileno(FILE *);
import int flushall(void);
import int fputchar(int);
import int getw(FILE *);
import int putw(int, FILE *);
import int rmtmp(void);
import char * tempnam(const char *, const char *);
import int unlink(const char *);

#endif  // _STDIO_DEFINED

#endif  // __SCSTDIO_H__


