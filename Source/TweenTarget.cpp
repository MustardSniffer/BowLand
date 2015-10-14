#include "TweenTarget.hpp"

using namespace DirectX;

// Create a new, empty tween target
TweenTarget::TweenTarget()
    : _targetType( TweenType::Float )
{
    _target.Float = nullptr;
}

// Set target to be a single float
TweenTarget::TweenTarget( float* target )
{
    SetTarget( target );
}

// Set target to be a float2
TweenTarget::TweenTarget( XMFLOAT2* target )
{
    SetTarget( target );
}

// Set target to be a float3
TweenTarget::TweenTarget( XMFLOAT3* target )
{
    SetTarget( target );
}

// Set target to be a float4
TweenTarget::TweenTarget( XMFLOAT4* target )
{
    SetTarget( target );
}

// Destroy the tween target
TweenTarget::~TweenTarget()
{
    _target.Float = nullptr;
    _targetType = TweenType::Float; // 0
}

// Set target as a float
void TweenTarget::SetTarget( float* target )
{
    _target.Float = target;
    _targetType = TweenType::Float;
}

// Set target as a float2
void TweenTarget::SetTarget( XMFLOAT2* target )
{
    _target.Float2 = target;
    _targetType = TweenType::Float2;
}

// Set target as a float3
void TweenTarget::SetTarget( XMFLOAT3* target )
{
    _target.Float3 = target;
    _targetType = TweenType::Float3;
}

// Set target as a float4
void TweenTarget::SetTarget( XMFLOAT4* target )
{
    _target.Float4 = target;
    _targetType = TweenType::Float4;
}

// Set the float target value
void TweenTarget::SetValue( float value )
{
    if ( _targetType == TweenType::Float )
    {
        *_target.Float = value;
    }
}

// Set the float2 target value
void TweenTarget::SetValue( XMFLOAT2 value )
{
    if ( _targetType == TweenType::Float2 )
    {
        *_target.Float2 = value;
    }
}

// Set the float3 target value
void TweenTarget::SetValue( XMFLOAT3 value )
{
    if ( _targetType == TweenType::Float3 )
    {
        *_target.Float3 = value;
    }
}

// Set the float4 target value
void TweenTarget::SetValue( XMFLOAT4 value )
{
    if ( _targetType == TweenType::Float4 )
    {
        *_target.Float4 = value;
    }
}
