#include "StdAfx.h"
#include "tt.h"

#include <Windows.h>
#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/ocsp.h>
typedef void *lt_dlhandle;

lt_dlhandle lt_dlopenext(const char *name)
{
	wchar_t wcsname[MAX_PATH];
	mbstowcs(wcsname,name,strlen(name));
	wcsname[strlen(name)]='\0';
	HMODULE  hModule = LoadLibrary(wcsname);
	if (hModule == NULL) {
		return NULL;
	}

	return hModule;
}

void *lt_dlsym(lt_dlhandle handle,  const char *symbol)
{
	//wchar_t wcsname[MAX_PATH];
	//mbstowcs(wcsname,symbol,strlen(symbol));
	//wcsname[strlen(symbol)]='\0';
	return GetProcAddress((HMODULE)handle, symbol); 
}

int lt_dlclose(lt_dlhandle handle)
{
	return 0;
}

const char *lt_dlerror(void)
{
	return "";
}

int main()
{
	
	lt_dlhandle h =lt_dlopenext("rlm_eap");
	
	void * p =
		lt_dlsym(h,"rlm_eap");
	p;
	
	/*
	HMODULE  hModule = LoadLibrary(_T("rlm_exec"));
	if (hModule == NULL) {
		return NULL;
	}
	*/
	//void * p = GetProcAddress((HMODULE)hModule, "rlm_exec"/*wcsname*/); 

	//SSL_CTX_new(NULL);

	return 0;
}