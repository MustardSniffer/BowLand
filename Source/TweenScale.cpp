#include "TweenScale.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"

using namespace DirectX;

// Creates a new tween rotation component
TweenScale::TweenScale( GameObject* gameObject )
    : Tweener( gameObject )
    , _scale( 1, 1, 1 )
{
    _targetValue.SetTarget( &_scale );

    SetStartValue( XMFLOAT3( 1, 1, 1 ) );
    SetEndValue( XMFLOAT3( 1, 1, 1 ) );

    _isEnabled = true;
}

// Gets the starting value
XMFLOAT3 TweenScale::GetStartValue() const
{
    return _startValue.GetFloat3();
}

// Gets the ending value
XMFLOAT3 TweenScale::GetEndValue() const
{
    return _endValue.GetFloat3();
}

// Sets the starting value
void TweenScale::SetStartValue( const XMFLOAT3& value )
{
    _startValue.SetValue( value );
}

// Sets the ending value
void TweenScale::SetEndValue( const XMFLOAT3& value )
{
    _endValue.SetValue( value );
}

// Updates this component
void TweenScale::Update()
{
    Tweener::Update();

    Transform* transform = _gameObject->GetTransform();
    transform->SetScale( _scale );
}
