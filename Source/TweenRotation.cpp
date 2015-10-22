#include "TweenRotation.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"

using namespace DirectX;

// Creates a new tween rotation component
TweenRotation::TweenRotation( GameObject* gameObject )
    : Tweener( gameObject )
{
    _targetValue.SetTarget( _gameObject->GetTransform()->GetRotationPtr() );
    SetStartValue( XMFLOAT3( 0, 0, 0 ) );
    SetEndValue( XMFLOAT3( 0, 0, 0 ) );

    _isEnabled = true;
}

// Gets the starting value
XMFLOAT3 TweenRotation::GetStartValue()
{
    return _startValue.GetFloat3();
}

// Gets the ending value
XMFLOAT3 TweenRotation::GetEndValue()
{
    return _endValue.GetFloat3();
}

// Sets the starting value
void TweenRotation::SetStartValue( const XMFLOAT3& value )
{
    _startValue.SetValue( value );
}

// Sets the ending value
void TweenRotation::SetEndValue( const XMFLOAT3& value )
{
    _endValue.SetValue( value );
}

// Updates this component
void TweenRotation::Update( const GameTime& gameTime )
{
    Tweener::Update( gameTime );
    _gameObject->SetWorldMatrixDirty();
}
