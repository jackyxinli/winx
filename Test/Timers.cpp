/*****************************************************************************************

***		Timers.cpp : Implementation of the CTimerCycles and CTimerSecs classes. 

***		Developed by Langfine Ltd. 
***		Released to the public domain 12/Nov/2001.
***		Please visit our website www.langfine.com

***		Any modifications must be clearly commented to distinguish them from Langfine's 
***		original source code. Please advise Langfine of useful modifications so that we 
***		can make them generally available. 

*****************************************************************************************/

#include "stdafx.h"
#include "Timers.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*****************************************************************************************
Acknowlegement: Thanks to Gabriel Fleseriu for his article 
at http://www.codeguru.com/misc/exact_timer.html. CTimerCycles 
was based on information obtained from Gabriel's article. 
*****************************************************************************************/


// global data required for _asm instructions in CTimerCycles
void* pnStart; 
void* pnStop;


/*****************************************************************************************
CONSTRUCTOR:	CTimerCycles
DESCRIPTION:	Initialises member data
ARGUMENTS:		None
NOTES:			None
*****************************************************************************************/
CTimerCycles::CTimerCycles()
{
	// initialise member data
	m_nStart = 0;
	m_nStop = 0;

	// assembler code requires addresses held in global variables
	pnStart = &m_nStart;  
	pnStop = &m_nStop;
}


/*****************************************************************************************
FUNCTION:		Start()
DETAILS:		public
DESCRIPTION:	Obtains the current 64bit real time stamp counter and stores it. 
RETURNS:		none
ARGUMENTS:		none
NOTES:			'cpuid' is used to ensure serialised operation.
				'rdtsc' reads the real time stamp counter.
				The value obtained by rdtsc is loaded into 'm_nStart' (via 'pnStart').
*****************************************************************************************/
void CTimerCycles::Start()
{
	{_asm mov eax, 0				\
	_asm cpuid						\
	_asm rdtsc						\
	_asm mov ebx, pnStart			\
	_asm mov dword ptr [ebx], eax	\
	_asm mov dword ptr [ebx+4], edx	\
	_asm mov eax, 0					\
	_asm cpuid}
}


/*****************************************************************************************
FUNCTION:		Stop()
DETAILS:		public
DESCRIPTION:	Obtains the current 64bit real time stamp counter and calculates elapsed 
				cycles since the previous call of Start(). 
RETURNS:		If the function succeeds, returns the number of elapsed cycles since the 
				last call of Start().
				If the function fails, returns -1. 
ARGUMENTS:		none
NOTES:			'cpuid' is used to ensure serialised operation.
				'rdtsc' reads the real time stamp counter.
				The value obtained by rdtsc is loaded into 'm_nStop' (via 'pnStop').
				The number of cycles that have passed since Start() was last called is 
				then calcualted. 
*****************************************************************************************/
double CTimerCycles::Stop()
{
	{_asm mov eax, 0					  \
	_asm cpuid							  \
	_asm rdtsc							  \
	_asm mov ebx, pnStop				  \
	_asm mov dword ptr [ebx], eax		  \
	_asm mov dword ptr [ebx+4], edx	      \
	_asm mov eax, 0					      \
	_asm cpuid}

	double dCycles = -1;

	// perform a simple sanity check
	if ( m_nStart >= m_nStop )
	{
		// report an error if the sanity check failed
		AfxMessageBox("Timing Error - fault with real time stamp counter");
	}
	else
	{
		// calculate the number of elapsed cycles
		dCycles = (double)(m_nStop-m_nStart);
	}

	// return the number of cycles
	return dCycles;
};



/*****************************************************************************************
CONSTRUCTOR:	CTimerSecs
DESCRIPTION:	Initialises member data
ARGUMENTS:		None
NOTES:			Does not report an error if QueryPerformanceFrequency() fails
*****************************************************************************************/
CTimerSecs::CTimerSecs()
{	
	// initialise member data
	m_nFrequency.QuadPart = 0;
	m_nStart.QuadPart = 0;

	// retrieve the frequency of the high-resolution performance counter
	QueryPerformanceFrequency( &m_nFrequency );
}


/*****************************************************************************************
FUNCTION:		Start()
DETAILS:		public
DESCRIPTION:	Obtains the current value of the high resolution performance counter
RETURNS:		none
ARGUMENTS:		none
NOTES:			Does not report an error if QueryPerformanceCounter() fails
*****************************************************************************************/
void CTimerSecs::Start()
{
	// get the current value of the performance counter
    QueryPerformanceCounter( &m_nStart );
}


/*****************************************************************************************
FUNCTION:		Stop()
DETAILS:		public
DESCRIPTION:	Obtains the current value of the high resolution performance counter 
				and calculates elapsed time since the Start() function was called. 
RETURNS:		If the function succeeds, returns the number of seconds elapsed since the 
				last call of Start().
				If the function fails, returns -1
				(The function will fail if there is no high resolution performance counter
				available.) 
ARGUMENTS:		none
NOTES:			Reports an error if QueryPerformanceCounter() fails.
*****************************************************************************************/
double CTimerSecs::Stop()
{
	double dElapsed = 0;

    // get the current value of the performance counter
    LARGE_INTEGER nStop;
	nStop.QuadPart = 0;
    if ( QueryPerformanceCounter( &nStop ) )
	{
		// if a high-res counter was available calculate the elapsed time
		dElapsed = ( nStop.QuadPart - m_nStart.QuadPart ) / (double)m_nFrequency.QuadPart;
	}
	else
	{
		// display an error message if no high-res counter was available
		AfxMessageBox("Timing Error - no high resolution performance counter available");
		dElapsed = -1;
	}

	// return the elapsed counts
	return dElapsed;
}


