
DEFINE_COMMAND_ALT_PLUGIN(CaptureScreenshot, Screenshot, "", 0, 2, kParams_Tomm_CaptureScreenshot)
DEFINE_COMMAND_PLUGIN(CaptureScreenshotAlt, "", 0, 9, kParams_Tomm_CaptureScreenshotAlt)
DEFINE_COMMAND_PLUGIN(DeleteScreenshot, "", 0, 2, kParams_Tomm_DeleteScreenshot)





#include <stdio.h>
#include <windows.h>
#include <gdiplus.h>
#include <time.h>
#include <direct.h>  


/******************************************************************************
 * Checks to see if a directory exists. Note: This method only checks the
 * existence of the full path AND if path leaf is a dir.
 *
 * @return  >0 if dir exists AND is a dir,
 *           0 if dir does not exist OR exists but not a dir,
 *          <0 if an error occurred (errno is also set)
 *****************************************************************************/
int dirExists(const char* const path)
{
	struct stat info;

	int statRC = stat(path, &info);
	if (statRC != 0)
	{
		if (errno == ENOENT) { return 0; } // something along the path does not exist
		if (errno == ENOTDIR) { return 0; } // something in path prefix is not a dir
		return -1;
	}

	return (info.st_mode & S_IFDIR) ? 1 : 0;
}


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
	using namespace Gdiplus;
	UINT  num = 0;
	UINT  size = 0;

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;

	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return 0;
}

void gdiscreenAlt(char* s_strFolderName, char* s_strArgTemp, float fPosXStart, float fPosXEnd, float fPosYStart, float fPosYEnd, int Format, int iUseGameResolution, int iQuality) {
	using namespace Gdiplus;
	IStream* istream;
	HRESULT res = CreateStreamOnHGlobal(NULL, true, &istream);
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	{

		float fpicSizeX = fPosXEnd - fPosXStart;
		float fpicSizeY = fPosYEnd - fPosYStart;

		HDC scrdc, memdc;
		HBITMAP membit;
		scrdc = ::GetDC(0);
		//int Height = GetSystemMetrics(SM_CYSCREEN);
		//int Width = GetSystemMetrics(SM_CXSCREEN);
		//Console_Print("%s %d", "Height>>", Height);
		//Console_Print("%s %d", "Width>>", Width);
		memdc = CreateCompatibleDC(scrdc);
		//_MESSAGE("Current SUP stage %i", 6);
		//membit = CreateCompatibleBitmap(scrdc, Width, Height);

		if (iUseGameResolution == 1)
		{
			Tile* component = g_interfaceManager->menuRoot;
			Tile::Value* val = component->GetValue(kTileValue_resolutionconverter);
			float fConverter = val->num;
			fpicSizeX = fpicSizeX / fConverter;
			fpicSizeY = fpicSizeY / fConverter;
		}


		membit = CreateCompatibleBitmap(scrdc, fpicSizeX, fpicSizeY);
		//_MESSAGE("Current SUP stage %i", 7);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, membit);
		//BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);
		//_MESSAGE("Current SUP stage %i", 8);
		BitBlt(memdc, 0, 0, fpicSizeX, fpicSizeY, scrdc, fPosXStart, fPosYStart, SRCCOPY);
		Gdiplus::Bitmap bitmap(membit, NULL);
		CLSID clsid;
		//_MESSAGE("Current SUP stage %i", 9);


		if (dirExists("Data\\textures") > 0)
		{}
		else
		{
			_mkdir("Data\\textures");
			_MESSAGE("%s %s", "Creating textures directory.Really?>>", "Data\\textures");
		}



		if (dirExists("Data\\textures\\SUPScreenshots") > 0)
		{}
		else
		{
			_mkdir("Data\\textures\\SUPScreenshots");
			_MESSAGE("%s %s", "Creating MAIN directory>>", "Data\\textures\\SUPScreenshots");
		}

		char s_TempFolderCheck[0x4000]{};
		strcat(s_TempFolderCheck, "Data\\textures\\SUPScreenshots\\");
		strcat(s_TempFolderCheck, s_strFolderName);

		if (dirExists(s_TempFolderCheck) > 0)
		{}
		else
		{
			_mkdir(s_TempFolderCheck);
			_MESSAGE("%s %s", "Creating TARGET directory>>", s_TempFolderCheck);
		}


		//rmdir("testdir");
		//remove("[DEBUG LOG]Solid Project--2021-03-27--22-45-49.txt");

		switch (Format)
		{
		case 0:
			strcat(s_strArgTemp, ".jpg");
			GetEncoderClsid(L"image/jpeg", &clsid);
			break;
		case 1:
			strcat(s_strArgTemp, ".bmp");
			GetEncoderClsid(L"image/bmp", &clsid);
			break;
		case 2:
			strcat(s_strArgTemp, ".gif");
			GetEncoderClsid(L"image/gif", &clsid);
			break;
		case 3:
			strcat(s_strArgTemp, ".tiff");
			GetEncoderClsid(L"image/tiff", &clsid);
			break;
		case 4:
			strcat(s_strArgTemp, ".png");
			GetEncoderClsid(L"image/png", &clsid);
			break;
		case 5:
			strcat(s_strArgTemp, ".dds");
			GetEncoderClsid(L"image/png", &clsid);
			break;
		case 6:
			strcat(s_strArgTemp, ".dds");
			GetEncoderClsid(L"image/jpeg", &clsid);
			break;
		}

		char s_FinalPath[0x4000]{};
		strcat(s_FinalPath, "Data\\textures\\SUPScreenshots\\");
		strcat(s_FinalPath, s_strFolderName);
		strcat(s_FinalPath, "\\");
		strcat(s_FinalPath, s_strArgTemp);

		std::wstring w;
		std::copy(s_FinalPath, s_FinalPath + strlen(s_FinalPath), back_inserter(w));
		const WCHAR* filename = w.c_str();



		if (Format == 0 || Format == 6)
		{
			if (iQuality == -999)
			{iQuality = 100;}
			CLSID             encoderClsid;
			EncoderParameters encoderParameters;
			ULONG             quality = (unsigned long)iQuality;
			Status            stat;

			encoderParameters.Count = 1;
			encoderParameters.Parameter[0].Guid = EncoderQuality;
			encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
			encoderParameters.Parameter[0].NumberOfValues = 1;
			encoderParameters.Parameter[0].Value = &quality;
			bitmap.Save(filename, &clsid, &encoderParameters);
			bitmap.Save(istream, &clsid, &encoderParameters);
		}
		else {
			bitmap.Save(filename, &clsid, NULL);
			bitmap.Save(istream, &clsid, NULL);
		}

		// Create a bitmap from the stream and save it to make sure the stream has the image
		//Gdiplus::Bitmap bmp(istream, NULL); ///////
		//bmp.Save(filename, &clsid, NULL); ////////

		delete& clsid;
		DeleteObject(memdc);
		DeleteObject(membit);
		::ReleaseDC(0, scrdc);
	}
	GdiplusShutdown(gdiplusToken);

}

