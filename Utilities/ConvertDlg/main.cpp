#include "Fuji.h"
#include "MFStringCache.h"

uint16 *gpArray[128];
char stringBuffer[2048];

uint16* ConvertToUnicode(char *pBuffer, int size)
{
	uint16 *pNewBuffer = (uint16*)malloc(size*2 + 2);

	for(int a = 0; a < size; a++)
	{
		pNewBuffer[a] = pBuffer[a];
	}

	free(pBuffer);

	return pNewBuffer;
}

uint16* GetStringCopy(const uint16* pString, int len)
{
	int l = len < 0 ? MFWString_Length(pString) : len;

	uint16 *pStr = (uint16*)malloc(sizeof(uint16) * (l + 1));

	MFWString_CopyN(pStr, pString, l);
	pStr[l] = 0;

	if(l && *pStr == '\"' && pStr[l-1] == '\"')
	{
		pStr[l-1] = 0;
		++pStr;
	}

	return pStr;
}

int GetSizeFromPointers(const uint16 *p1, const uint16 *p2)
{
	return ((uint32&)p2 - (uint32&)p1) >> 1;
}

uint16** GetLine(const uint16 *&pString, uint16 **ppOutput = gpArray)
{
	bool inQuotes = false;

	const uint16 *pS = pString;
	int column = 0;

	while(*pString)
	{
		if(*pString == '\"')
			inQuotes = !inQuotes;

		if((*pString == ',' || MFIsNewline(*pString)) && !inQuotes)
		{
			int strLen = GetSizeFromPointers(pS, pString);

			if(strLen)
			{
				ppOutput[column] = GetStringCopy(pS, strLen);
			}
			else
			{
				ppOutput[column] = NULL;
			}

			if(MFIsNewline(*pString))
				break;

			pS = pString+1;
			++column;
		}

		++pString;
	}

	while(MFIsNewline(*pString))
		++pString;

	return ppOutput;
}

const char *GetPagedString(const uint16 *pString)
{
	// write string into stringBuffer[]
	int len = MFWString_Length(pString);

	for(int a=0; a<len; a++)
	{
		stringBuffer[a] = (uint8)pString[a];
	}

	stringBuffer[len] = 0;

	return stringBuffer;
}

const char *GetAnsiString(const uint16 *pString)
{
	int len = MFWString_Length(pString);

	for(int a=0; a<len; a++)
	{
		if(pString[a] > 255)
			return NULL;
		stringBuffer[a] = (uint8)pString[a];
	}

	stringBuffer[len] = 0;

	return stringBuffer;
}

const uint16 *GetUnicodeString(const uint16 *pString)
{
	if(*pString == 0xFEFF)
		return pString;

	int len = MFWString_Length(pString);

	uint16 *pT = (uint16*)stringBuffer;

	*pT = 0xFEFF;
	++pT;

	for(int a=0; a<len; a++)
	{
		pT[a] = pString[a];
	}

	pT[len] = 0;

	return &pT[-1];
}

struct MFStringTable
{
	uint32 magic;
	int numStrings;
};

