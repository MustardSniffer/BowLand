#include "Player.hpp"
#include "GameObject.hpp"
#include <algorithm>

#define CLAMP(val, mn, mx) std::min( mx, std::max( mn, val ) )

const int Player::PlayerMaxHealth = 5;

// Creates a new player component
Player::Player( GameObject* gameObject )
    : Component( gameObject )
    , _bodyCollider( nullptr )
    , _crossbowParent( nullptr )
    , _headCollider( nullptr )
    , _health( PlayerMaxHealth )
{
}

// Destroys this player component
Player::~Player()
{
    _bodyCollider = nullptr;
    _crossbowParent = nullptr;
    _headCollider = nullptr;
    _health = 0;
}

// Gets this player's body collider
const Collider* Player::GetBodyCollider() const
{
    return _bodyCollider;
}

// Gets this player's head collider
const Collider* Player::GetHeadCollider() const
{
    return _headCollider;
}

// Gets this player's position
DirectX::XMFLOAT3 Player::GetPosition() const
{
    return _gameObject->GetTransform()->GetPosition();
}

// Checks to see if this player is dead
bool Player::IsDead() const
{
    return _health == 0;
}

// Modifies this player's health by the given damage
void Player::ModifyHealth( int damage )
{
    _health = CLAMP( _health + damage, 0, PlayerMaxHealth );
}

// Sets the direction the crossbow should be pointing
void Player::SetCrossbowDirection( const DirectX::XMFLOAT2& direction )
{
}

// Updates this player component
void Player::Update()
{
}
