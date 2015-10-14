#include "TweenValue.hpp"

using namespace DirectX;

// Create default target
TweenValue::TweenValue()
{
    _value.Float4 = XMFLOAT4( 0, 0, 0, 0 );
    SetValue( 0.0f );
}

// Create float target
TweenValue::TweenValue( float value )
{
    _value.Float4 = XMFLOAT4( 0, 0, 0, 0 );
    SetValue( value );
}

// Create float2 target
TweenValue::TweenValue( DirectX::XMFLOAT2 value )
{
    _value.Float4 = XMFLOAT4( 0, 0, 0, 0 );
    SetValue( value );
}

// Create float3 target
TweenValue::TweenValue( DirectX::XMFLOAT3 value )
{
    _value.Float4 = XMFLOAT4( 0, 0, 0, 0 );
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
    _value.Float4 = XMFLOAT4( 0, 0, 0, 0 );
}

// Set value to be a single float
void TweenValue::SetValue( float value )
{
    _value.Float = value;
    _valueType = TweenType::Float;
}

// Set value to be a float2
void TweenValue::SetValue( DirectX::XMFLOAT2 value )
{
    _value.Float2 = value;
    _valueType = TweenType::Float2;
}

// Set value to be a float3
void TweenValue::SetValue( DirectX::XMFLOAT3 value )
{
    _value.Float3 = value;
    _valueType = TweenType::Float3;
}

// Set value to be a float4
void TweenValue::SetValue( DirectX::XMFLOAT4 value )
{
    _value.Float4 = value;
    _valueType = TweenType::Float4;
}