int main(int argc, char *argv[])
{
	char fileName[256] = "";
	char outPath[256] = "";

	int a;

	// process command line
	for(a=1; a<argc; a++)
	{
		if(argv[a][0] == '-' || argv[a][0] == '/')
		{
			if(!MFString_CaseCmp(&argv[a][1], "v") || !MFString_CaseCmp(&argv[a][1], "version"))
			{
				// print version
				return 0;
			}
		}
		else
		{
			if(!fileName[0])
				MFString_Copy(fileName, argv[a]);
			else if(!outPath[0])
				MFString_Copy(outPath, argv[a]);
		}
	}

	if(!fileName[0])
	{
		printf("No file specified...\n");
		return 1;
	}

	FILE *pFile = fopen(fileName, "rb");

	if(!pFile)
	{
		// error
		return 1;
	}

	// get file size
	fseek(pFile, 0, SEEK_END);
	int size = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	// read file
	uint16 *pBuffer = (uint16*)malloc(size+2);
	fread(pBuffer, size, 1, pFile);

	if(*pBuffer != 0xFEFF)
	{
		// convert ascii to unicode
		pBuffer = ConvertToUnicode((char*)pBuffer, size);
	}
	else
	{
		size >>= 1;

		++pBuffer;
		--size;
	}

	pBuffer[size] = 0;

	// parse csv file
	int columnCount = 1;
	int lineCount = 1;

	int totalLines = 0;

	MFStringCache **ppCaches;
	MFWStringCache **ppWCaches;

	uint16 ***pppStringPointers = NULL;
	uint16 **ppColumnNames;

	// count the number of columns
	bool inQuotes = false;

	const uint16 *pT = pBuffer;
	while(*pT && *pT != '\n')
	{
		if(*pT == '\"')
			inQuotes = !inQuotes;

		if(*pT == ',' && !inQuotes)
			++columnCount;

		++pT;
	}

	// count the number of entries
	inQuotes = false;
	pT = pBuffer;
	for(a=0; a<size; a++)
	{
		if(*pT == '\"')
			inQuotes = !inQuotes;

		if(*pT == '\n' && !inQuotes)
			lineCount++;

		++pT;
	}

	// assert that there are more than 1 lines
	if(lineCount <= 1)
	{
		// we dont have any actual dialog
		return 2;
	}

	// allocate some lists of stuff
	ppCaches = (MFStringCache**)malloc(sizeof(MFStringCache*) * columnCount);
	ppWCaches = (MFWStringCache**)malloc(sizeof(MFWStringCache*) * columnCount);

	ppColumnNames = (uint16**)malloc(sizeof(uint16*) * columnCount);

	pppStringPointers = (uint16***)malloc(sizeof(uint16**) * columnCount);
	for(a=0; a<columnCount; a++)
		pppStringPointers[a] = (uint16**)malloc(sizeof(uint16*) * lineCount);

	// get the names of each column
	pT = pBuffer;
	GetLine(pT, ppColumnNames);

	while(*pT)
	{
		uint16 **ppLine = GetLine(pT);

		// check if theres anything on the line
		bool emptyLine = true;
		for(a=0; a<columnCount; a++)
		{
			if(ppLine[a] != NULL)
			{
				emptyLine = false;
				break;
			}
		}

		if(emptyLine)
			continue;

		for(a=0; a<columnCount; a++)
		{
			uint16 *pText = ppLine[a];

			if(!pText && ppColumnNames[a])
				pText = L"Missing From Translation";

			pppStringPointers[a][totalLines] = pText;
		}

		++totalLines;
	}

	// now parse each language into font pages
	for(a=0; a<columnCount; a++)
	{
		if(ppColumnNames[a])
		{
			ppCaches[a] = MFStringCache_Create(size);
			ppWCaches[a] = MFWStringCache_Create(size * 2);

			for(int b=0; b<totalLines; b++)
			{
				const char *pAnsiString = GetAnsiString(pppStringPointers[a][b]);
				if(pAnsiString)
					pppStringPointers[a][b] = (uint16*)MFStringCache_Add(ppCaches[a], pAnsiString);
				else
					pppStringPointers[a][b] = (uint16*)MFWStringCache_Add(ppWCaches[a], GetUnicodeString(pppStringPointers[a][b]));
			}
		}
		else
		{
			ppCaches[a] = NULL;
			ppWCaches[a] = NULL;
		}
	}

	// write out data
	char file[256];

	char *pSlash = MFString_RChr(fileName, '/');
	if(!pSlash)
		pSlash = MFString_RChr(fileName, '\\');
	if(!pSlash)
		pSlash = fileName;
	else
		++pSlash;

	MFString_Copy(file, pSlash);
	char *pC = MFString_RChr(file, '.');
	if(pC)
		*pC = 0;

	int l = MFString_Length(outPath);
	if(l && (outPath[l-1] != '/' && outPath[l-1] != '\\'))
	{
		outPath[l] = '/';
		outPath[l+1] = 0;
	}

	for(a=0; a<columnCount; a++)
	{
		if(!ppColumnNames[a])
			continue;

		char outputFilename[256];

		if(!MFWString_CaseCmp(ppColumnNames[a], L"Enum"))
		{
			// write out enum include files
			sprintf(outputFilename, "%s.h", file);
//			sprintf(outputFilename, "%s%s.h", outPath, file);

			FILE *pOut = fopen(outputFilename, "w");

			fprintf(pOut, "/*** %s.h, generated by ConvertDlg.exe ***/\n\n", file);

			for(int b=0; b<totalLines; b++)
			{
				fprintf(pOut, "#define %s %d\n", pppStringPointers[a][b], b);
			}

			fclose(pOut);
		}

		// write out language file
		MFStringTable table;
		table.magic = MFMAKEFOURCC('D','L','G','1');
		table.numStrings = totalLines;

		char *pCache = MFStringCache_GetCache(ppCaches[a]);
		int cacheSize = MFStringCache_GetSize(ppCaches[a]);
		uint16 *pWCache = MFWStringCache_GetCache(ppWCaches[a]);
		int wcacheSize = MFWStringCache_GetSize(ppWCaches[a]);
		uint32 base = (uint32&)pCache;
		uint32 wbase = (uint32&)pWCache;
		base -= sizeof(char**) * totalLines + sizeof(table) + wcacheSize;
		wbase -= sizeof(char**) * totalLines + sizeof(table);

		for(int b=0; b<totalLines; b++)
		{
			// fixup
			if(*pppStringPointers[a][b] == 0xFEFF)
                (uint32&)pppStringPointers[a][b] -= wbase;
			else
                (uint32&)pppStringPointers[a][b] -= base;
		}

		// write out the string cache
		sprintf(outputFilename, "%s%s.%s", outPath, file, GetPagedString(ppColumnNames[a]));

		FILE *pOut = fopen(outputFilename, "wb");

		fwrite(&table, sizeof(table), 1, pOut);
		fwrite(pppStringPointers[a], totalLines, sizeof(char **), pOut);
		fwrite(pWCache, wcacheSize, 1, pOut);
		fwrite(pCache, cacheSize, 1, pOut);

		fclose(pOut);

		printf("> %s\n", outputFilename);
	}

	return 0;
}
