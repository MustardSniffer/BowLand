#pragma once

// TODO - This needs to be redone because of the switch to quaternions

#include "Tweener.hpp"

/// <summary>
/// Defines a tween component that only affects a GameObject's rotation.
/// </summary>
class TweenRotation : public Tweener
{
public:
    /// <summary>
    /// Creates a new tween rotation component.
    /// </summary>
    TweenRotation( GameObject* gameObject );

    /// <summary>
    /// Gets the starting rotation value.
    /// </summary>
    DirectX::XMFLOAT3 GetStartValue() const;

    /// <summary>
    /// Gets the ending rotation value.
    /// </summary>
    DirectX::XMFLOAT3 GetEndValue() const;

    /// <summary>
    /// Sets the starting rotation value.
    /// </summary>
    /// <param name="value">The new starting value.</param>
    void SetStartValue( const DirectX::XMFLOAT3& value );

    /// <summary>
    /// Sets the ending rotation value.
    /// </summary>
    /// <param name="value">The new ending value.</param>
    void SetEndValue( const DirectX::XMFLOAT3& value );

    /// <summary>
    /// Updates this tween rotation component.
    /// </summary>
    void Update() override;
};
