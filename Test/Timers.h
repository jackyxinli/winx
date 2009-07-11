/*****************************************************************************************

***		Timer.h: interface for the CTimerCycles and CTimerSecs classes. 

***		Developed by Langfine Ltd. 
***		Released to the public domain 12/Nov/2001.
***		Please visit our website www.langfine.com

***		Any modifications must be clearly commented to distinguish them from Langfine's 
***		original source code. Please advise Langfine of useful modifications so that we 
***		can make them generally available. 

*****************************************************************************************/


#if !defined(AFX_TIMER_H__E44E6F98_1663_45EB_9B40_756182AA8C23__INCLUDED_)
#define AFX_TIMER_H__E44E6F98_1663_45EB_9B40_756182AA8C23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/****************************************************************************************
							**** NOTE TO THE PROGRAMMER ****

The Windows NT (and higher) operating system is preemptive and mulitasking. This implies
that any process may be interrupted by the operating system in order to allocate time
slices to other processes. This complicates the task of timing the execution of a 
particular sequence of code and should be born in mind when using these timer classes.
To minimise inaccuracies produced by process interruption, the code under test could 
have its priority boosted temporarily. This should ensure it is interrupted less. When 
comparing application performance, try to compare like with like; run the applications 
in a 'clean' environment (reboot your pc, close down any unnecessary programs or services).
To gain more accurate timing information, run each test as many times as possible and 
calculate average times. Ensure that the code under test does not attempt to access files 
stored on the hard disc; the location of the file on disc and factors such as file 
fragmentation will greatly effect performance.
*****************************************************************************************/


/*****************************************************************************************
CLASS:			CTimerCycles
DESCRIPTION:	Calculates elapsed cycles by use of the real time stamp counter.
				Uses the assembly code instruction 'rdtsc' to read the real time stamp 
				counter when Start() and Stop() functions are called. The difference in
				the two counter readings gives the number of cycles elapsed between the
				calls to Start() and Stop(). 
NOTES:			Acknowlegement: Thanks to Gabriel Fleseriu for his article 
				at http://www.codeguru.com/misc/exact_timer.html. This class 
				was based on information obtained from Gabriel's article. 
				NB. The rdtsc instruction is only available with Pentium IA-family 
				processors - this class cannot be used on processors that do not support 
				the rdtsc instruction. If used on an incompatible processor, GP faults or
				exceptions may occur. 
*****************************************************************************************/
class CTimerCycles
{
public:
	CTimerCycles();		// constructor
	void Start();		// start the timer
	double Stop();		// stop the timer and return 

private:
	__int64 m_nStart;	// cycles count retrieved by Start()
	__int64 m_nStop;	// cycles count retrieved by End()
};



/*****************************************************************************************
CLASS:			CTimerSecs
DESCRIPTION:	Calculates elapsed time by use of the Windows high resolution performance
				counter. The counter is read when Start() and Stop() functions are called.
				The frequency of the counter is read by the constructor. The elapsed time
				is obtained by calculating the difference between the two counter 
				readings and then dividing this value by the counter frequency.
NOTES:			Acknowlegement: Thanks to Rick York for his article at 
				http://codeguru.earthweb.com/misc/HighResolutionTimer.shtml which 
				provided useful information. 
				This class uses the Win32 API functions QueryPerformanceFrequency and 
				QueryPerformanceCounter. Not all CPUs support these functions. All Intel 
				Pentiums do support it. The class will display an error message if used 
				on a CPU that does not support it. 
*****************************************************************************************/
class CTimerSecs
{
  public :
	CTimerSecs();    // constructor
    void Start();    // start the timer
    double Stop();   // stop the timer and return elapsed time (secs)

  private :
	LARGE_INTEGER m_nFrequency;	// cycles per second of the performance counter
	LARGE_INTEGER m_nStart;		// the value of the counter retrieved by Start()
};


#endif // !defined(AFX_TIMER_H__E44E6F98_1663_45EB_9B40_756182AA8C23__INCLUDED_)
