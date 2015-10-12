#include "GameObject.hpp"
#include "Component.hpp"

using namespace DirectX;

// Create a new game object
GameObject::GameObject()
    : _parent( nullptr )
{
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
    // TODO - Input values from transform or get the transform's world matrix directly
    XMMATRIX rotation = XMMatrixRotationX( 0 )
                      * XMMatrixRotationY( 0 )
                      * XMMatrixRotationZ( 0 );
    XMMATRIX world    = XMMatrixScaling( 1, 1, 1 )
                      * rotation
                      * XMMatrixTranslation( 0, 0, 0 );
    
    // Check if we have a parent, then combine the matrices if necessary
    if ( _parent )
    {
        XMFLOAT4X4 parentWorld = _parent->GetWorldMatrix();
        world = XMLoadFloat4x4( &parentWorld ) * world;
    }

    XMFLOAT4X4 returnWorld;
    XMStoreFloat4x4( &returnWorld, world );
    return returnWorld;
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
