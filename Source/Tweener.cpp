#include "Tweener.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

#define M_FPI M_PI##f

std::vector<Tweener::TweenFunction> Tweener::_tweenFunctions;

// Create a new tweener
Tweener::Tweener( GameObject* gameObject )
    : Component( gameObject )
    , _targetValue( nullptr )
    , _tweenMethod( TweenMethod::Linear )
    , _tweenFunction( nullptr )
    , _startValue( 0.0f )
    , _endingValue( 1.0f )
    , _duration( 1.0f )
    , _startTime( 0.0f )
{
    _isDrawable = false;
    _isEnabled = false;
    _usesLateUpdate = false;

    // Create the tween functions if they haven't been yet
    if ( _tweenFunctions.size() == 0 )
    {
        CreateTweenFunctions();
    }

    // Set the tween method and function in one go
    SetTweenMethod( TweenMethod::Linear );
}

// Destroy this tweener
Tweener::~Tweener()
{
    _targetValue   = nullptr;
    _tweenMethod   = TweenMethod::Linear; // 0
    _tweenFunction = nullptr;
    _startValue    = 0.0f;
    _endingValue   = 0.0f;
    _duration      = 0.0f;
    _startTime     = 0.0f;
}

/// <summary>
/// Registers the given tween method and the provided function.
/// </summary>
/// <param name="method">The tween method.</param>
/// <param name="function">The tween function. Parameters provided: start, end, time, duration.</param>
#define RegisterTweenFunction(method, function) \
    _tweenFunctions[static_cast<int>( method )] = [](float start, float end, float time, float duration) -> float function ;

// Creates all of the tween functions
void Tweener::CreateTweenFunctions()
{
    _tweenFunctions.resize( static_cast<int>( TweenMethod::TweenMethodCount ) );

    // All tween code is based off of Robert Penner's easing functions http://robertpenner.com/easing/

    RegisterTweenFunction( TweenMethod::Linear,
    {
        return ( end - start ) * time / duration + start;
    } );
    RegisterTweenFunction( TweenMethod::QuadraticEaseOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::QuadraticEaseIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::QuadraticEaseInOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::QuadraticEaseOutIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::ExponentialEaseOut,
    {
        return ( time == duration )
               ? end
               : ( end - start ) * ( -powf( 2.0f, -10.0f * time / duration ) + 1.0f ) + start;
    } );
    RegisterTweenFunction( TweenMethod::ExponentialEaseIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::ExponentialEaseInOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::ExponentialEaseOutIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::CubicEaseOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::CubicEaseIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::CubicEaseInOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::CubicEaseOutIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::QuarticEaseOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::QuarticEaseIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::QuarticEaseInOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::QuarticEaseOutIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::QuinticEaseOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::QuinticEaseIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::QuinticEaseInOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::QuinticEaseOutIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::CircularEaseOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::CircularEaseIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::CircularEaseInOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::CircularEaseOutIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::SineEaseOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::SineEaseIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::SineEaseInOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::SineEaseOutIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::ElasticEaseOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::ElasticEaseIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::ElasticEaseInOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::ElasticEaseOutIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::BounceEaseOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::BounceEaseIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::BounceEaseInOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::BounceEaseOutIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::BackEaseOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::BackEaseIn,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::BackEaseInOut,
    {
        return 0.0f;
    } );
    RegisterTweenFunction( TweenMethod::BackEaseOutIn,
    {
        return 0.0f;
    } );
}

// Set start value
void Tweener::SetStartValue( float value )
{
    _startValue = value;
}

// Set ending value
void Tweener::SetEndValue( float value )
{
    _endingValue = value;
}

// Set duration
void Tweener::SetDuration( float duration )
{
    _duration = duration;
}

// Set tween method
void Tweener::SetTweenMethod( TweenMethod method )
{
    if ( method == TweenMethod::TweenMethodCount )
    {
        method = TweenMethod::Linear;
    }

    _tweenMethod = method;
    _tweenFunction = _tweenFunctions[ static_cast<int>( method ) ];
}

// Starts the tweener
void Tweener::Start( const GameTime& gameTime, float* target )
{
    _startTime = gameTime.GetTotalTime();
    _targetValue = target;
    SetEnabled( true );
}

// Update tween component
void Tweener::Update( const GameTime& gameTime )
{
    // Get the time, and make sure it doesn't go higher than the duration
    float time = gameTime.GetTotalTime() - _startTime;
    if ( time >= _duration )
    {
        time = _duration;
        SetEnabled( false );
    }

    // Modify the target value
    if ( _tweenFunction && _targetValue )
    {
        float value = _tweenFunction( _startValue, _endingValue, time, _duration );
        *_targetValue = value;
    }
}
