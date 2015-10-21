#include "Tweener.hpp"
#include "Time.hpp"
#include "GameObject.hpp"
#include <assert.h>

using namespace DirectX;

std::vector<Tweener::TweenFunction> Tweener::_tweenFunctions;

// Create a new tweener
Tweener::Tweener( GameObject* gameObject )
    : Component( gameObject )
    , _tweenMethod( TweenMethod::Linear )
    , _playMode( TweenPlayMode::Once )
    , _tweenFunction( nullptr )
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
    _tweenMethod   = TweenMethod::Linear; // 0
    _tweenFunction = nullptr;
    _duration      = 0.0f;
    _startTime     = 0.0f;
}

// Checks to see if the values are compatible.
bool Tweener::AreValuesCompatible() const
{
    return   _targetValue
        && ( _startValue.GetValueType() == _endValue.GetValueType() )
        && ( _startValue.GetValueType() == _targetValue.GetTargetType() );
}

// Handle when our animation ends
void Tweener::OnEnd()
{
    switch ( _playMode )
    {
        case TweenPlayMode::Loop:
        {
            this->Play();
        }
        break;

        case TweenPlayMode::PingPong:
        {
            // Swap the start and end values
            TweenValue end = _endValue;
            _endValue = _startValue;
            _startValue = end;

            // Begin again
            this->Play();
        }
        break;

        case TweenPlayMode::None:
        case TweenPlayMode::Once:
        default:
        {
            this->SetEnabled( false );
        }
        break;
    }
}

// Get the tween duration
float Tweener::GetDuration() const
{
    return _duration;
}

// Get the tween method
TweenMethod Tweener::GetTweenMethod() const
{
    return _tweenMethod;
}

// Get the play mode
TweenPlayMode Tweener::GetPlayMode() const
{
    return _playMode;
}

// Plays our tween animation
void Tweener::Play()
{
    _startTime = Time::GetTotalTime();

    // Ensure the values are compatible and only start if they are
    SetEnabled( AreValuesCompatible() );
}

// Set duration
void Tweener::SetDuration( float duration )
{
    _duration = duration;
}

// Set play mode
void Tweener::SetPlayMode( TweenPlayMode mode )
{
    _playMode = mode;
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

// Update tween component
void Tweener::Update( const GameTime& gameTime )
{
    assert( AreValuesCompatible() && "Tween values are incompatible!" );

    // Get the time, and make sure it doesn't go higher than the duration
    float time = gameTime.GetTotalTime() - _startTime;
    if ( time >= _duration )
    {
        time = _duration;
        OnEnd();
    }

    // Modify the target value
    if ( _tweenFunction && _targetValue )
    {
        switch ( _targetValue.GetTargetType() )
        {
            case TweenType::Float:
            {
                float src = _startValue.GetFloat();
                float dst = _endValue.GetFloat();
                
                float value = _tweenFunction( src, dst, time, _duration );
                _targetValue.SetValue( value );
                
                break;
            }
            case TweenType::Float2:
            {
                XMFLOAT2 src = _startValue.GetFloat2();
                XMFLOAT2 dst = _endValue.GetFloat2();
                
                XMFLOAT2 value;
                value.x = _tweenFunction( src.x, dst.x, time, _duration );
                value.y = _tweenFunction( src.y, dst.y, time, _duration );

                _targetValue.SetValue( value );
                break;
            }
            case TweenType::Float3:
            {
                XMFLOAT3 src = _startValue.GetFloat3();
                XMFLOAT3 dst = _endValue.GetFloat3();

                XMFLOAT3 value;
                value.x = _tweenFunction( src.x, dst.x, time, _duration );
                value.y = _tweenFunction( src.y, dst.y, time, _duration );
                value.z = _tweenFunction( src.z, dst.z, time, _duration );

                _targetValue.SetValue( value );
                break;
            }
            case TweenType::Float4:
            {
                XMFLOAT4 src = _startValue.GetFloat4();
                XMFLOAT4 dst = _endValue.GetFloat4();

                XMFLOAT4 value;
                value.x = _tweenFunction( src.x, dst.x, time, _duration );
                value.y = _tweenFunction( src.y, dst.y, time, _duration );
                value.z = _tweenFunction( src.z, dst.z, time, _duration );
                value.w = _tweenFunction( src.w, dst.w, time, _duration );

                _targetValue.SetValue( value );
                break;
            }
        }
    }
}
