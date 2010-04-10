#ifndef _D_RUNTINE_PROFILER_164045623__
#define _D_RUNTINE_PROFILER_164045623__

struct NewtonWorld;

#define MAX_FRAMES 62
#define MAX_FRAMES_STEP 1
#define MAX_TRACKS 10

#define CHART_HIEGHT 120


class dRuntimeProfiler
{
	public:
	dRuntimeProfiler(int thread, int origin_x, int origin_y);
	~dRuntimeProfiler(void);

	void Render (NewtonWorld* nWorld, int mask);
	void DrawLabel (dFloat x, dFloat y, const char* label);
	void DrawTrack (dFloat x, dFloat y, const dVector& color, int start, unsigned* track);

	int m_thread;
	int m_oringin_x;
	int m_oringin_y;
	int m_frameIndex;
	unsigned m_perfomanceTracks[MAX_TRACKS][MAX_FRAMES];
};


#endif