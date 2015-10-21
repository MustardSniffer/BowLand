#pragma once

#include "DirectX.hpp"

/// <summary>
/// Defines the static class for getting time information.
/// </summary>
class Time
{
    static double  _perfCounterSeconds;
    static __int64 _startTime;
    static __int64 _currTime;
    static __int64 _lastTime;
    static float   _totalTime;
    static float   _elapsedTime;

    /// <summary>
    /// Starts keeping track of time.
    /// </summary>
    static void Start();

    /// <summary>
    /// Updates timing values.
    /// </summary>
    static void Update();

    friend class DirectXGameCore;
    friend class MyDemoGame;

    // Hide instance constructors + destructor
    Time() = delete;
    Time( const Time& ) = delete;
    Time( Time&& ) = delete;
    ~Time();

    // Hide assignment operators
    Time& operator=( const Time& ) = delete;
    Time& operator=( Time&& ) = delete;

public:
    /// <summary>
    /// Gets the amount of time that has passed since the game started.
    /// </summary>
    static float GetTotalTime();

    /// <summary>
    /// Gets the amount of time that has passed since the last frame.
    /// </summary>
    static float GetElapsedTime();
};