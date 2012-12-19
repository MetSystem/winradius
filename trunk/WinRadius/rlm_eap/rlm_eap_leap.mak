#;


INCLUDE=/I"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include" /I"D:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include" /I"F:\svn\existing-winradius-code-0\winradius-code-0\WinRadius\src" /I"D:\openssl\openssl_winxp_vs2008_x86\openssl\include"

LINK=link

LIBPATH= /LIBPATH:"F:\svn\existing-winradius-code-0\winradius-code-0\WinRadius\winradiusd\Debug" /LIBPATH:"F:\svn\existing-winradius-code-0\winradius-code-0\WinRadius\rlm_eap\Debug" /LIBPATH:"D:\openssl\openssl_winxp_vs2008_x86\openssl\lib" /LIBPATH:"D:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib"

LINKFLAGS=/INCREMENTAL /NOLOGO /DLL  /MANIFEST  /ALLOWISOLATION /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /DEBUG  /SUBSYSTEM:WINDOWS /TLBID:1 /DYNAMICBASE /NXCOMPAT /MACHINE:X86 /ERRORREPORT:QUEUE /NODEFAULTLIB:MSVCRTD.lib

CORELIBS= "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib"

LIBS=smbdes.obj soh.obj sha1.obj hmacsha1.obj ssleay32.lib libeay32.lib Ws2_32.lib acct.obj auth.obj client.obj command.obj conffile.obj crypt.obj detail.obj dhcpd.obj dict.obj dirent.obj evaluate.obj event-lib.obj event.obj exec.obj fifo.obj getaddrinfo.obj getopt.obj hash.obj heap.obj hmac.obj isaac.obj listen.obj log-lib.obj log.obj mainconfig.obj md4.obj md5.obj misc.obj missing.obj modcall.obj modules.obj mschap.obj packet.obj print.obj radius.obj radiusd.obj rbtree.obj realms.obj snprintf.obj stats.obj stdafx.obj strlcat.obj strlcpy.obj token.obj util.obj valuepair-lib.obj valuepair.obj version.obj winradiusd.obj xlat.obj cb.obj eap.obj eapcommon.obj eapcrypto.obj eapsimlib.obj eap_tls.obj eap_tnc.obj fips186prf.obj mem.obj mppe_keys.obj peap.obj rlm_eap.obj tls.obj tncs_connect.obj ttls.obj 

#CPPFLAGS=/ZI /nologo /W3 /WX- /Od /Oy- /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "RLM_EAP_MD5_EXPORTS" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /Gm /EHsc /RTC1 /GS /fp:precise /Zc:wchar_t /Zc:forScope  /Fp"Debug\rlm_eap.pch"   /Fd"Debug\vc100.pdb" /Gd /analyze- /errorReport:queue 

CPPFLAGS=/ZI /nologo /W3 /WX- /Od /Oy- /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "RLM_EAP_EXPORTS" /D "_WINDLL"  /Gm /EHsc /RTC1 /GS /fp:precise  /Zc:forScope /Fp"Debug\rlm_eap.pch"  /Gd /analyze- /errorReport:queue 

#CPPFLAGS=/Gd /D "_DEBUG" /D "_USRDLL" /D "_WINDLL" /D "AMD64"


rlm_eap_leap.dll:cb.obj eap.obj eapcommon.obj eapcrypto.obj eapsimlib.obj eap_leap.obj eap_md5.obj eap_tls.obj eap_tnc.obj fips186prf.obj mem.obj mppe_keys.obj peap.obj rlm_eap.obj rlm_eap_leap.obj  rlm_eap_peap.obj rlm_eap_sim.obj rlm_eap_tls.obj rlm_eap_tnc.obj rlm_eap_ttls.obj smbdes.obj tls.obj tncs_connect.obj ttls.obj 
	$(LINK) /DLL /out:..\DEBUG\rlm_eap_leap.dll $(LIBPATH)  $(LIBS) $(LINKFLAGS)
.cpp.obj:
	$(CPP) $(CPPFLAGS) $(INCLUDE) /c $<
