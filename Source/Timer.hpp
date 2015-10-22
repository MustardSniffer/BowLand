#pragma once

#include "DirectX.hpp"

/// <summary>
/// Defines a timer.
/// </summary>
class Timer
{
    double  _perfCounterSeconds;
    __int64 _startTime;
    __int64 _endTime;

public:
    /// <summary>
    /// Creates a new timer.
    /// </summary>
    Timer();

    /// <summary>
    /// Destroys this timer.
    /// </summary>
    ~Timer();

    /// <summary>
    /// Gets the amount of time that has passed since the timer was started.
    /// </summary>
    float GetElapsedTime() const;

    /// <summary>
    /// Starts the timer.
    /// </summary>
    void Start();

    /// <summary>
    /// Stops the timer.
    /// </summary>
    void Stop();
};