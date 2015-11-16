#pragma once

#include <DirectXMath.h>
#include "TweenType.hpp"

/// <summary>
/// Defines a tween value.
/// </summary>
class TweenValue
{
    // Union to cut back on used memory
    DirectX::XMFLOAT4 _values;
    TweenType _valueType;

public:
    /// <summary>
    /// Creates a default tween value.
    /// </summary>
    TweenValue();

    /// <summary>
    /// Creates a new tween value.
    /// </summary>
    /// <param name="value">The initial underlying value.</param>
    TweenValue( float value );

    /// <summary>
    /// Creates a new tween value.
    /// </summary>
    /// <param name="value">The initial underlying value.</param>
    TweenValue( DirectX::XMFLOAT2 value );

    /// <summary>
    /// Creates a new tween value.
    /// </summary>
    /// <param name="value">The initial underlying value.</param>
    TweenValue( DirectX::XMFLOAT3 value );

    /// <summary>
    /// Creates a new tween value.
    /// </summary>
    /// <param name="value">The initial underlying value.</param>
    TweenValue( DirectX::XMFLOAT4 value );

    /// <summary>
    /// Destroys this tween value.
    /// </summary>
    ~TweenValue();

    /// <summary>
    /// Gets the underlying value as a single float.
    /// </summary>
    inline float GetFloat() const
    {
        return _values.x;
    }

    /// <summary>
    /// Gets the underlying value as a float2.
    /// </summary>
    inline DirectX::XMFLOAT2 GetFloat2() const
    {
        return DirectX::XMFLOAT2(_values.x, _values.y);
    }

    /// <summary>
    /// Gets the underlying value as a float3.
    /// </summary>
    inline DirectX::XMFLOAT3 GetFloat3() const
    {
        return DirectX::XMFLOAT3(_values.x, _values.y, _values.z);
    }

    /// <summary>
    /// Gets the underlying value as a float4.
    /// </summary>
    inline DirectX::XMFLOAT4 GetFloat4() const
    {
        return _values;
    }

    /// <summary>
    /// Gets the value type.
    /// </summary>
    inline TweenType GetValueType() const
    {
        return _valueType;
    }

    /// <summary>
    /// Sets the underlying value.
    /// </summary>
    /// <param name="value">The new value.</param>
    void SetValue( float value );

    /// <summary>
    /// Sets the underlying value.
    /// </summary>
    /// <param name="value">The new value.</param>
    void SetValue( DirectX::XMFLOAT2 value );

    /// <summary>
    /// Sets the underlying value.
    /// </summary>
    /// <param name="value">The new value.</param>
    void SetValue( DirectX::XMFLOAT3 value );

    /// <summary>
    /// Sets the underlying value.
    /// </summary>
    /// <param name="value">The new value.</param>
    void SetValue( DirectX::XMFLOAT4 value );
};