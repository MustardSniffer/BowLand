#pragma once

#include "Tweener.hpp"

/// <summary>
/// Defines a tween component that only affects a GameObject's scale.
/// </summary>
class TweenScale : public Tweener
{
    DirectX::XMFLOAT3 _scale;

public:
    /// <summary>
    /// Creates a new tween scale component.
    /// </summary>
    TweenScale( GameObject* gameObject );

    /// <summary>
    /// Gets the starting scale value.
    /// </summary>
    DirectX::XMFLOAT3 GetStartValue() const;

    /// <summary>
    /// Gets the ending scale value.
    /// </summary>
    DirectX::XMFLOAT3 GetEndValue() const;

    /// <summary>
    /// Sets the starting scale value.
    /// </summary>
    /// <param name="value">The new starting value.</param>
    void SetStartValue( const DirectX::XMFLOAT3& value );

    /// <summary>
    /// Sets the ending scale value.
    /// </summary>
    /// <param name="value">The new ending value.</param>
    void SetEndValue( const DirectX::XMFLOAT3& value );

    /// <summary>
    /// Updates this tween scale component.
    /// </summary>
    void Update() override;
};
