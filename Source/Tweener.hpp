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
    BounceEaseOut,
    BounceEaseIn,
    BounceEaseInOut,
    BounceEaseOutIn,
    BackEaseOut,
    BackEaseIn,
    BackEaseInOut,
    BackEaseOutIn,

    TweenMethodCount
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

    // TODO - Create TweenTarget class to support float, float2, float3, etc.
    TweenTarget _targetValue;
    TweenMethod _tweenMethod;
    TweenFunction _tweenFunction;
    TweenValue _startValue;
    TweenValue _endValue;
    float _duration;
    float _startTime;
    bool _areValuesCompatible;
    bool _isAffectingTransform;

    /// <summary>
    /// Creates all of the tween functions.
    /// </summary>
    static void CreateTweenFunctions();

    /// <summary>
    /// Checks to see if the start, end, and target values are compatible.
    /// </summary>
    void CheckForCompatibleValues();

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
    /// Gets this tween animation's starting value.
    /// </summary>
    inline TweenValue GetStartValue()
    {
        return _startValue;
    }

    /// <summary>
    /// Gets this tween animation's ending value.
    /// </summary>
    inline TweenValue GetEndValue()
    {
        return _endValue;
    }

    /// <summary>
    /// Gets this tween animation's duration.
    /// </summary>
    inline float GetDuration()
    {
        return _duration;
    }

    /// <summary>
    /// Gets this tweener's tween method.
    /// </summary>
    inline TweenMethod GetTweenMethod() const
    {
        return _tweenMethod;
    }

    /// <summary>
    /// Sets this tweener's starting value.
    /// </summary>
    /// <param name="value">The new start value.</param>
    void SetStartValue( TweenValue value );

    /// <summary>
    /// Sets this tweener's ending value.
    /// </summary>
    /// <param name="value">The new end value.</param>
    void SetEndValue( TweenValue value );

    /// <summary>
    /// Sets this tweener's duration.
    /// </summary>
    /// <param name="duration">The new duration.</param>
    void SetDuration( float duration );

    /// <summary>
    /// Sets this tweener's tween method.
    /// </summary>
    /// <param name="method">The new tween method.</param>
    void SetTweenMethod( TweenMethod method );

    /// <summary>
    /// Starts this tweener.
    /// </summary>
    /// <param name="gameTime">Provides a snapshot of timing values.</param>
    /// <param name="target">The target value to modify.</param>
    /// <param name="targetIsTransform">True if the target belongs to the transform of the owner game object, false if not.</param>
    void Start( const GameTime& gameTime, TweenTarget target, bool targetIsTransform = false );

    /// <summary>
    /// Updates this tweening component.
    /// </summary>
    /// <param name="gameTime">Provides a snapshot of timing values.</param>
    void Update( const GameTime& gameTime ) override;
};