//int main()
//{
//	clock_t t1 = clock();
//	int i;
//	int iterations = 1;
//	for (i = 0;i < iterations;i++) {
//		gdiscreen();
//	}
//	clock_t t2 = clock();
//	printf("%d iterations: %0.0f fps\n", iterations, iterations / ((double)(t2 - t1) / CLOCKS_PER_SEC));
//	return 0;
//}





bool Cmd_CaptureScreenshotAlt_Execute(COMMAND_ARGS)
{
	char s_strFolderName[0x4000];
	char s_strArgTemp[0x4000];

	float fPosXStart, fPosXEnd, fPosYStart, fPosYEnd;
	int Format, iUseGameResolution, iQuality = -999;

	//_MESSAGE("Current SUP stage %i", 0);
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strFolderName, &s_strArgTemp, &fPosXStart, &fPosXEnd, &fPosYStart, &fPosYEnd, &Format, &iUseGameResolution, &iQuality))
	{
		gdiscreenAlt(s_strFolderName, s_strArgTemp, fPosXStart, fPosXEnd, fPosYStart, fPosYEnd, Format, iUseGameResolution, iQuality);
	}
	return true;

}


#define _WIN32_LEAN_AND_MEAN
#include <windows.h>
//Credits to Inline
bool sanitizePath(const char* parent, const char* relPath) {
	char fullPath[MAX_PATH] = "";

	if (!GetFullPathNameA(relPath, MAX_PATH, fullPath, nullptr)) {
		return false;
	}

	if (strstr(fullPath, parent) != fullPath) {
		return false;
	}

	
	return true;
}

