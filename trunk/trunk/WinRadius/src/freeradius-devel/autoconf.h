/* src/include/autoconf.h.  Generated from autoconf.h.in by configure.  */
/* src/include/autoconf.h.in.  Generated from configure.in by autoheader.  */

/* Include support for Ascend binary filter attributes */
#define ASCEND_BINARY 

/* BSD-Style get*byaddr_r */
#define BSDSTYLE 3

/* style of ctime_r function */
#define CTIMERSTYLE POSIXSTYLE

/* style of gethostbyaddr_r functions */
#define GETHOSTBYADDRRSTYLE GNUSTYLE

/* style of gethostbyname_r functions */
#define GETHOSTBYNAMERSTYLE GNUSTYLE

/* GNU-Style get*byaddr_r */
#define GNUSTYLE 1

/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 1

/* Define to 1 if you have the `closefrom' function. */
/* #undef HAVE_CLOSEFROM */

/* Do we have the crypt function */
#define HAVE_CRYPT 

/* Define to 1 if you have the <crypt.h> header file. */
#define HAVE_CRYPT_H 1

/* Define to 1 if you have the `ctime_r' function. */
#define HAVE_CTIME_R 1

/* Define to 1 if you have the declaration of `gethostbyaddr_r', and to 0 if
   you don't. */
/* #undef HAVE_DECL_GETHOSTBYADDR_R */

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#define HAVE_DIRENT_H 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the <fnmatch.h> header file. */
#define HAVE_FNMATCH_H 1

/* Define to 1 if you have the `getaddrinfo' function. */
#define HAVE_GETADDRINFO 1

/* Define to 1 if you have the `getnameinfo' function. */
#define HAVE_GETNAMEINFO 1

/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 1

/* Define to 1 if you have the `getopt_long' function. */
#define HAVE_GETOPT_LONG 1

/* Define to 1 if you have the `getpeereid' function. */
/* #undef HAVE_GETPEEREID */

/* Define to 1 if you have the `getresuid' function. */
#define HAVE_GETRESUID 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the `getusershell' function. */
#define HAVE_GETUSERSHELL 1

/* Define to 1 if you have the <glob.h> header file. */
#define HAVE_GLOB_H 1

/* Define to 1 if you have the `gmtime_r' function. */
#define HAVE_GMTIME_R 1

/* Define to 1 if you have the <grp.h> header file. */
#define HAVE_GRP_H 1

/* Define to 1 if you have the <history.h> header file. */
/* #undef HAVE_HISTORY_H */

/* define if you have IN6_PKTINFO (Linux) */
#define HAVE_IN6_PKTINFO 

/* Define to 1 if you have the `inet_aton' function. */
#define HAVE_INET_ATON 1

/* Define to 1 if you have the `inet_ntop' function. */
#define HAVE_INET_NTOP 1

/* Define to 1 if you have the `inet_pton' function. */
#define HAVE_INET_PTON 1

/* Define to 1 if you have the `initgroups' function. */
#define HAVE_INITGROUPS 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* define if you have IP_PKTINFO (Linux) */
#define HAVE_IP_PKTINFO 

/* Define to 1 if you have the `crypto' library (-lcrypto). */
#define HAVE_LIBCRYPTO 1

/* Define to 1 if you have the `nsl' library (-lnsl). */
#define HAVE_LIBNSL 1

/* Define to 1 if you have the `pcap' library (-lpcap). */
/* #undef HAVE_LIBPCAP */

/* Define if you have a readline compatible library */
#define HAVE_LIBREADLINE 1

/* Define to 1 if you have the `resolv' library (-lresolv). */
#define HAVE_LIBRESOLV 1

/* Define to 1 if you have the `socket' library (-lsocket). */
/* #undef HAVE_LIBSOCKET */

/* Define to 1 if you have the `ssl' library (-lssl). */
#define HAVE_LIBSSL 1

