#include "Timer.hpp"

// Create a new timer
Timer::Timer()
{
    ZeroMemory( this, sizeof( Timer ) );

    // Query the performance counter for accurate timing information
    __int64 frequency;
    QueryPerformanceFrequency( reinterpret_cast<LARGE_INTEGER*>( &frequency ) );
    _perfCounterSeconds = 1.0 / frequency;
}

// Destroy the timer
Timer::~Timer()
{
    ZeroMemory( this, sizeof( Timer ) );
}

// Get the elapsed time
float Timer::GetElapsedTime() const
{
    __int64 delta = 0;

    // If we haven't been stopped
    if ( _endTime == 0 )
    {
        // Get the elapsed time until now
        __int64 now = 0;
        QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>( &now ) );
        delta = now - _startTime;
    }
    // Otherwise get the actual delta time
    else
    {
        delta = _endTime - _startTime;
    }

    return static_cast<float>( delta * _perfCounterSeconds );
}

// Start the timer
void Timer::Start()
{
    // Grab the start time
    QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>( &_startTime ) );
    _endTime   = 0;
}

// Stop the timer
void Timer::Stop()
{
    // Grab the end time
    QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>( &_endTime ) );
}
