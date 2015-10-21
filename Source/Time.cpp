#include "Time.hpp"

double  Time::_perfCounterSeconds = 0.0;
__int64 Time::_startTime          = 0;
__int64 Time::_currTime           = 0;
__int64 Time::_lastTime           = 0;
float   Time::_totalTime          = 0.0f;
float   Time::_elapsedTime        = 0.0f;

// Starts keeping track of time
void Time::Start()
{
    // Query the performance counter for accurate timing information
    __int64 frequency;
    QueryPerformanceFrequency( reinterpret_cast<LARGE_INTEGER*>( &frequency ) );
    _perfCounterSeconds = 1.0 / frequency;

    // Grab the start time
    __int64 now;
    QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>( &now ) );
    _startTime = now;
    _currTime = now;
    _lastTime = now;

    // Zero out everything else
    _totalTime = 0.0f;
    _elapsedTime = 0.0f;
}

// Updates time values
void Time::Update()
{
    // Grab the current time
    __int64 now;
    QueryPerformanceCounter( (LARGE_INTEGER*)&now );
    _lastTime = _currTime;
    _currTime = now;

    // Calculate delta time and clamp to zero if negative.
    //  - This could happen if the processor goes into a
    //    power save mode or the process itself gets moved
    //    to another processor core.
    _elapsedTime = static_cast<float>( ( _currTime - _lastTime ) * _perfCounterSeconds );
    if ( _elapsedTime < 0.0f )
    {
        _elapsedTime = 0.0f;
    }

    // Calculate total time
    _totalTime = static_cast<float>( ( _currTime - _startTime ) * _perfCounterSeconds );
}

// Get the total tracked time
float Time::GetTotalTime()
{
    return _totalTime;
}

// Get the elapsed time since last frame
float Time::GetElapsedTime()
{
    return _elapsedTime;
}
