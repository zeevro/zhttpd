#include <windows.h>
#include <stdio.h>

#include "log.h"
#include "strings.h"

void GetLogFilePath(char * path, const int len)
{
	int bytesRead;
	bytesRead = GetEnvironmentVariable("TEMP", path, len);
	strcat(path, "\\");
	strcat(path, LOG_FILE);
}

void Log(const char * text)
{
	//fprintf(logFile, "%s\n", text);
	printf("%s\n", text);
}

void LogError(const int id)
{
	char szErrorMsg[STR_LEN], szTemp[STR_LEN];
	FormatMessage(FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_FROM_SYSTEM, NULL, id, 0, szErrorMsg, STR_LEN, NULL);
	sprintf(szTemp, "ERROR: %s", szErrorMsg);
	Log(szTemp);
}