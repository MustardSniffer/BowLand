#include "GameObject.hpp"
#include "Component.hpp"
#include "Transform.hpp"

using namespace DirectX;

// Create a new game object
GameObject::GameObject( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
    : _parent( nullptr )
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
GameObject* GameObject::AddChild()
{
    auto child = std::make_shared<GameObject>( _device, _deviceContext );
    _children.push_back( child );
    child->_parent = this;
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
void GameObject::UpdateWorldMatrix(){

    XMFLOAT3 pos = _transform->GetPosition();
    XMFLOAT3 rot = _transform->GetRotation();
    XMFLOAT3 sca = _transform->GetScale();

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

// Check if the world matrix needs to be updated
bool GameObject::isWorldMatrixDirty() const{
    return dirtyWorldMatrix;
}

// Set that the world matrix needs to be updated
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
    // If our world matrix is dirty, update it
    if ( dirtyWorldMatrix )
    {
        UpdateWorldMatrix();
    }

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
