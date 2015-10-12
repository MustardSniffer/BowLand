#pragma once
#include "DirectXMath.h"
#include "GameObject.hpp"
#include "Transform.hpp"
#include "Component.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

class MeshRenderer : public Component
{
protected:
	std::shared_ptr<Material> _material;
	std::shared_ptr<Mesh> _mesh;

public:
	/// <summary>
	/// Creates a new mesh renderer.
	/// </summary>
	/// <param name="gameObj">The GameObject this renderer belongs to.</param>
	MeshRenderer(GameObject* gameObj);

	/// <summary>
	///	Destroys this object.
	/// </summary>
	~MeshRenderer();

	/// <summary>
	/// Copies the mesh and material of another mesh renderer.
	/// </summary>
	/// <param name="nRender">The renderer to copy from.</param>
	void CopyMeshRenderer(MeshRenderer* nRender);

	/// <summary>
	/// sets the mesh to render.
	/// </summary>
	/// <param name="nMesh">The mesh to be rendered.</param>
	void SetMesh(std::shared_ptr<Mesh> nMesh);

	/// <summary>
	/// Sets the material to render.
	/// </summary>
	/// <param name="nMaterial">The material to be rendered.</param>
	void SetMaterial(std::shared_ptr<Material> nMaterial);

	/// <summary>
	/// Returns the mesh of this renderer.
	/// </summary>
	std::shared_ptr<Mesh> GetMesh();

	/// <summary>
	/// Returns the material of this renderer.
	/// </summary>
	std::shared_ptr<Material> GetMaterial();

	/// <summary>
	/// Updates the renderer.
	/// </summary>
	/// <param name="gameTime">Reference to the current game time</param>
	void Update(const GameTime& gameTime) override;

	/// <summary>
	/// Draws the material and mesh of this renderer.
	/// </summary>
	/// <param name="gameTime">Reference to the current game time.</param>
	void Draw(const GameTime& gameTime) override;
};

