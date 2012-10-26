
#include "stdafx.h"

#include <stdlib.h>
#include <windows.h>
#include <winbase.h>
#include <WinNT.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <freeradius-devel/dirent.h>

#include <string>

using namespace std;

static WIN32_FIND_DATA file_data; 
static HANDLE hSearch; 
static DIR find_dir;
static _dirent find_file;
static WIN32_FIND_DATA FileData; 
static BOOL is_first_find ;




char* rad_str_replace(char *v)
{
	
	char *str = v;
	while  (*(str++) != '\0')
	{
		if (*str == '/'){
			*str = '\\';
		}
	}
	if (*(v+strlen(v)-1) == '\\'){
		*(v+strlen(v)-1) = '\0';
	}
	return v;
}

DIR *opendir(const char *dir_name)
{
	strcpy(find_dir.dir_name,dir_name);
	struct _stat buf;
	/* Get data associated with "stat.c": */
	int result = _stat( rad_str_replace(find_dir.dir_name), &buf );

	/* Check if statistics are valid: */
	if( result != 0 ){
		return NULL;
	}

	is_first_find = FALSE;
	
	
	return &find_dir;   

}

struct dirent * readdir(DIR *dir)
{
	wchar_t tmp[MAX_PATH];
	char buf[MAX_PATH];
	//printf("list file:%s\r\n",buf);
	if (!is_first_find){
		
		memset(buf,0,MAX_PATH);
		memset(tmp,0,MAX_PATH);
		sprintf(buf,"%s\\%s",dir->dir_name,"*");

		int len = strlen(buf);
		mbstowcs(tmp,buf,len);
		
		
		hSearch = FindFirstFile(tmp, &FileData); 
		if (hSearch == INVALID_HANDLE_VALUE) 
		{ 
			return NULL;
		} 
		is_first_find = TRUE;
		memset(buf,0,MAX_PATH);
		wcstombs(buf,FileData.cFileName,wcslen(FileData.cFileName));
		memset(find_file.d_name,0,MAX_PATH);
		strcpy(find_file.d_name,buf);
		dir->hSearch = hSearch;
		return &find_file;
	}

	if (!FindNextFile(hSearch, &FileData)) 
    {

		return NULL;
	}
	memset(buf,0,MAX_PATH);
	memset(find_file.d_name,0,MAX_PATH);
	wcstombs(buf,FileData.cFileName,wcslen(FileData.cFileName));
		
	strcpy(find_file.d_name,buf);
	dir->hSearch = hSearch;
	return &find_file;
}
void closedir(DIR *dir)
{
	is_first_find = FALSE;
	FindClose(dir->hSearch);
}