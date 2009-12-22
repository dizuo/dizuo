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

#ifndef Time_INCLUDE_ONCE
#define Time_INCLUDE_ONCE

namespace vl
{
	typedef double Real;
  //-----------------------------------------------------------------------------
  // timerSeconds()
  // Under Windows, due to BIOS or HAL bugs this function could return different
  // results on different threads, which means that if more than one thread needs
  // to sinchronize with the others using this function they should use the time
  // returned in one of those threads as reference.
  //-----------------------------------------------------------------------------
  /**
   * Simple class to be used as a timer and to retrieve the current time and date.
  */
  class Time
  {
  public:
    Time();
	~Time(){};

    int year() const { return mYear; }
    int month() const { return mMonth; }
    int dayOfWeek() const { return mDayOfWeek; }
    int dayOfMonth() const { return mDayOfMonth; }
    int hour() const { return mHour; }
    int minute() const { return mMinute; }
    int second() const { return mSecond; }
    int microsecond() const { return mMicrosecond; }

    static vl::Real timerSeconds();

    void start() { mStart = timerSeconds(); }
    void stop()  { mStart = -1.0; }
    bool isStarted() const { return mStart != -1; }

	vl::Real getFps(unsigned long elapsedFrames = 1);
    vl::Real elapsed() const { return mStart >= 0 ? timerSeconds() - mStart : -1; }

  protected:
    int mYear; // 1601 through 30827.
    int mMonth; // 1..12
    int mDayOfWeek; // 0 = Sunday, 6 = Saturday
    int mDayOfMonth; // 1..31
    int mHour; // 0..23
    int mMinute; // 0..59
    int mSecond; // 0..59
    int mMicrosecond; // 0 ... 999999

    vl::Real mStart;
  private:
	static vl::Real mStartTime; 
	static double init_start_time();
  };
}

#endif
