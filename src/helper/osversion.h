/*
 *  osversion.h 
 *  osversion
 *
 *  Created by Jevstein on 2018/4/11 18:57.
 *  Copyright @ 2018year Jevstein. All rights reserved.
 *
 */
#ifndef OS_VERSION_H
#define OS_VERSION_H
  
#define YI_VERSION_STR   "3.1.2"  
/* 
   YI_VERSION is (major << 16) + (minor << 8) + patch. 
 */  
#define YI_VERSION 0x030102  
  
/* 
   The operating system, must be one of: (YI_OS_x) 
 
     MACX   - Mac OS X 
     MAC9   - Mac OS 9 
     MSDOS  - MS-DOS and Windows 
     OS2    - OS/2 
     OS2EMX - XFree86 on OS/2 (not PM) 
     WIN32  - Win32 (Windows 95/98/ME and Windows NT/2000/XP) 
     CYGWIN - Cygwin 
     SOLARIS    - Sun Solaris 
     HPUX   - HP-UX 
     ULTRIX - DEC Ultrix 
     LINUX  - Linux 
     FREEBSD    - FreeBSD 
     NETBSD - NetBSD 
     OPENBSD    - OpenBSD 
     BSDI   - BSD/OS 
     IRIX   - SGI Irix 
     OSF    - HP Tru64 UNIX 
     SCO    - SCO OpenServer 5 
     UNIXWARE   - UnixWare 7, Open UNIX 8 
     AIX    - AIX 
     HURD   - GNU Hurd 
     DGUX   - DG/UX 
     RELIANT    - Reliant UNIX 
     DYNIX  - DYNIX/ptx 
     QNX    - QNX 
     QNX6   - QNX RTP 6.1 
     LYNX   - LynxOS 
     BSD4   - Any BSD 4.4 system 
     UNIX   - Any UNIX BSD/SYSV system 
*/  
  
#if defined(__APPLE__) && defined(__GNUC__)  
#  define YI_OS_MACX  
#elif defined(__MACOSX__)  
#  define YI_OS_MACX  
#elif defined(macintosh)  
#  define YI_OS_MAC9  
#elif defined(__CYGWIN__)  
#  define YI_OS_CYGWIN  
#elif defined(MSDOS) || defined(_MSDOS)  
#  define YI_OS_MSDOS  
#elif defined(__OS2__)  
#  if defined(__EMX__)  
#    define YI_OS_OS2EMX  
#  else  
#    define YI_OS_OS2  
#  endif  
#elif !defined(SAG_COM) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))  
#  define YI_OS_WIN32  
#  define YI_OS_WIN64  
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))  
#  define YI_OS_WIN32  
#elif defined(__MWERKS__) && defined(__INTEL__)  
#  define YI_OS_WIN32  
#elif defined(__sun) || defined(sun)  
#  define YI_OS_SOLARIS  
#elif defined(hpux) || defined(__hpux)  
#  define YI_OS_HPUX  
#elif defined(__ultrix) || defined(ultrix)  
#  define YI_OS_ULTRIX  
#elif defined(sinix)  
#  define YI_OS_RELIANT  
#elif defined(__linux__) || defined(__linux)  
#  define YI_OS_LINUX  
#elif defined(__FreeBSD__)  
#  define YI_OS_FREEBSD  
#  define YI_OS_BSD4  
#elif defined(__NetBSD__)  
#  define YI_OS_NETBSD  
#  define YI_OS_BSD4  
#elif defined(__OpenBSD__)  
#  define YI_OS_OPENBSD  
#  define YI_OS_BSD4  
#elif defined(__bsdi__)  
#  define YI_OS_BSDI  
#  define YI_OS_BSD4  
#elif defined(__sgi)  
#  define YI_OS_IRIX  
#elif defined(__osf__)  
#  define YI_OS_OSF  
#elif defined(_AIX)  
#  define YI_OS_AIX  
#elif defined(__Lynx__)  
#  define YI_OS_LYNX  
#elif defined(__GNU_HURD__)  
#  define YI_OS_HURD  
#elif defined(__DGUX__)  
#  define YI_OS_DGUX  
#elif defined(__QNXNTO__)  
#  define YI_OS_QNX6  
#elif defined(__QNX__)  
#  define YI_OS_QNX  
#elif defined(_SEQUENT_)  
#  define YI_OS_DYNIX  
#elif defined(_SCO_DS)                   /* SCO OpenServer 5 + GCC */  
#  define YI_OS_SCO  
#elif defined(__USLC__)                  /* all SCO platforms + UDK or OUDK */  
#  define YI_OS_UNIXWARE  
#  define YI_OS_UNIXWARE7  
#elif defined(__svr4__) && defined(i386) /* Open UNIX 8 + GCC */  
#  define YI_OS_UNIXWARE  
#  define YI_OS_UNIXWARE7  
#else  
#  error "Qt has not been ported to this OS - talk to qt-bugs@trolltech.com"  
#endif  
  
#if defined(YI_OS_MAC9) || defined(YI_OS_MACX)  
#  define YI_OS_MAC  
#endif  
  
#if defined(YI_OS_MAC9) || defined(YI_OS_MSDOS) || defined(YI_OS_OS2) || defined(YI_OS_WIN32) || defined(YI_OS_WIN64)  
#  undef YI_OS_UNIX  
#elif !defined(YI_OS_UNIX)  
#  define YI_OS_UNIX  
#endif

#endif//OS_VERSION_H  