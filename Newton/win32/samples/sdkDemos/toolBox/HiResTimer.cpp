//********************************************************************
// Newton Game dynamics 
// copyright 2000-2004
// By Julio Jerez
// VC: 6.0
// 
//********************************************************************



#include <toolbox_stdafx.h>
#include "HiResTimer.h"
#include "OpenGlUtil.h"

extern int keyBoardTraceMode;

#define LOCK_FRAME_RATE
const dFloat TICKS2SEC = 1.0e-6f;

CHiResTimer::CHiResTimer() 
{
	m_prevTime = 0;
}

CHiResTimer::~CHiResTimer() 
{
}



dFloat CHiResTimer::GetElapsedSeconds()
{
	dFloat timeStep;
	unsigned miliseconds;
	
//	miliseconds = glutGet (GLUT_ELAPSED_TIME);
	miliseconds = GetTimeInMicrosenconds();

	// optimal keep the fps below 70 fps
	#ifdef LOCK_FRAME_RATE
	while ((miliseconds - m_prevTime) < 16500) {
		miliseconds = GetTimeInMicrosenconds();
	}
	#endif

	timeStep = dFloat (miliseconds - m_prevTime) * TICKS2SEC;
	m_prevTime = miliseconds;
	
	if (timeStep > 1.0f) {
		timeStep = 1.0f;
	}
//	if (timeStep < 0.005f) {
//		timeStep = 0.005f;
//	}


if (keyBoardTraceMode)
{
#ifdef RECORD_LOG
//	Sleep (100);
	extern FILE * file;
	fwrite (&timeStep, sizeof (float), 1, file);
	fflush (file);
#endif

#ifdef READ_LOG
	extern FILE * file;
	//Sleep (150);
	fread (&timeStep, sizeof (float), 1, file);
#endif
}

	
	return timeStep;
} 



unsigned CHiResTimer::GetTimeInMicrosenconds()
{
#ifdef _MSC_VER
	
	LARGE_INTEGER count;
	static bool firstTime = true;
	static bool haveTimer = false;
	static LARGE_INTEGER frequency;
	static LARGE_INTEGER baseCount;

	if (firstTime) {
		firstTime = false;
		haveTimer = QueryPerformanceFrequency(&frequency) ? true : false;
		QueryPerformanceCounter (&baseCount);
	} 

	if (haveTimer) {
		unsigned ticks;
		QueryPerformanceCounter (&count);
		count.QuadPart -= baseCount.QuadPart;
		ticks = unsigned (count.QuadPart * LONGLONG (1000000) / frequency.QuadPart);
		return ticks;
	} else {
		return glutGet (GLUT_ELAPSED_TIME) * 1000;
	}
#else
	return glutGet (GLUT_ELAPSED_TIME) * 1000;
#endif

}




