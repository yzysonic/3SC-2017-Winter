#include "Time.h"
#include "Window.h"

#pragma comment (lib, "winmm.lib")

struct Timer
{
	DWORD startTime = 0;
	DWORD interval = 0;
};

void SetTimerInterval(Timer* _this, float s);
void ResetTimer(Timer* _this);
float ElapsedTime(Timer* _this);
bool TimeUp(Timer* _this);

Timer	g_pFrameTimer;
Timer	g_pFPSTimer;
DWORD	g_FrameError;
DWORD	g_FrameCount;
float	g_DeltaTime;
int		g_fps;

void SetTimerInterval(Timer* _this, float interval)
{
	_this->interval = (DWORD)(interval * 1000);
}

void ResetTimer(Timer* _this)
{
	_this->startTime = timeGetTime();
}

float ElapsedTime(Timer* _this)
{
	return (timeGetTime() - _this->startTime) / 1000.0f;
}

bool TimeUp(Timer* _this)
{
	if ((timeGetTime() - _this->startTime) >= _this->interval)
		return true;
	else
		return false;
}


void InitTime()
{
	timeBeginPeriod(1); // •ª‰ð”\‚ðÝ’è
	SetTimerInterval(&g_pFrameTimer, 1.0f/SystemParameters::FPS);
	SetTimerInterval(&g_pFPSTimer, 1.0f);
	g_FrameError = 0;
	g_FrameCount = 0;
}

float DeltaTime()
{
	return g_DeltaTime;
}

int FPS()
{
	return g_fps;
}


void FramerateControl()
{
	static DWORD elapsed, sleep, _elapsed, _error;

	// fpsŒv‘ª
	g_FrameCount++;
	elapsed = timeGetTime() - g_pFPSTimer.startTime;
	//if (elapsed >= g_pFPSTimer->interval)
	if (g_FrameCount == 30)
	{
		//g_fps = g_FrameCount / (g_pFPSTimer->interval / 1000.0f);
		g_fps = (int)(g_FrameCount / ElapsedTime(&g_pFPSTimer));
		g_FrameCount = 0;
		ResetTimer(&g_pFPSTimer);
	}

	// fps‡‚í‚¹
	elapsed = timeGetTime() - g_pFrameTimer.startTime;
	//sleep = g_pFrameTimer->interval - min(elapsed + g_FrameError, g_pFrameTimer->interval);
	sleep = g_pFrameTimer.interval - min(elapsed, g_pFrameTimer.interval);
	if (sleep > 0)
	{
		Sleep(sleep);
		//_elapsed = elapsed;
		elapsed = (timeGetTime() - g_pFrameTimer.startTime);
		//g_FrameError = max(elapsed - g_pFrameTimer->interval, 0);
	}
	//else
	//	g_FrameError = 0;
	
	//g_FrameError = max(elapsed - g_pFrameTimer->interval, 0);
	g_DeltaTime = min(elapsed / 1000.0f, (1.0f / SystemParameters::FPS)*3);

	ResetTimer(&g_pFrameTimer);

#ifdef _DEBUG
	char s[256];
	sprintf_s(s, "%s FPS:%d", SystemParameters::windowTitle, g_fps);
	SetWindowText(GetHWnd(), s);
	//sprintf_s(s, "%d\n", elapsed);
	//OutputDebugString(s);
#endif
}
