// WinRadiusClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#include <freeradius-devel/libradius.h>

#define RADIUS_DICTIONARY	"dictionary"

static rbtree_t *filename_tree = NULL;

static int filename_walk(void *context, const char *data)
{
	const char	*filename = data;

	//context = context;	/* -Wunused */

	/*
	 *	Read request(s) from the file.
	 */
	//if (!radclient_init(filename)) {
	//	return 1;	/* stop walking */
	//}

	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{

	WSADATA wsaData;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	char* radius_dir="E:\\cpp\\WinRadius\\dictionary";
	if (dict_init(radius_dir, RADIUS_DICTIONARY) < 0) {
		fr_perror("radclient");
		return 1;
	}
/*
	filename_tree = rbtree_create(filename_cmp, NULL, 0);
	if (!filename_tree) {
		fprintf(stderr, "radclient: Out of memory\n");
		exit(1);
	}

	rbtree_insert(filename_tree, "C:\\aaa.txt");
	
	//If no '-f' is specified, we're reading from stdin.
	 
	if (rbtree_num_elements(filename_tree) == 0) {
		rbtree_insert(filename_tree, "-");
	}


	
	//Walk over the list of filenames, creating the requests.
	 
	if (rbtree_walk(filename_tree, InOrder, filename_walk, NULL) != 0) {
		exit(1);
	}
*/
	RADIUS_PACKET* request  = rad_alloc(1);
	int filedone;
	FILE* fp = fopen("C:\\aaa.txt","r");
	if (fp==NULL)
	{
		printf("fp is null\n");
		return 1;
	}

	request->vps = readvp2(fp, &filedone, "radclient:");
	
	rad_encode(request,NULL,"testing123");

	rad_print_hex(request);
	fclose(fp);

		WSACleanup();
	return 0;
}
