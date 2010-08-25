// stdafx.cpp : source file that includes just the standard includes
// NewView.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information


#include <toolbox_stdafx.h>

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file


#ifdef _MSC_VER
// Windows user assets path
void GetWorkingFileName (const char* name, char* outPathName)
{
	char path[256]; 
	GetModuleFileNameA(NULL, path, 256);

	char* ptr = strstr (path, "samples");
	ptr [0] = 0;
	sprintf (outPathName, "%ssamples/bin/%s", path, name);
}
#endif

#ifdef _MIPS_ARCH
	// Mac user assets path
//	#define ASSETS_PATH "."
//	void GetWorkingFileName (const char* name, char* outPathName)
//	{
//		sprintf (outPathName, "%s/%s", ASSETS_PATH, name);
//	}


	
	void GetWorkingFileName (const char* name, char* outPathName) 
	{ 
		CFURLRef appURL = CFBundleCopyBundleURL( CFBundleGetMainBundle() ); 
		CFURLRef appDirURL = CFURLCreateCopyDeletingLastPathComponent( NULL, appURL); 
		CFStringRef fileName = CFStringCreateWithCString( NULL, name, kCFStringEncodingUTF8 ); 
		CFURLRef fileURL = CFURLCreateCopyAppendingPathComponent( NULL, appDirURL, fileName, false ); 
		CFStringRef filePath = CFURLCopyFileSystemPath( fileURL, kCFURLPOSIXPathStyle ); 
		CFStringGetCString (filePath, outPathName, PATH_MAX, kCFStringEncodingUTF8); 

		// Clean up 
		CFRelease( appURL ); 
		CFRelease( appDirURL ); 
		CFRelease( fileName ); 
		CFRelease( fileURL ); 
		CFRelease( filePath ); 
	}

#endif


#ifdef _LINUX_VER
	// Linux user assets path
	#define ASSETS_PATH "newtonSDK/samples/bin"
	void GetWorkingFileName (const char* name, char* outPathName)
	{
		char *env;
		env = getenv("HOME");
		sprintf (outPathName, "%s/%s/%s", env, ASSETS_PATH, name);
	}
#endif



	// little Indian/big Indian conversion
#ifdef __ppc__
	unsigned short SWAP_INT16(unsigned short x)
	{
		return ((x >> 8) & 0xff) + ((x & 0xff) << 8);
	}
	unsigned SWAP_INT32(unsigned x)
	{
		return SWAP_INT16 ( x >> 16) + (SWAP_INT16 (x) << 16);
	}


	void SWAP_FLOAT32_ARRAY (const void *array, dInt32 count)
	{
		dInt32* ptr;

		ptr = (dInt32*) array;
		count /= sizeof (dInt32);
		for (dInt32 i = 0; i < count; i ++) {
			dInt32 x;
			x = SWAP_INT32 (ptr[i]);
			ptr[i] = x;
		}
	}

#else

	unsigned SWAP_INT32(unsigned x)
	{
		return x;
	}

	unsigned short SWAP_INT16(unsigned short x)
	{
		return x;
	}

	void SWAP_FLOAT32_ARRAY (void *array, dInt32 count)
	{
	}

#endif