/* Define to 1 if you have the `ws2_32' library (-lws2_32). */
/* #undef HAVE_LIBWS2_32 */

/* Define to 1 if you have the `localtime_r' function. */
#define HAVE_LOCALTIME_R 1

/* Define to 1 if you have the `lockf' function. */
#define HAVE_LOCKF 1

/* Do we have the lt_dladvise_init function */
/* #undef HAVE_LT_DLADVISE_INIT */

/* Define to 1 if you have the <malloc.h> header file. */
#define HAVE_MALLOC_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
/* #undef HAVE_NDIR_H */

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define to 1 if you have the <net/if.h> header file. */
#define HAVE_NET_IF_H 1

/* Define to 1 if you have the <openssl/crypto.h> header file. */
#define HAVE_OPENSSL_CRYPTO_H 1

/* Define to 1 if you have the <openssl/engine.h> header file. */
#define HAVE_OPENSSL_ENGINE_H 1

/* Define to 1 if you have the <openssl/err.h> header file. */
#define HAVE_OPENSSL_ERR_H 1

/* Define to 1 if you have the <openssl/evp.h> header file. */
#define HAVE_OPENSSL_EVP_H 1

/* Define to 1 if you have the <openssl/ocsp.h> header file. */
#define HAVE_OPENSSL_OCSP_H 1

/* Define to 1 if you have the <openssl/ssl.h> header file. */
#define HAVE_OPENSSL_SSL_H 1

/* Define to 1 if you have the function pcap_dump_fopen. */
/* #undef HAVE_PCAP_DUMP_FOPEN */

/* Define to 1 if you have the function pcap_fopen_offline. */
/* #undef HAVE_PCAP_FOPEN_OFFLINE */

/* Define to 1 if you have the <pcap.h> header file. */
/* #undef HAVE_PCAP_H */

/* Define to 1 if you have the <prot.h> header file. */
/* #undef HAVE_PROT_H */

/* Define to 1 if you have the <pthread.h> header file. */
#define HAVE_PTHREAD_H 1

/* Define to 1 if you have the `pthread_sigmask' function. */
#define HAVE_PTHREAD_SIGMASK 1

/* Define to 1 if you have the <pwd.h> header file. */
#define HAVE_PWD_H 1

/* Define to 1 if you have the <readline.h> header file. */
/* #undef HAVE_READLINE_H */

/* Define if your readline library has \`add_history' */
/* #undef HAVE_READLINE_HISTORY */

/* Define to 1 if you have the <readline/history.h> header file. */
/* #undef HAVE_READLINE_HISTORY_H */

/* Define to 1 if you have the <readline/readline.h> header file. */
#define HAVE_READLINE_READLINE_H 1

/* define this if we have the <regex.h> header file */
#define HAVE_REGEX_H 

/* define this if we have REG_EXTENDED (from <regex.h>) */
#define HAVE_REG_EXTENDED 

/* Define to 1 if you have the <resource.h> header file. */
/* #undef HAVE_RESOURCE_H */

/* Define to 1 if you have the <semaphore.h> header file. */
#define HAVE_SEMAPHORE_H 1

/* Define to 1 if you have the `setlinebuf' function. */
#define HAVE_SETLINEBUF 1

/* Define to 1 if you have the `setresuid' function. */
#define HAVE_SETRESUID 1

/* Define to 1 if you have the `setsid' function. */
#define HAVE_SETSID 1

/* Define to 1 if you have the `setuid' function. */
#define HAVE_SETUID 1

/* Define to 1 if you have the `setvbuf' function. */
#define HAVE_SETVBUF 1

/* Define to 1 if you have the <siad.h> header file. */
/* #undef HAVE_SIAD_H */

/* Define to 1 if you have the <sia.h> header file. */
/* #undef HAVE_SIA_H */

/* Define to 1 if you have the `sigaction' function. */
#define HAVE_SIGACTION 1

/* Define to 1 if you have the <signal.h> header file. */
#define HAVE_SIGNAL_H 1

