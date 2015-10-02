#pragma once

#include "Mesh.hpp"
#include "DirectX.hpp"
#include "Material.hpp"
#include <memory> // for std::shared_ptr

/// <summary>
/// Defines an entity.
/// </summary>
class Entity
{
    DirectX::XMFLOAT4X4 _worldMatrix;
    DirectX::XMFLOAT4X4 _rotationMatrix;
    DirectX::XMFLOAT3 _position;
    DirectX::XMFLOAT3 _rotation;
    DirectX::XMFLOAT3 _scale;
    std::shared_ptr<Mesh> _mesh;
    std::shared_ptr<Material> _material;
    bool _isWorldMatrixStale;
    bool _isRotationMatrixStale;

    // Prevent access to the move constructor
    Entity( Entity&& ) = delete;

    // Prevent access to the move assignment operator
    Entity& operator=( Entity&& ) = delete;

public:
    /// <summary>
    /// Creates a new entity.
    /// </summary>
    /// <param name="mesh">The mesh to use with this entity.</param>
    Entity( std::shared_ptr<Mesh>& mesh, std::shared_ptr<Material>& material );

    /// <summary>
    /// Creates a new entity, copying data from another.
    /// </summary>
    Entity( const Entity& ) = default;

    /// <summary>
    /// Destroys this entity.
    /// </summary>
    ~Entity() = default; // TODO - Virtual if this becomes a base class

    /// <summary>
    /// Gets this entity's position.
    /// </summary>
    const DirectX::XMFLOAT3& GetPosition() const;

    /// <summary>
    /// Gets this entity's rotation.
    /// </summary>
    const DirectX::XMFLOAT3& GetRotation() const;

    /// <summary>
    /// Gets this entity's scale.
    /// </summary>
    const DirectX::XMFLOAT3& GetScale() const;

    /// <summary>
    /// Draws this entity.
    /// </summary>
    /// <param name="deviceContext">The device context to use to draw this entity.</param>
    /// <param name="vertexShader">The vertex shader to use to set the world matrix of.</param>
    void Draw( ID3D11DeviceContext* deviceContext );

    /// <summary>
    /// Gets this entity's world matrix.
    /// </summary>
    const DirectX::XMFLOAT4X4& GetWorldMatrix();

    /// <summary>
    /// Gets this entity's rotation matrix.
    /// </summary>
    const DirectX::XMFLOAT4X4& GetRotationMatrix();

    /// <summary>
    /// Moves this entity.
    /// </summary>
    /// <param name="x">The amount to move along the X-axis.</param>
    /// <param name="y">The amount to move along the Y-axis.</param>
    /// <param name="z">The amount to move along the Z-axis.</param>
    void Move( float x, float y, float z );

    /// <summary>
    /// Moves this entity.
    /// </summary>
    /// <param name="amount">The amount to move by.</param>
    void Move( const DirectX::XMFLOAT3& amount );

    /// <summary>
    /// Rotates this entity.
    /// </summary>
    /// <param name="x">The amount to rotate along the X-axis.</param>
    /// <param name="y">The amount to rotate along the Y-axis.</param>
    /// <param name="z">The amount to rotate along the Z-axis.</param>
    void Rotate( float x, float y, float z );

    /// <summary>
    /// Rotates this entity.
    /// </summary>
    /// <param name="amount">The amount to rotate by.</param>
    void Rotate( const DirectX::XMFLOAT3& amount );

    /// <summary>
    /// Scales this entity.
    /// </summary>
    /// <param name="x">The amount to scale along the X-axis.</param>
    /// <param name="y">The amount to scale along the Y-axis.</param>
    /// <param name="z">The amount to scale along the Z-axis.</param>
    void Scale( float x, float y, float z );

    /// <summary>
    /// Scales this entity.
    /// </summary>
    /// <param name="amount">The amount to scales by.</param>
    void Scale( const DirectX::XMFLOAT3& amount );

    /// <summary>
    /// Sets this entity's position.
    /// </summary>
    /// <param name="x">The new X component of the position.</param>
    /// <param name="y">The new Y component of the position.</param>
    /// <param name="z">The new Z component of the position.</param>
    void SetPosition( float x, float y, float z );

    /// <summary>
    /// Sets this entity's position.
    /// </summary>
    /// <param name="position">The new position.</param>
    void SetPosition( const DirectX::XMFLOAT3& position );

    /// <summary>
    /// Sets this entity's rotation.
    /// </summary>
    /// <param name="x">The new X component of the rotation.</param>
    /// <param name="y">The new Y component of the rotation.</param>
    /// <param name="z">The new Z component of the rotation.</param>
    void SetRotation( float x, float y, float z );

    /// <summary>
    /// Sets this entity's rotation.
    /// </summary>
    /// <param name="rotation">The new rotation.</param>
    void SetRotation( const DirectX::XMFLOAT3& rotation );

    /// <summary>
    /// Sets this entity's scale.
    /// </summary>
    /// <param name="x">The new X component of the scale.</param>
    /// <param name="y">The new Y component of the scale.</param>
    /// <param name="z">The new Z component of the scale.</param>
    void SetScale( float x, float y, float z );

    /// <summary>
    /// Sets this entity's scale.
    /// </summary>
    /// <param name="scale">The new scale.</param>
    void SetScale( const DirectX::XMFLOAT3& scale );

    /// <summary>
    /// Copies another entity's data into this entity.
    /// </summary>
    Entity& operator=( const Entity& ) = default;

protected:
    /// <summary>
    /// Creates the matrices.
    /// </summary>
    void CreateMatrices( bool world, bool rotation );
};
