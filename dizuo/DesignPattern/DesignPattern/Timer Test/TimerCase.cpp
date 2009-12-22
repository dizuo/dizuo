#include	<iostream>
#include	<iomanip>
#include	<string>
#include	"Time.hpp"

#include "timer.h"
#include "HiResTimer.h"

template<class Event>
class TimerProgram
{
public:
	TimerProgram()
		: mMaxTime(0.0)
	{}
	~TimerProgram(){}
	
	void renderEvent()
	{
		eEventCase.render();

		if (mMaxTime != 0)
		{
			if ( !timer.isStarted() )
				timer.start();
			else
				if (timer.elapsed() > mMaxTime)
					eEventCase.quit();
		}
	}

	void setMaxTime(vl::Real t) { mMaxTime = t; }

protected:
	vl::Real mMaxTime;
	vl::Time timer;
	Event eEventCase;
};

int main()
{
	using namespace std;
 	using namespace vl;
 	Time timer1;
 	timer1.start();
 	vl::Real mMaxTime = 10;
 
 	cout << timer1.year() << " / " << timer1.month() << " / " << timer1.dayOfMonth() << " / " << endl;
 	cout << Time::timerSeconds() << endl;
 	cout << timer1.microsecond() << endl;

	CTimer timer2;
	timer2.Initialize();
	int i=0;
	while(i < 5)
	{
		std::cout << "Time2 pass : " << timer2.GetTimeMSec() << endl;

		i++;
		timer2.Update();
	}
	
	CHiResTimer timer3;
	timer3.Init();

  	while (1)
 	{
 		std::cout << "\rTime pass : " << timer1.elapsed() << "\t" << 10000*timer3.GetElapsedSeconds();
 		if( timer1.elapsed() > mMaxTime )
 			exit(1);
 	}

	system("PAUSE");
	return 0;
}