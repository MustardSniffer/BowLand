#pragma once

#include "Component.hpp"
#include "GameObject.hpp"

class Transform : public Component
{
    friend class TweenRotation;
    friend class TweenPosition;
    friend class TweenScale;

protected:
    DirectX::XMFLOAT3 _position;
    DirectX::XMFLOAT3 _scale;
    DirectX::XMFLOAT3 _rotation;

    /// <summary>
    ///	Initializes the starting values of this transform
    /// </summary
    /// <param name="nPos">The initial position of this transform</param>
    void InitializeTransform(DirectX::XMFLOAT3 nPos);

public:
    /// <summary>
    /// Creates a new transform.
    /// </summary>
    /// <param name="gameObj">The parent GameObject of this transform.</param>
    Transform(GameObject* gameObj);

    /// <summary>
    /// Destroys this transform.
    /// </summary>
    ~Transform();

    /// <summary>
    /// Sets this transform's values to those of another transform.
    /// </summary>
    /// <param name="nTra">The transform to be copied into this one.</param>
    void CopyTransform(Transform* nTra);

    /// <summary>
    /// Sets this transform to the given position.
    /// </summary>
    /// <param name="nPos">The position to move this transform to.</param>
    void SetPosition(DirectX::XMFLOAT3 nPos);

    /// <summary>
    /// Sets this transform to a given scale.
    /// </summary>
    /// <param name="nSca">The scale to set this transform to.</param>
    void SetScale(DirectX::XMFLOAT3 nSca);

    /// <summary>
    /// Sets this transform to a given rotation in Euler angles.
    /// </summary>
    /// <param name="">The rotation to set this transform to.</param>
    void SetRotation(DirectX::XMFLOAT3 nRot);

    /// <summary>
    /// Returns the position of this transform.
    /// </summary>
    DirectX::XMFLOAT3 GetPosition() const;

    /// <summary>
    /// Returns the scale of this transform.
    /// </summary>
    DirectX::XMFLOAT3 GetScale() const;

    /// <summary>
    /// Returns the rotation of this transform in Euler angles.
    /// </summary>
    DirectX::XMFLOAT3 GetRotation() const;

    /// <summary>
    /// Updates this component.
    /// </summary>
    /// <param name="gameTime">Provides a snapshot of timing values.</param>
    void Update(const GameTime& gameTime) override;
};

