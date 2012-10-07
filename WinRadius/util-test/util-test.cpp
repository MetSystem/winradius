// util-test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "..\include\rad_types.h"
#include "..\include\md5.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#define MAX_PASS_LEN (128)

#define AUTH_VECTOR_LEN 16

#define AUTH_PASS_LEN (AUTH_VECTOR_LEN)

int md5_test()
{
	FR_MD5_CTX context;
	uint8_t digest[16];

	fr_MD5Init(&context);
	fr_MD5Update(&context, (const uint8_t *)"aa", 2);
	//fr_MD5Update(&context, (const uint8_t *) secret, strlen(secret));
	fr_MD5Final(digest, &context);

	for (int i=0;i<16;i++)
	{
		printf("%02X",digest[i]);
	}
	printf("\n");
	return 1;
}



static void make_passwd(uint8_t *output, size_t *outlen,
			const uint8_t *input, size_t inlen,
			const char *secret, const uint8_t *vector)
{
	FR_MD5_CTX context, old;
	uint8_t	digest[AUTH_VECTOR_LEN];
	uint8_t passwd[MAX_PASS_LEN];
	int	i, n;
	int	len;

	/*
	 *	If the length is zero, round it up.
	 */
	len = inlen;

	if (len > MAX_PASS_LEN) len = MAX_PASS_LEN;

	memcpy(passwd, input, len);
	memset(passwd + len, 0, sizeof(passwd) - len);

	if (len == 0) {
		len = AUTH_PASS_LEN;
	}

	else if ((len & 0x0f) != 0) {
		len += 0x0f;
		len &= ~0x0f;
	}
	*outlen = len;

	fr_MD5Init(&context);
	fr_MD5Update(&context, (const uint8_t *) secret, strlen(secret));
	old = context;

	/*
	 *	Do first pass.
	 */
	fr_MD5Update(&context, vector, AUTH_PASS_LEN);

	for (n = 0; n < len; n += AUTH_PASS_LEN) {
		if (n > 0) {
			context = old;
			fr_MD5Update(&context,
				       passwd + n - AUTH_PASS_LEN,
				       AUTH_PASS_LEN);
		}

		fr_MD5Final(digest, &context);
		for (i = 0; i < AUTH_PASS_LEN; i++) {
			passwd[i + n] ^= digest[i];
		}
	}

	memcpy(output, passwd, len);
}


int _tmain_1(int argc, _TCHAR* argv[])
{

	SOCKET SendSocket;
	sockaddr_in RecvAddr;
	int Port = 27015;
	char SendBuf[1024];
	int BufLen = 1024;

	WSADATA wsaData;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	//---------------------------------------------
	// Create a socket for sending data
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//---------------------------------------------
	// Set up the RecvAddr structure with the IP address of
	// the receiver (in this example case "123.456.789.1")
	// and the specified port number.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	RecvAddr.sin_addr.s_addr = inet_addr("123.456.789.1");

	//---------------------------------------------
	// Send a datagram to the receiver
	printf("Sending a datagram to the receiver...\n");
	sendto(SendSocket, 
		SendBuf, 
		BufLen, 
		0, 
		(SOCKADDR *) &RecvAddr, 
		sizeof(RecvAddr));

	//---------------------------------------------
	// When the application is finished sending, close the socket.
	printf("Finished sending. Closing socket.\n");
	closesocket(SendSocket);

	//---------------------------------------------
	// Clean up and quit.
	printf("Exiting.\n");
	WSACleanup();




	
	return 0;
}

