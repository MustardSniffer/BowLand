#pragma once

#include "DirectX.hpp"

// TODO - Remove this class

/// <summary>
/// Defines a way to keep track of time.
/// </summary>
class GameTime
{
    double _perfCounterSeconds;
    __int64 _startTime;
    __int64 _currTime;
    __int64 _lastTime;
    float _totalTime;
    float _elapsedTime;

public:
    /// <summary>
    /// Creates a new game time object.
    /// </summary>
    GameTime();

    /// <summary>
    /// Destroys this game time object.
    /// </summary>
    ~GameTime();

    /// <summary>
    /// Gets the total amount of time that has passed since this started keeping track of time.
    /// </summary>
    inline float GetTotalTime() const
    {
        return _totalTime;
    }

    /// <summary>
    /// Gets the elapsed amount of time that has passed between updates.
    /// </summary>
    inline float GetElapsedTime() const
    {
        return _elapsedTime;
    }

    /// <summary>
    /// Resets this game time object.
    /// </summary>
    void Reset();

    /// <summary>
    /// Updates this game time object.
    /// </summary>
    void Update();
};