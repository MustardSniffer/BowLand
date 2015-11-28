#include "GameObject.hpp"
#include "Component.hpp"
#include "Transform.hpp"

using namespace DirectX;

// Create a new game object
GameObject::GameObject( const std::string& name, ID3D11Device* device, ID3D11DeviceContext* deviceContext )
    : _name( name )
    , _parent( nullptr )
    , _device( nullptr )
    , _deviceContext( nullptr )
    , _transform( nullptr )
{
    UpdateD3DResource( _device, device );
    UpdateD3DResource( _deviceContext, deviceContext );
    _transform = AddComponent<Transform>();
}

// Destroy this game object
GameObject::~GameObject()
{
    _parent = nullptr;
    _transform = nullptr;
    ReleaseMacro( _device );
    ReleaseMacro( _deviceContext );
}

// Add  a child to this game object
GameObject* GameObject::AddChild( const std::string& name )
{
    // Create the child
    auto child = std::make_shared<GameObject>( name, _device, _deviceContext );
    child->_parent = this;

    // Record the child
    _children.push_back( child );
    _childrenCache[ name ] = child;

    // Return the child
    return child.get();
}

// Get device
const ID3D11Device* GameObject::GetDevice() const
{
    return _device;
}

// Get device
ID3D11Device* GameObject::GetDevice()
{
    return _device;
}

// Get device context
const ID3D11DeviceContext* GameObject::GetDeviceContext() const
{
    return _deviceContext;
}

// Get device context
ID3D11DeviceContext* GameObject::GetDeviceContext()
{
    return _deviceContext;
}

// Get total number of children
size_t GameObject::GetChildCount() const
{
    return _children.size();
}

// Get a child by index
const GameObject* GameObject::GetChild( size_t index ) const
{
    if ( index >= _children.size() )
    {
        return nullptr;
    }
    return _children[ index ].get();
}

// Get a child by index
GameObject* GameObject::GetChild( size_t index )
{
    if ( index >= _children.size() )
    {
        return nullptr;
    }
    return _children[ index ].get();
}

// Get a child by name
const GameObject* GameObject::GetChildByName( const std::string& name ) const
{
    auto search = _childrenCache.find( name );
    if ( search == _childrenCache.end() )
    {
        return nullptr;
    }
    return search->second.get();
}

// Get a child by name
GameObject* GameObject::GetChildByName( const std::string& name )
{
    auto search = _childrenCache.find( name );
    if ( search == _childrenCache.end() )
    {
        return nullptr;
    }
    return search->second.get();
}

// Get our name
std::string GameObject::GetName() const
{
    return _name;
}

// Get the transform
const Transform* GameObject::GetTransform() const
{
    return _transform;
}

// Get the transform
Transform* GameObject::GetTransform()
{
    return _transform;
}

// Get this game object's world matrix
XMFLOAT4X4 GameObject::GetWorldMatrix() const
{
    return worldMat;
}

// Update world matrix
void GameObject::UpdateWorldMatrix()
{
    XMFLOAT4X4 world4x4 = _transform->GetWorldMatrix();
    XMMATRIX world = XMLoadFloat4x4( &world4x4 );
    
    // Check if we have a parent, then combine the matrices if necessary
    if (_parent)
    {
        XMFLOAT4X4 parentWorld = _parent->GetWorldMatrix();
        world = XMLoadFloat4x4( &parentWorld ) * world;
    }

    XMStoreFloat4x4( &worldMat, world );
    dirtyWorldMatrix = false;
}

// Check if the world matrix needs to be updated
bool GameObject::isWorldMatrixDirty() const{
    return dirtyWorldMatrix;
}

// Set that the world matrix needs to be updated
void GameObject::SetWorldMatrixDirty(){
    dirtyWorldMatrix = true;
}

// Update all components
void GameObject::Update()
{
    // Update all of our components
    for ( auto& pair : _components )
    {
        auto& component = pair.second;
        if ( component->IsEnabled() )
        {
            component->Update();
        }
    }

    // Perform the late update on all of our components
    for ( auto& pair : _components )
    {
        auto& component = pair.second;
        if ( component->IsEnabled() && component->UsesLateUpdate() )
        {
            component->LateUpdate();
        }
    }

    // Now update all of our children
    for ( auto& child : _children )
    {
        child->Update();
    }
}

// Draw all components
void GameObject::Draw()
{
    // If our world matrix is dirty, update it
    if ( dirtyWorldMatrix )
    {
        UpdateWorldMatrix();
    }

    // Draw all of our components
    for ( auto& pair : _components )
    {
        auto& component = pair.second;
        if ( component->IsEnabled() && component->IsDrawable() )
        {
            component->Draw();
        }
    }

    // Now update all of our children
    for ( auto& child : _children )
    {
        child->Draw();
    }
}
