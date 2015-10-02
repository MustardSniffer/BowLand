#pragma once

#include "DirectX.hpp"

/// <summary>
/// Defines a camera.
/// </summary>
class Camera
{
    DirectX::XMFLOAT4X4 _viewMatrix;
    DirectX::XMFLOAT4X4 _projMatrix;
    DirectX::XMFLOAT3   _position;
    DirectX::XMFLOAT3   _translation;
    DirectX::XMFLOAT3   _viewDirection;
    DirectX::XMFLOAT2   _rotation;

public:
    /// <summary>
    /// Creates a new camera.
    /// </summary>
    /// <param name="fovy">The top-down field of view angle.</param>
    /// <param name="aspect">The display region aspect ratio.</param>
    /// <param name="nearZ">The near clipping plane's distance from the camera.</param>
    /// <param name="farZ">The far clipping plane's distance from the camera.</param>
    Camera( float fovy, float aspect, float nearZ, float farZ );

    /// <summary>
    /// Destroys this camera.
    /// </summary>
    ~Camera();

    /// <summary>
    /// Gets this camera's view matrix.
    /// </summary>
    const DirectX::XMFLOAT4X4& GetViewMatrix() const;

    /// <summary>
    /// Gets this camera's projection matrix.
    /// </summary>
    const DirectX::XMFLOAT4X4& GetProjectionMatrix() const;

    // GetPosition, GetViewDirection, GetRotation?

    /// <summary>
    /// Moves this camera.
    /// </summary>
    /// <param name="x">The amount to move in the X direction.</param>
    /// <param name="y">The amount to move in the Y direction.</param>
    /// <param name="z">The amount to move in the Z direction.</param>
    void Move( float x, float y, float z );

    /// <summary>
    /// Moves this camera.
    /// </summary>
    /// <param name="amount">The amount to move.</param>
    void Move( const DirectX::XMFLOAT3& amount );

    /// <summary>
    /// Moves this camera immediately instead of waiting until Update is called.
    /// </summary>
    /// <param name="x">The amount to move in the X direction.</param>
    /// <param name="y">The amount to move in the Y direction.</param>
    /// <param name="z">The amount to move in the Z direction.</param>
    void MoveImmediate( float x, float y, float z );

    /// <summary>
    /// Moves this camera immediately instead of waiting until Update is called.
    /// </summary>
    /// <param name="amount">The amount to move.</param>
    void MoveImmediate( const DirectX::XMFLOAT3& amount );

    /// <summary>
    /// Rotates this camera.
    /// </summary>
    /// <param name="x">The amount to rotate in the X direction.</param>
    /// <param name="y">The amount to rotate in the Y direction.</param>
    void Rotate( float x, float y );

    /// <summary>
    /// Rotates this camera.
    /// </summary>
    /// <param name="amount">The amount to rotate.</param>
    void Rotate( const DirectX::XMFLOAT2& amount );

    /// <summary>
    /// Update's this camera's view matrix.
    /// </summary>
    void Update();

    /// <summary>
    /// Updates this camera's projection matrix.
    /// </summary>
    /// <param name="fovy">The top-down field of view angle.</param>
    /// <param name="aspect">The display region aspect ratio.</param>
    /// <param name="nearZ">The near clipping plane's distance from the camera.</param>
    /// <param name="farZ">The far clipping plane's distance from the camera.</param>
    void UpdateProjection( float fovy, float aspect, float nearZ, float farZ );
};