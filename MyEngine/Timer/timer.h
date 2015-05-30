
//---------------------------------------------------------------------------
#ifndef TIMER_H
#define TIMER_H
//---------------------------------------------------------------------------
#include "../myengine_api.h"
#include <windows.h>
//---------------------------------------------------------------------------
namespace Inaba
{
//---------------------------------------------------------------------------
class MYENGINE_API Timer
{
// constructor
public:
	Timer ();


// measure
public:
	void firstMeasure ();
	void measure ();

	float timeBetweenFrames () const;
	unsigned int fps () const;

private:
	double m_dTimeBetweenFrames;
	double m_dMeasureFpsSample;
	unsigned int m_uiFPS;
	unsigned int m_uiFrameCounter;

	LARGE_INTEGER m_kPerfCount1;
	LARGE_INTEGER m_kPerfCount2;
	LARGE_INTEGER m_kFrequency;
};
//---------------------------------------------------------------------------
#include "timer.inl"
//---------------------------------------------------------------------------
} // end namespace
//---------------------------------------------------------------------------
#endif // TIMER_H
//---------------------------------------------------------------------------
