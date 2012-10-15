#ifndef DIRENT_H
#define DIRENT_H


typedef struct 
{
	HANDLE hSearch;
	char dir_name[_MAX_PATH];
} DIR;


typedef struct dirent {
	char d_name[_MAX_PATH];
} _dirent;

typedef struct dirent _dirent;

DIR *opendir(const char *dir);

struct dirent * readdir(DIR *dir);

void closedir(DIR *dir);


#endif //DIRENT_H