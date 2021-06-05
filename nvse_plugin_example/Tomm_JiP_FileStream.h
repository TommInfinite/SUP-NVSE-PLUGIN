//From JIP's Utility.h

class FileStream
{
	FILE* theFile;

protected:
	HANDLE		theFile2;
	UInt32		streamLength;
	UInt32		streamOffset;

public:
	FileStream() : theFile(NULL) {}
	~FileStream() { if (theFile) fclose(theFile); }

	bool Open(const char* filePath);
	bool OpenAt(const char* filePath, UInt32 inOffset);
	bool OpenWrite(char* filePath, bool append);
	bool Create(const char* filePath);
	void SetOffset(UInt32 inOffset);

	void Close()
	{
		fclose(theFile);
		theFile = NULL;
	}

	UInt32 GetLength();
	char ReadChar();
	void ReadBuf(void* outData, UInt32 inLength);
	void WriteChar(char chr);
	void WriteStr(const char* inStr);
	void WriteBuf(const void* inData, UInt32 inLength);
	int WriteFmtStr(const char* fmt, ...);

	static void MakeAllDirs(char* fullPath);
};

void FileStream::WriteStr(const char* inStr)
{
	fputs(inStr, theFile);
	fflush(theFile);
}

bool FileStream::Create(const char* filePath)
{
	if (theFile) fclose(theFile);
	theFile = _fsopen(filePath, "wb", 0x20);
	return theFile ? true : false;
}


UInt32 FileStream::GetLength()
{
	fseek(theFile, 0, SEEK_END);
	UInt32 result = ftell(theFile);
	rewind(theFile);
	return result;
}


bool FileStream::Open(const char* filePath)
{
	if (theFile) fclose(theFile);
	theFile = _fsopen(filePath, "rb", 0x20);
	return theFile ? true : false;
}

bool __fastcall FileExists(const char* path) /// From Stewie tweaks
{
	UInt32 attr = GetFileAttributes(path);
	return (attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}

//bool FileStream::OpenAt(const char* filePath, UInt32 inOffset)
//{
//	theFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//	if (theFile == INVALID_HANDLE_VALUE)
//		return false;
//	streamLength = GetFileSize(theFile, NULL);
//	streamOffset = inOffset;
//	if (streamOffset >= streamLength)
//	{
//		Close();
//		return false;
//	}
//	if (streamOffset)
//		SetFilePointer(theFile, streamOffset, NULL, FILE_BEGIN);
//	return true;
//}


void FileStream::ReadBuf(void* outData, UInt32 inLength)
{
	UInt32 bytesRead;
	ReadFile(theFile2, outData, inLength, &bytesRead, NULL);
	streamOffset += bytesRead;
}

bool FileStream::OpenAt(const char* filePath, UInt32 inOffset)
{
	theFile2 = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (theFile2 == INVALID_HANDLE_VALUE)
		return false;
	streamLength = GetFileSize(theFile2, NULL);
	streamOffset = inOffset;
	if (streamOffset >= streamLength)
	{
		Close();
		return false;
	}
	if (streamOffset)
		SetFilePointer(theFile2, streamOffset, NULL, FILE_BEGIN);
	return true;
}