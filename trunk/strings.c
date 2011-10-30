#include <string.h>

#include "strings.h"

char * stristr(const char * szHaystack, const char * szNeedle)
{
	char szTmpHaystack[STR_LEN], szTmpNeedle[STR_LEN];
	char * ret = 0;
	
	strcpy(szTmpHaystack, szHaystack);
	strcpy(szTmpNeedle, szNeedle);
	
	_strlwr(szTmpHaystack);
	_strlwr(szTmpNeedle);
	
	ret = strstr(szTmpHaystack, szTmpNeedle);
	
	if (ret)
	{
		return ret - szTmpHaystack + (char *)szHaystack;
	}
	
	return 0;
}

char * strchrrev(const char * szHaystack, const char cNeedle)
{
	char szTemp[STR_LEN];
	char * ret = 0;

	strcpy(szTemp, szHaystack);
	strrev(szTemp);
	
	ret = strchr(szTemp, (int)cNeedle);

	if (!ret)
	{
		return 0;
	}

	return szHaystack + STR_LEN - (ret - szTemp);
}