#include "Tweener.hpp"
#include "GameObject.hpp"
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define M_FPI static_cast<float>( M_PI )

using namespace DirectX;

/// <summary>
/// Registers the given tween method and the provided function.
/// </summary>
/// <param name="method">The tween method.</param>
/// <param name="function">The tween function. Parameters provided: start, end, time, duration.</param>
#define RegisterTweenFunction(method, function) \
    _tweenFunctions[static_cast<int>( method )] = [](float start, float end, float time, float duration) -> float function

/// <summary>
/// Implements an ease out/in or ease in/out function.
/// </summary>
/// <param name="firstFunc">The first function type.</param>
/// <param name="secondFunc">The second function type.</param>
#define ImplementMixedEaseFunction(firstFunc, secondFunc) \
    TweenFunction function = nullptr; \
    float s = 0.0f; \
    float e = 0.0f; \
    float t = 0.0f; \
    if ( time < duration * 0.5f ) \
    { \
        function = _tweenFunctions[ static_cast<int>( firstFunc ) ]; \
        s = start; \
        e = start + ( end - start ) * 0.5f; \
        t = time * 2.0f; \
    } \
    else \
    { \
        function = _tweenFunctions[ static_cast<int>( secondFunc ) ]; \
        s = start + ( end - start ) * 0.5f; \
        e = end; \
        t = time * 2.0f - duration; \
    } \
    return function( s, e, t, duration )