bool Cmd_DeleteScreenshot_Execute(COMMAND_ARGS)
{
	char s_strFolderName[0x4000]{};
	char s_strArgTemp[0x4000]{};
	char s_TempPathRelative[0x4000]{};
	char s_FullPathRelative[0x4000]{};


	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strFolderName, &s_strArgTemp))
	{
		sprintf(s_TempPathRelative, "Data\\textures\\SUPScreenshots\\%s\\%s", s_strFolderName, s_strArgTemp);
		sprintf(s_FullPathRelative, "%s%s", FalloutFolderPath, s_TempPathRelative);
		
		bool bRes = sanitizePath(s_FullPathRelative, s_TempPathRelative);

		if ((bRes) == true)
		{
			if (FileExists (s_FullPathRelative))
			{
				*result = 1;
				remove(s_FullPathRelative);
			}else 
			{
				*result = -1;
			}
		}else
		{
			*result = -2;
		}

		//Console_Print("%s %d", "BOOL>>", bRes);
		//Console_Print("%s %s", "Relative path>>", s_TempPathRelative);
		//Console_Print("%s %s", "s_FullPathRelative>>", s_FullPathRelative);
		//Console_Print("%s %s", "FalloutFolder>>", FalloutFolderPath);
		//Console_Print("%s %f", "*result>>", *result);
	}



	return true;
}



void gdiscreen( char* s_strArgTemp,  int Format, int iNumberArgs) {
	using namespace Gdiplus;
	IStream* istream;
	HRESULT res = CreateStreamOnHGlobal(NULL, true, &istream);
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	{


		//Console_Print("%s %d", "iNumberArgs>>", iNumberArgs);

		float fpicSizeX = g_screenWidth;
		float fpicSizeY = g_screenHeight;

		//Console_Print("%s %d", "g_screenWidth>>", g_screenWidth);
		//Console_Print("%s %d", "g_screenHeight>>", g_screenHeight);

		HDC scrdc, memdc;
		HBITMAP membit;
		scrdc = ::GetDC(0);

		memdc = CreateCompatibleDC(scrdc);

		membit = CreateCompatibleBitmap(scrdc, fpicSizeX, fpicSizeY);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, membit);
		BitBlt(memdc, 0, 0, fpicSizeX, fpicSizeY, scrdc, 0, 0, SRCCOPY);
		Gdiplus::Bitmap bitmap(membit, NULL);
		CLSID clsid;


		if (iNumberArgs == 0)
		{
			strcat(s_strArgTemp, "a_Screenshot");
		}

		if (iNumberArgs <2)
		{
			Format = 0;
		}


		switch (Format)
		{
		case 0:
			strcat(s_strArgTemp, ".jpg");
			GetEncoderClsid(L"image/jpeg", &clsid);
			break;
		case 1:
			strcat(s_strArgTemp, ".bmp");
			GetEncoderClsid(L"image/bmp", &clsid);
			break;
		case 2:
			strcat(s_strArgTemp, ".gif");
			GetEncoderClsid(L"image/gif", &clsid);
			break;
		case 3:
			strcat(s_strArgTemp, ".tiff");
			GetEncoderClsid(L"image/tiff", &clsid);
			break;
		case 4:
			strcat(s_strArgTemp, ".png");
			GetEncoderClsid(L"image/png", &clsid);
			break;
		case 5:
			strcat(s_strArgTemp, ".dds");
			GetEncoderClsid(L"image/png", &clsid);
			break;
		case 6:
			strcat(s_strArgTemp, ".dds");
			GetEncoderClsid(L"image/jpeg", &clsid);
			break;
		}



		std::wstring w;
		std::copy(s_strArgTemp, s_strArgTemp + strlen(s_strArgTemp), back_inserter(w));
		const WCHAR* filename = w.c_str();

		bitmap.Save(filename, &clsid, NULL); // To save the jpeg to a file


		bitmap.Save(istream, &clsid, NULL);

		delete& clsid;
		DeleteObject(memdc);
		DeleteObject(membit);
		::ReleaseDC(0, scrdc);
	}
	GdiplusShutdown(gdiplusToken);
}














bool Cmd_CaptureScreenshot_Execute(COMMAND_ARGS)
{

	char s_strArgTemp[0x4000]{};

	int Format;
	int iNumberArgs = NUM_ARGS;

	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgTemp, &Format))
	{
		gdiscreen(s_strArgTemp, Format, iNumberArgs);
	}
	return true;

}

