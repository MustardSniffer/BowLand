#pragma once

#include <DirectXMath.h>
#include "TweenType.hpp"

/// <summary>
/// Defines a target for tweening operations.
/// </summary>
class TweenTarget
{
    union
    {
        float* Float;
        DirectX::XMFLOAT2* Float2;
        DirectX::XMFLOAT3* Float3;
        DirectX::XMFLOAT4* Float4;
    } _target;
    TweenType _targetType;

public:
    /// <summary>
    /// Creates a new tween target.
    /// </summary>
    TweenTarget();

    /// <summary>
    /// Creates a new tween target.
    /// </summary>
    /// <param name="target">The initial target.</param>
    TweenTarget( float* target );

    /// <summary>
    /// Creates a new tween target.
    /// </summary>
    /// <param name="target">The initial target.</param>
    TweenTarget( DirectX::XMFLOAT2* target );

    /// <summary>
    /// Creates a new tween target.
    /// </summary>
    /// <param name="target">The initial target.</param>
    TweenTarget( DirectX::XMFLOAT3* target );

    /// <summary>
    /// Creates a new tween target.
    /// </summary>
    /// <param name="target">The initial target.</param>
    TweenTarget( DirectX::XMFLOAT4* target );

    /// <summary>
    /// Destroys this tween target.
    /// </summary>
    ~TweenTarget();

    /// <summary>
    /// Gets the target as the given type.
    /// </summary>
    template<typename T> inline const T* GetTarget() const
    {
        // Since it's a union, it doesn't matter which one we return
        return reinterpret_cast<const T*>( static_cast<const void*>( _target.Float ) );
    }

    /// <summary>
    /// Gets the target as the given type.
    /// </summary>
    template<typename T> inline T* GetTarget()
    {
        // Since it's a union, it doesn't matter which one we return
        return reinterpret_cast<T*>( static_cast<void*>( _target.Float ) );
    }

    /// <summary>
    /// Gets the target type.
    /// </summary>
    inline TweenType GetTargetType() const
    {
        return _targetType;
    }

    /// <summary>
    /// Sets the tween target.
    /// </summary>
    /// <param name="target">The new target.</param>
    void SetTarget( float* target );

    /// <summary>
    /// Sets the tween target.
    /// </summary>
    /// <param name="target">The new target.</param>
    void SetTarget( DirectX::XMFLOAT2* target );

    /// <summary>
    /// Sets the tween target.
    /// </summary>
    /// <param name="target">The new target.</param>
    void SetTarget( DirectX::XMFLOAT3* target );

    /// <summary>
    /// Sets the tween target.
    /// </summary>
    /// <param name="target">The new target.</param>
    void SetTarget( DirectX::XMFLOAT4* target );

    /// <summary>
    /// Sets the target's value.
    /// </summary>
    /// <param name="value">The new value.</param>
    void SetValue( float value );

    /// <summary>
    /// Sets the target's value.
    /// </summary>
    /// <param name="value">The new value.</param>
    void SetValue( DirectX::XMFLOAT2 value );

    /// <summary>
    /// Sets the target's value.
    /// </summary>
    /// <param name="value">The new value.</param>
    void SetValue( DirectX::XMFLOAT3 value );

    /// <summary>
    /// Sets the target's value.
    /// </summary>
    /// <param name="value">The new value.</param>
    void SetValue( DirectX::XMFLOAT4 value );

    /// <summary>
    /// Implicitly checks to see if this target has a value.
    /// </summary>
    inline operator bool() const
    {
        return static_cast<const void*>( _target.Float ) != nullptr;
    }
};