#include "TweenValue.hpp"

using namespace DirectX;

// Create default target
TweenValue::TweenValue()
{
    _values = XMFLOAT4( 0, 0, 0, 0 );
    SetValue( 0.0f );
}

// Create float target
TweenValue::TweenValue( float value )
{
    _values = XMFLOAT4( 0, 0, 0, 0 );
    SetValue( value );
}

// Create float2 target
TweenValue::TweenValue( DirectX::XMFLOAT2 value )
{
    _values = XMFLOAT4( 0, 0, 0, 0 );
    SetValue( value );
}

// Create float3 target
TweenValue::TweenValue( DirectX::XMFLOAT3 value )
{
    _values = XMFLOAT4( 0, 0, 0, 0 );
    SetValue( value );
}

// Create float4 target
TweenValue::TweenValue( DirectX::XMFLOAT4 value )
{
    SetValue( value );
}

// Destroy this tween value
TweenValue::~TweenValue()
{
    _values = XMFLOAT4( 0, 0, 0, 0 );
}

// Set value to be a single float
void TweenValue::SetValue( float value )
{
    _values.x = value;
    _valueType = TweenType::Float;
}

// Set value to be a float2
void TweenValue::SetValue( DirectX::XMFLOAT2 value )
{
    _values.x = value.x;
    _values.y = value.y;
    _valueType = TweenType::Float2;
}

// Set value to be a float3
void TweenValue::SetValue( DirectX::XMFLOAT3 value )
{
    _values.x = value.x;
    _values.y = value.y;
    _values.z = value.z;
    _valueType = TweenType::Float3;
}

// Set value to be a float4
void TweenValue::SetValue( DirectX::XMFLOAT4 value )
{
    _values = value;
    _valueType = TweenType::Float4;
}
