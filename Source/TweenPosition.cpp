#include "TweenPosition.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"

using namespace DirectX;

// Creates a new tween position component
TweenPosition::TweenPosition( GameObject* gameObject )
    : Tweener( gameObject )
    , _position( 0, 0, 0 )
{
    _targetValue.SetTarget( &_position );

    SetStartValue( XMFLOAT3( 0, 0, 0 ) );
    SetEndValue( XMFLOAT3( 0, 0, 0 ) );

    _isEnabled = true;
}

// Gets the starting value
XMFLOAT3 TweenPosition::GetStartValue() const
{
    return _startValue.GetFloat3();
}

// Gets the ending value
XMFLOAT3 TweenPosition::GetEndValue() const
{
    return _endValue.GetFloat3();
}

// Sets the starting value
void TweenPosition::SetStartValue( const XMFLOAT3& value )
{
    _startValue.SetValue( value );
}

// Sets the ending value
void TweenPosition::SetEndValue( const XMFLOAT3& value )
{
    _endValue.SetValue( value );
}

// Updates this component
void TweenPosition::Update()
{
    Tweener::Update();
    
    Transform* transform = _gameObject->GetTransform();
    transform->SetPosition( _position );
}