// Creates all of the tween functions
void Tweener::CreateTweenFunctions()
{
    _tweenFunctions.resize( static_cast<int>( TweenMethod::TweenMethodCount ) );

    // All tween code is based off of Robert Penner's easing functions http://robertpenner.com/easing/
    // Specifically, the C# implementations http://wpf-animation.googlecode.com/svn/trunk/src/WPF/Animation/PennerDoubleAnimation.cs

    RegisterTweenFunction( TweenMethod::Linear,
    {
        return ( end - start ) * time / duration + start;
    } );
    RegisterTweenFunction( TweenMethod::QuadraticEaseOut,
    {
        return ( start - end ) * ( time /= duration ) * ( time - 2.0f ) + start;
    } );
    RegisterTweenFunction( TweenMethod::QuadraticEaseIn,
    {
        return ( end - start ) * ( time /= duration ) * time + start;
    } );
    RegisterTweenFunction( TweenMethod::QuadraticEaseInOut,
    {
        ImplementMixedEaseFunction( TweenMethod::QuadraticEaseIn, TweenMethod::QuadraticEaseOut );
    } );
    RegisterTweenFunction( TweenMethod::QuadraticEaseOutIn,
    {
        ImplementMixedEaseFunction( TweenMethod::QuadraticEaseOut, TweenMethod::QuadraticEaseIn );
    } );
    RegisterTweenFunction( TweenMethod::ExponentialEaseOut,
    {
        return ( time == duration )
               ? end
               : ( end - start ) * ( -powf( 2.0f, -10.0f * time / duration ) + 1.0f ) + start;
    } );
    RegisterTweenFunction( TweenMethod::ExponentialEaseIn,
    {
        return ( time == 0.0f )
               ? start
               : ( end - start ) * powf( 2.0f, 10.0f * ( time / duration - 1 ) ) + start;
    } );
    RegisterTweenFunction( TweenMethod::ExponentialEaseInOut,
    {
        ImplementMixedEaseFunction( TweenMethod::ExponentialEaseIn, TweenMethod::ExponentialEaseOut );
    } );
    RegisterTweenFunction( TweenMethod::ExponentialEaseOutIn,
    {
        ImplementMixedEaseFunction( TweenMethod::ExponentialEaseOut, TweenMethod::ExponentialEaseIn );
    } );
    RegisterTweenFunction( TweenMethod::CubicEaseOut,
    {
        return ( end - start ) * ( ( time = time / duration - 1.0f ) * time * time + 1.0f ) + start;
    } );
    RegisterTweenFunction( TweenMethod::CubicEaseIn,
    {
        return ( end - start ) * ( time /= duration ) * time * time + start;
    } );
    RegisterTweenFunction( TweenMethod::CubicEaseInOut,
    {
        ImplementMixedEaseFunction( TweenMethod::CubicEaseIn, TweenMethod::CubicEaseOut );
    } );
    RegisterTweenFunction( TweenMethod::CubicEaseOutIn,
    {
        ImplementMixedEaseFunction( TweenMethod::CubicEaseOut, TweenMethod::CubicEaseIn );
    } );
    RegisterTweenFunction( TweenMethod::QuarticEaseOut,
    {
        return ( start - end ) * ( ( time = time / duration - 1.0f ) * time * time * time - 1.0f ) + start;
    } );
    RegisterTweenFunction( TweenMethod::QuarticEaseIn,
    {
        return ( end - start ) * ( time /= duration ) * time * time * time + start;
    } );
    RegisterTweenFunction( TweenMethod::QuarticEaseInOut,
    {
        ImplementMixedEaseFunction( TweenMethod::QuarticEaseIn, TweenMethod::QuarticEaseOut );
    } );
    RegisterTweenFunction( TweenMethod::QuarticEaseOutIn,
    {
        ImplementMixedEaseFunction( TweenMethod::QuarticEaseOut, TweenMethod::QuarticEaseIn );
    } );
    RegisterTweenFunction( TweenMethod::QuinticEaseOut,
    {
        return ( end - start ) * ( ( time = time / duration - 1.0f ) * time * time * time * time + 1.0f ) + start;
    } );
    RegisterTweenFunction( TweenMethod::QuinticEaseIn,
    {
        return ( end - start ) * ( time /= duration ) * time * time * time * time + start;
    } );
    RegisterTweenFunction( TweenMethod::QuinticEaseInOut,
    {
        ImplementMixedEaseFunction( TweenMethod::QuinticEaseIn, TweenMethod::QuinticEaseOut );
    } );
    RegisterTweenFunction( TweenMethod::QuinticEaseOutIn,
    {
        ImplementMixedEaseFunction( TweenMethod::QuinticEaseOut, TweenMethod::QuinticEaseIn );
    } );
    RegisterTweenFunction( TweenMethod::CircularEaseOut,
    {
        return ( end - start ) * sqrtf( 1.0f - ( time = time / duration - 1.0f ) * time ) + start;
    } );
    RegisterTweenFunction( TweenMethod::CircularEaseIn,
    {
        return ( start - end ) * ( sqrtf( 1.0f - ( time /= duration ) * time ) - 1.0f ) + start;
    } );
    RegisterTweenFunction( TweenMethod::CircularEaseInOut,
    {
        // TODO - This one is a bit jumpy...
        ImplementMixedEaseFunction( TweenMethod::CircularEaseIn, TweenMethod::CircularEaseOut );
    } );
    RegisterTweenFunction( TweenMethod::CircularEaseOutIn,
    {
        ImplementMixedEaseFunction( TweenMethod::CircularEaseOut, TweenMethod::CircularEaseIn );
    } );
    RegisterTweenFunction( TweenMethod::SineEaseOut,
    {
        return ( end - start ) * sinf( time / duration * ( M_FPI * 0.5f ) ) + start;
    } );
    RegisterTweenFunction( TweenMethod::SineEaseIn,
    {
        return ( start - end ) * cosf( time / duration * ( M_FPI * 0.5f ) ) + ( end - start ) + start;
    } );
    RegisterTweenFunction( TweenMethod::SineEaseInOut,
    {
        ImplementMixedEaseFunction( TweenMethod::SineEaseIn, TweenMethod::SineEaseOut );
    } );
    RegisterTweenFunction( TweenMethod::SineEaseOutIn,
    {
        ImplementMixedEaseFunction( TweenMethod::SineEaseOut, TweenMethod::SineEaseIn );
    } );
    RegisterTweenFunction( TweenMethod::ElasticEaseOut,
    {
        if ( ( time /= duration ) == 1.0f )
        {
            return end;
        }

        float p = duration * 0.3f;
        float s = p * 0.25f;

        return ( end - start ) * powf( 2.0f, -10.0f * time ) * sinf( ( time * duration - s ) * ( 2.0f * M_FPI ) / p ) + ( end - start ) + start;
    } );
    RegisterTweenFunction( TweenMethod::ElasticEaseIn,
    {
        if ( ( time /= duration ) == 1.0f )
        {
            return end;
        }

        float p = duration * 0.3f;
        float s = p * 0.25f;

        return -( ( end - start ) * powf( 2.0f, 10.0f * ( time -= 1.0f ) ) * sinf( ( time * duration - s ) * ( 2.0f * M_FPI ) / p ) ) + start;
    } );
    RegisterTweenFunction( TweenMethod::ElasticEaseInOut,
    {
        ImplementMixedEaseFunction( TweenMethod::ElasticEaseIn, TweenMethod::ElasticEaseOut );
    } );
    RegisterTweenFunction( TweenMethod::ElasticEaseOutIn,
    {
        ImplementMixedEaseFunction( TweenMethod::ElasticEaseOut, TweenMethod::ElasticEaseIn );
    } );
    /*
    RegisterTweenFunction( TweenMethod::BounceEaseOut,
    {
        float delta = end - start;
        if ( ( time /= duration ) < 0.3636363636f ) // 1 / 2.75
        {
            return delta * ( 7.5625f * time * time ) + start;
        }
        else if ( time < 0.7272727272f ) // 2 / 2.75
        {
            return delta * ( 7.5625f * ( time -= 0.5454545455f ) * time + 0.75f ) + start; // 1.5 / 2.75
        }
        else if ( time < 0.9090909091f ) // 2.5 / 2.75
        {
            return delta * ( 7.5625f * ( time -= 0.8181818182f ) * time + 0.9375f ) + start; // 2.25 / 2.75
        }
        else
        {
            return delta * ( 7.5625f * ( time -= 0.9545454545f ) * time + 0.984375f ) + start; // 2.625 / 2.75
        }
    } );
    RegisterTweenFunction( TweenMethod::BounceEaseIn,
    {
        TweenFunction bounceEaseOut = _tweenFunctions[ static_cast<int>( TweenMethod::BounceEaseOut ) ];
        return ( end - start ) - bounceEaseOut( 0, end, duration - time, duration ) + start;
        //return bounceEaseOut( start, end, duration - time, duration );
    } );
    RegisterTweenFunction( TweenMethod::BounceEaseInOut,
    {
        ImplementMixedEaseFunction( TweenMethod::BounceEaseIn, TweenMethod::BounceEaseOut );
    } );
    RegisterTweenFunction( TweenMethod::BounceEaseOutIn,
    {
        ImplementMixedEaseFunction( TweenMethod::BounceEaseOut, TweenMethod::BounceEaseIn );
    } );
    */
    RegisterTweenFunction( TweenMethod::BackEaseOut,
    {
        return ( end - start ) * ( ( time = time / duration - 1.0f ) * time * ( 2.70158f * time + 1.70158f ) + 1.0f ) + start;
    } );
    RegisterTweenFunction( TweenMethod::BackEaseIn,
    {
        return ( end - start ) * ( time /= duration ) * time * ( 2.70158f * time - 1.70158f ) + start;
    } );
    RegisterTweenFunction( TweenMethod::BackEaseInOut,
    {
        ImplementMixedEaseFunction( TweenMethod::BackEaseIn, TweenMethod::BackEaseOut );
    } );
    RegisterTweenFunction( TweenMethod::BackEaseOutIn,
    {
        ImplementMixedEaseFunction( TweenMethod::BackEaseOut, TweenMethod::BackEaseIn );
    } );
}
