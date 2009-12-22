/*****************************************************************************/
/*                                                                           */
/*  Visualization Library                                                    */
/*  http://www.visualizationlibrary.com                                      */
/*                                                                           */
/*  Copyright 2005-2009 Michele Bosi                                         */
/*                                                                           */
/*  This file is part of Visualization Library.                              */
/*  It may only be used, modified, and distributed under the terms of the    */
/*  license specified in the LICENSE.TXT file. By continuing to use,         */
/*  modify or distribute this file you indicate that you have read the       */
/*  license and understand and accept it fully.                              */
/*                                                                           */
/*  Visualization Library is distributed in the hope that it will be useful, */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/*  LICENSE.TXT file for more details.                                       */
/*                                                                           */
/*****************************************************************************/

#include "Time.hpp"
#include <ctime>
#include <cmath>

#include <windows.h>

using namespace vl;

double Time::mStartTime = init_start_time();

double Time::init_start_time()
{
	#if defined(_WIN32)
	// Win32
	LARGE_INTEGER Frequency;
	LARGE_INTEGER PerformanceCount;
	BOOL has_timer = QueryPerformanceFrequency( &Frequency );
	if (has_timer)
	{
		QueryPerformanceCounter( &PerformanceCount );
		return (double)PerformanceCount.QuadPart/Frequency.QuadPart;
	}
	else
	{
		return GetTickCount() / 1000.0;
	}
	#endif
}

//-----------------------------------------------------------------------------
// Time
//-----------------------------------------------------------------------------
Time::Time()
{
	mStart = -1;
	#if defined(_WIN32)
	SYSTEMTIME local_time;
	GetLocalTime(&local_time);
	mYear = local_time.wYear;
	mMonth = local_time.wMonth;
	mDayOfWeek = local_time.wDayOfWeek;
	mDayOfMonth = local_time.wDay;
	mHour = local_time.wHour;
	mMinute = local_time.wMinute;
	mSecond = local_time.wSecond;
	mMicrosecond = local_time.wMilliseconds * 1000;
	#endif
}
//-----------------------------------------------------------------------------
//! Seconds passed from an arbitrary origin
//! @todo Support Linux/X11
//! QueryPerformanceFrequency should be called only once in the application lifetime
vl::Real Time::timerSeconds()
{
	#if defined(_WIN32)
    // Win32
    LARGE_INTEGER Frequency;
    LARGE_INTEGER PerformanceCount;
    BOOL has_timer = QueryPerformanceFrequency( &Frequency );
    if (has_timer)
    {
      QueryPerformanceCounter( &PerformanceCount );
      return (vl::Real)((double)PerformanceCount.QuadPart/Frequency.QuadPart  - mStartTime);
    }
    else
    {
      return (vl::Real)(GetTickCount() / 1000.0 - mStartTime);
    }
	#endif
}
//-----------------------------------------------------------------------------
