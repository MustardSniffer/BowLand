#pragma once

#include "Component.hpp"
#include "TweenTarget.hpp"
#include "TweenValue.hpp"
#include <vector>

/// <summary>
/// An enumeration of possible tween methods.
/// </summary>
enum class TweenMethod
{
    Linear,
    QuadraticEaseOut,
    QuadraticEaseIn,
    QuadraticEaseInOut,
    QuadraticEaseOutIn,
    ExponentialEaseOut,
    ExponentialEaseIn,
    ExponentialEaseInOut,
    ExponentialEaseOutIn,
    CubicEaseOut,
    CubicEaseIn,
    CubicEaseInOut,
    CubicEaseOutIn,
    QuarticEaseOut,
    QuarticEaseIn,
    QuarticEaseInOut,
    QuarticEaseOutIn,
    QuinticEaseOut,
    QuinticEaseIn,
    QuinticEaseInOut,
    QuinticEaseOutIn,
    CircularEaseOut,
    CircularEaseIn,
    CircularEaseInOut,
    CircularEaseOutIn,
    SineEaseOut,
    SineEaseIn,
    SineEaseInOut,
    SineEaseOutIn,
    ElasticEaseOut,
    ElasticEaseIn,
    ElasticEaseInOut,
    ElasticEaseOutIn,
    /*
    BounceEaseOut,
    BounceEaseIn,
    BounceEaseInOut,
    BounceEaseOutIn,
    */
    BackEaseOut,
    BackEaseIn,
    BackEaseInOut,
    BackEaseOutIn,

    TweenMethodCount
};

/// <summary>
/// An enumeration of possible tween play modes.
/// </summary>
enum class TweenPlayMode
{
    None,
    Once,
    Loop,
    PingPong
};

/// <summary>
/// Defines the base for tween components.
/// </summary>
class Tweener : public Component
{
    /// <summary>
    /// The function signature of tween functions.
    /// </summary>
    /// <param name="start">The starting value.</param>
    /// <param name="end">The ending value.</param>
    /// <param name="time">The current time in the animation in the range [0, duration].</param>
    /// <param name="duration">The duration of the animation.</param>
    typedef float( *TweenFunction )( float start, float end, float time, float duration );

    // All of the tween functions
    static std::vector<TweenFunction> _tweenFunctions;

    TweenFunction _tweenFunction;
    TweenMethod   _tweenMethod;
    TweenPlayMode _playMode;
    float _startTime;
    float _duration;

protected:
    TweenTarget _targetValue;
    TweenValue _startValue;
    TweenValue _endValue;

    /// <summary>
    /// Creates all of the tween functions.
    /// </summary>
    static void CreateTweenFunctions();

    /// <summary>
    /// Checks to see if the target, start, and end values are compatible.
    /// </summary>
    bool AreValuesCompatible() const;

    /// <summary>
    /// Checks to see if the start, end, and target values are compatible.
    /// </summary>
    void CheckForCompatibleValues();

    /// <summary>
    /// Handles when this tweener is done.
    /// </summary>
    virtual void OnEnd();

public:
    /// <summary>
    /// Creates a new tweener.
    /// </summary>
    /// <param name="gameObject">The game object this tween component would belong to.</param>
    Tweener( GameObject* gameObject );

    /// <summary>
    /// Destroys this tweener.
    /// </summary>
    virtual ~Tweener();

    /// <summary>
    /// Gets this tween animation's duration.
    /// </summary>
    float GetDuration() const;

    /// <summary>
    /// Gets this tweener's tween method.
    /// </summary>
    TweenMethod GetTweenMethod() const;

    /// <summary>
    /// Gets this tweener's play mode.
    /// </summary>
    TweenPlayMode GetPlayMode() const;

    /// <summary>
    /// Plays this tweener's animation.
    /// </summary>
    void Play();

    /// <summary>
    /// Sets this tweener's duration.
    /// </summary>
    /// <param name="duration">The new duration.</param>
    void SetDuration( float duration );

    /// <summary>
    /// Sets this tweener's play mode.
    /// </summary>
    /// <param name="mode">The new tween mode.</param>
    void SetPlayMode( TweenPlayMode mode );

    /// <summary>
    /// Sets this tweener's tween method.
    /// </summary>
    /// <param name="method">The new tween method.</param>
    void SetTweenMethod( TweenMethod method );

    /// <summary>
    /// Updates this tweening component.
    /// </summary>
    virtual void Update() override;
};
