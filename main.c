#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "strings.h"
#include "log.h"

/* #pragma comment(lib, "user32") */
#pragma comment(lib, "ws2_32")

#define LISTEN_PORT 80

BOOL bShutDown = FALSE;

unsigned long HandleRequest(const char * szRequest, const char * szRequestHeader, const char * szRequestBody, SOCKET * s)
{
	Log("##########");
	Log(szRequest);
	Log("----------");
	Log(szRequestHeader);
	Log("----------");
	Log(szRequestBody);
	Log("^^^^^^^^^^");
	
	return 0;
}

unsigned long WINAPI ThreadFunc(LPVOID lpParam)
{
	SOCKET * s = (SOCKET *)lpParam;
	char szTemp[STR_LEN], szRequest[STR_LEN] = {0};
	char szRequestHeader[STR_LEN] = {0}, szRequestBody[STR_LEN] = {0};
	char * pcTemp = 0;
	int ret = 0;
	int len = 0, nHeaderLength = 0, nContentLength = 0;
	
	while (len = recv(*s, szTemp, STR_LEN, 0))
	{
		strncat(szRequest, szTemp, len);
		
		if (szRequestHeader[0] == '\0')
		{
			/* Check if the header was completly received */
			nHeaderLength = (int)(strstr(szRequest, "\r\n\r\n") - szRequest);
			if (nHeaderLength > 0)
			{
				nHeaderLength += 2;
				sprintf(szTemp, "Header length: %d", nHeaderLength);
				Log(szTemp);

				/* Copy the request header aside */
				strncpy(szRequestHeader, szRequest, nHeaderLength);
				
				pcTemp = stristr(szRequestHeader, "Content-Length:");
				if (pcTemp)
				{
					/* Copy the "Content-Length" line */
					strncpy(szTemp, pcTemp, (int)(strchr(pcTemp, '\r') - pcTemp));

					Log(szTemp);

					/* Get the content length */
					len = strspn(&(szTemp[strlen("Content-Length:")]), " \t");
					nContentLength = atoi(&(szTemp[strlen("Content-Length:") + len - 1]));

					sprintf(szTemp, "Content-Length: %d", nContentLength);
					Log(szTemp);
				}
			}
		}

		if (szRequestHeader[0]) /* Header parsing is complete */
		{
			if (nContentLength)
			{
				if (szRequestBody[0] == '\0')
				{
					strcpy(szTemp, &(szRequest[nHeaderLength + 2]));
				}

				if (strlen(szRequestBody) < nContentLength)
				{
					strcat(szRequestBody, szTemp);
				}
			}

			if (strlen(szRequestBody) == nContentLength)
			{
				return HandleRequest(szRequest, szRequestHeader, szRequestBody, s);
			}
		}
	}

	return 0;
}

int main(int argc, char * argv[])
{
	int ret = 0, len = 0, thread_id = 0;
	HANDLE hThread = 0;
	WSADATA wsaData;
	char szTemp[STR_LEN] /*, szLogFile[STR_LEN] */;
	SOCKET sock, client;
	struct sockaddr_in sa;

	/*
	GetLogFilePath(szLogFile, STR_LEN);
	logFile = fopen(szLogFile, "a");
	*/

	Log("----------");
	Log("Loaded!");

	Log("WSAStartup()");
	ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		LogError(GetLastError());
		return 1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		Log("Wrong Winsock DLL version!");
		WSACleanup();
		return 1; 
	}
	
	Log("socket()");
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sock)
	{
		LogError(WSAGetLastError());
		WSACleanup();
		return 1;
	}

	Log("bind()");
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("0.0.0.0");
	sa.sin_port = htons(LISTEN_PORT);
	ret = bind(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (0 != ret)
	{
		LogError(WSAGetLastError());
		WSACleanup();
		return 1;
	}

	Log("listen()");
	ret = listen(sock, SOMAXCONN);
	if (0 != ret)
	{
		closesocket(sock);
		LogError(WSAGetLastError());
		WSACleanup();
		return 1;
	}

	Log("Entering loop.");

	while (!bShutDown)
	{
		Log("accept()");
		len = sizeof(sa);
		client = accept(sock, (struct sockaddr *)&sa, &len);
		if (INVALID_SOCKET == client)
		{
			closesocket(sock);
			LogError(WSAGetLastError());
			WSACleanup();
			return 1;
		}

		hThread = CreateThread(NULL, 0, ThreadFunc, (LPVOID)&client, 0, &thread_id);
		sprintf(szTemp, "hThread = %d", hThread);
		Log(szTemp);
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}
