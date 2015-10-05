#include "GameTime.hpp"

// Create a new game time object
GameTime::GameTime()
{
    Reset();
}

// Destroys this game time object
GameTime::~GameTime()
{
    _perfCounterSeconds = 0.0;
    _startTime = 0;
    _currTime = 0;
    _lastTime = 0;
    _totalTime = 0.0f;
    _elapsedTime = 0.0f;
}

// Reset time values
void GameTime::Reset()
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

// Update time values
void GameTime::Update()
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