/* Define to 1 if you have the `sigprocmask' function. */
#define HAVE_SIGPROCMASK 1

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strlcat' function. */
/* #undef HAVE_STRLCAT */

/* Define to 1 if you have the `strlcpy' function. */
/* #undef HAVE_STRLCPY */

/* Define to 1 if you have the `strncasecmp' function. */
#define HAVE_STRNCASECMP 1

/* Define to 1 if you have the `strsep' function. */
#define HAVE_STRSEP 1

/* Define to 1 if you have the `strsignal' function. */
#define HAVE_STRSIGNAL 1

/* Generic DNS lookups */
#define HAVE_STRUCT_ADDRINFO 1

/* IPv6 address structure */
#define HAVE_STRUCT_IN6_ADDR 1

/* IPv6 socket addresses */
#define HAVE_STRUCT_SOCKADDR_IN6 1

/* Generic socket addresses */
#define HAVE_STRUCT_SOCKADDR_STORAGE 1

/* Define to 1 if you have the <syslog.h> header file. */
#define HAVE_SYSLOG_H 1

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_DIR_H */

/* Define to 1 if you have the <sys/fcntl.h> header file. */
#define HAVE_SYS_FCNTL_H 1

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_NDIR_H */

/* Define to 1 if you have the <sys/prctl.h> header file. */
#define HAVE_SYS_PRCTL_H 1

/* Define to 1 if you have the <sys/resource.h> header file. */
#define HAVE_SYS_RESOURCE_H 1

/* Define to 1 if you have the <sys/security.h> header file. */
/* #undef HAVE_SYS_SECURITY_H */

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/un.h> header file. */
#define HAVE_SYS_UN_H 1

/* Define to 1 if you have the <sys/wait.h> header file. */
#define HAVE_SYS_WAIT_H 1

/* Define if the compiler supports __thread */
/* #undef HAVE_THREAD_TLS */

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the <utmpx.h> header file. */
#define HAVE_UTMPX_H 1

/* Define to 1 if you have the <utmp.h> header file. */
#define HAVE_UTMP_H 1

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* Define to 1 if you have the <winsock.h> header file. */
/* #undef HAVE_WINSOCK_H */

/* define if you have OSFC2 authentication */
/* #undef OSFC2 */

/* define if you have OSFSIA authentication */
/* #undef OSFSIA */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* Posix-Style ctime_r */
#define POSIXSTYLE 1

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* Solaris-Style ctime_r */
#define SOLARISSTYLE 2

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* SYSV-Style get*byaddr_r */
#define SYSVSTYLE 2

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Include experimental support for DHCP */
/* #undef WITH_DHCP */

/* 1.x compatibility */
/* #undef WITH_POST_PROXY_AUTHORIZE */

/* define if you want udpfromto */
/* #undef WITH_UDPFROMTO */

/* define if you want VMPS support */
#define WITH_VMPS 1

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */

/* Define to 1 if on AIX 3.
   System headers sometimes define this.
   We just want to avoid a redefinition error message.  */
#ifndef _ALL_SOURCE
/* # undef _ALL_SOURCE */
#endif

/* Number of bits in a file offset, on hosts where this is settable. */
#define _FILE_OFFSET_BITS 64

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef gid_t */

/* Define to `long int' if <sys/types.h> does not define. */
/* #undef off_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* socklen_t is generally 'int' on systems which don't use it */
/* #undef socklen_t */

/* Define to `int' if <sys/types.h> doesn't define. */
/* #undef uid_t */

/* uint16_t should be the canonical '2 octets' for network traffic */
/* #undef uint16_t */

/* uint32_t should be the canonical 'network integer */
/* #undef uint32_t */

/* uint8_t should be the canonical 'octet' for network traffic */
/* #undef uint8_t */

/* define to something if you don't have ut_xtime in struct utmpx */
#define ut_xtime ut_tv.tv_sec
