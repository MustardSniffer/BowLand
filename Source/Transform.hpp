#pragma once

#include "Component.hpp"
#include "GameObject.hpp"

class Transform : public Component
{
    friend class Camera;
    friend class TweenRotation;
    friend class TweenPosition;
    friend class TweenScale;

protected:
    DirectX::XMFLOAT4 _rotation;
    DirectX::XMFLOAT3 _position;
    DirectX::XMFLOAT3 _scale;

    /// <summary>
    ///	Initializes the starting values of this transform
    /// </summary
    /// <param name="nPos">The initial position of this transform</param>
    void InitializeTransform( DirectX::XMFLOAT3 nPos );

public:
    /// <summary>
    /// Creates a new transform.
    /// </summary>
    /// <param name="gameObj">The parent GameObject of this transform.</param>
    Transform( GameObject* gameObj );

    /// <summary>
    /// Destroys this transform.
    /// </summary>
    ~Transform();

    /// <summary>
    /// Returns the position of this transform.
    /// </summary>
    DirectX::XMFLOAT3 GetPosition() const;

    /// <summary>
    /// Returns the rotation of this transform in Euler angles.
    /// </summary>
    DirectX::XMFLOAT4 GetRotation() const;

    /// <summary>
    /// Returns the scale of this transform.
    /// </summary>
    DirectX::XMFLOAT3 GetScale() const;

    /// <summary>
    /// Gets the world matrix representing this transform.
    /// </summary>
    DirectX::XMFLOAT4X4 GetWorldMatrix() const;

    /// <summary>
    /// Sets this transform to the given position.
    /// </summary>
    /// <param name="nPos">The position to move this transform to.</param>
    void SetPosition( const DirectX::XMFLOAT3& nPos );

    /// <summary>
    /// Sets this transform to a given scale.
    /// </summary>
    /// <param name="nSca">The scale to set this transform to.</param>
    void SetScale( const DirectX::XMFLOAT3& nSca );

    /// <summary>
    /// Sets this transform's rotation.
    /// </summary>
    /// <param name="nRot">The quaternion rotation.</param>
    void SetRotation( const DirectX::XMFLOAT4& nRot );

    /// <summary>
    /// Sets this transform's rotation from Euler angles in radians.
    /// </summary>
    /// <param name="yaw">The rotation's yaw.</param>
    /// <param name="pitch">The rotation's pitch.</param>
    /// <param name="roll">The rotation's roll.</param>
    void SetRotation( float yaw, float pitch, float roll );

    /// <summary>
    /// Updates this component.
    /// </summary>
    void Update() override;
};

