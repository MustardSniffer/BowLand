#include "GameObject.hpp"
#include "Component.hpp"
#include "Transform.hpp"

using namespace DirectX;

// Create a new game object
GameObject::GameObject()
    : _parent( nullptr )
{
	Transform* t = AddComponent<Transform>();
}

// Destroy this game object
GameObject::~GameObject()
{
    _parent = nullptr;
}

// Add  a child to this game object
GameObject* GameObject::AddChild()
{
    auto child = std::make_shared<GameObject>();
    _children.push_back( child );
    child->_parent = this;
    return child.get();
}

// Get this game object's world matrix
XMFLOAT4X4 GameObject::GetWorldMatrix() const
{
    return worldMat;
}

void GameObject::UpdateWorldMatrix(){

	const Transform* t = GetComponent<Transform>();
	XMFLOAT3 pos = t->GetPosition();
	XMFLOAT3 rot = t->GetRotation();
	XMFLOAT3 sca = t->GetScale();

	XMMATRIX POS = XMMatrixTranslation(pos.x, pos.y, pos.z);
	XMMATRIX SCALE = XMMatrixScaling(sca.x, sca.y, sca.z);
	XMMATRIX ROT = XMMatrixRotationX(rot.x) * XMMatrixRotationY(rot.y) * XMMatrixRotationZ(rot.z);
	XMMATRIX WORLD = SCALE * ROT * POS;
	
	// Check if we have a parent, then combine the matrices if necessary
	if (_parent)
	{
		XMFLOAT4X4 parentWorld = _parent->GetWorldMatrix();
		WORLD = XMLoadFloat4x4(&parentWorld) * WORLD;
	}

	XMStoreFloat4x4(&worldMat, XMMatrixTranspose(WORLD));
	dirtyWorldMatrix = false;
}

bool GameObject::isWorldMatrixDirty() const{
	return dirtyWorldMatrix;
}

void GameObject::SetWorldMatrixDirty(){
	dirtyWorldMatrix = true;
}

// Update all components
void GameObject::Update( const GameTime& gameTime )
{
    // Update all of our components
    for ( auto iter = _components.begin(); iter != _components.end(); ++iter )
    {
        std::shared_ptr<Component>& component = iter->second;
        if ( component->IsEnabled() )
        {
            component->Update( gameTime );
        }
    }

    // Perform the late update on all of our components
    for ( auto iter = _components.begin(); iter != _components.end(); ++iter )
    {
        std::shared_ptr<Component>& component = iter->second;
        if ( component->IsEnabled() && component->UsesLateUpdate() )
        {
            component->LateUpdate( gameTime );
        }
    }

    // Now update all of our children
    for ( auto iter = _children.begin(); iter != _children.end(); ++iter )
    {
        iter->get()->Update( gameTime );
    }
}

// Draw all components
void GameObject::Draw( const GameTime& gameTime )
{
    // Draw all of our components
    for ( auto iter = _components.begin(); iter != _components.end(); ++iter )
    {
        std::shared_ptr<Component>& component = iter->second;
        if ( component->IsDrawable() )
        {
            component->Draw( gameTime );
        }
    }

    // Now update all of our children
    for ( auto iter = _children.begin(); iter != _children.end(); ++iter )
    {
        // TODO - Add some kind of renderer to sort these by depth
        iter->get()->Draw( gameTime );
    }
}
