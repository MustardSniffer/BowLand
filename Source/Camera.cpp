#include "Camera.hpp"

using namespace DirectX;

// -- Left-handed coordinate system ----------------------
static const XMFLOAT3 VEC_RIGHT   (  1.0f,  0.0f,  0.0f );
static const XMFLOAT3 VEC_LEFT    ( -1.0f,  0.0f,  0.0f );
static const XMFLOAT3 VEC_UPWARD  (  0.0f,  1.0f,  0.0f );
static const XMFLOAT3 VEC_DOWNWARD(  0.0f, -1.0f,  0.0f );
static const XMFLOAT3 VEC_FORWARD (  0.0f,  0.0f,  1.0f );
static const XMFLOAT3 VEC_BACKWARD(  0.0f,  0.0f, -1.0f );
// -------------------------------------------------------


// Creates a new camera
Camera::Camera( float fovy, float aspect, float nearZ, float farZ )
    : _position( 0, 0, 0 )
    , _translation( 0, 0, 0 )
    , _viewDirection( 0, 0, 1 )
    , _rotation( 0, 0 )
{
    // Create the projection matrix
    UpdateProjection( fovy, aspect, nearZ, farZ );

    // Create the view matrix
    Update();
}

// Destroys this camera
Camera::~Camera()
{
    ZeroMemory( &_viewMatrix,    sizeof( XMFLOAT4X4 ) );
    ZeroMemory( &_projMatrix,    sizeof( XMFLOAT4X4 ) );
    ZeroMemory( &_position,      sizeof( XMFLOAT3 ) );
    ZeroMemory( &_viewDirection, sizeof( XMFLOAT3 ) );
    ZeroMemory( &_rotation,      sizeof( XMFLOAT2 ) );
}

// Get this camera's view matrix
const XMFLOAT4X4& Camera::GetViewMatrix() const
{
    return _viewMatrix;
}

// Get this camera's projection matrix
const XMFLOAT4X4& Camera::GetProjectionMatrix() const
{
    return _projMatrix;
}

// Move this camera
void Camera::Move( float x, float y, float z )
{
    _translation.x += x;
    _translation.y += y;
    _translation.z += z;
}

// Move this camera
void Camera::Move( const XMFLOAT3& amount )
{
    _translation.x += amount.x;
    _translation.y += amount.y;
    _translation.z += amount.z;
}

// Move this camera immediately
void Camera::MoveImmediate( float x, float y, float z )
{
    _position.x += x;
    _position.y += y;
    _position.z += z;
}

// Move this camera immediately
void Camera::MoveImmediate( const XMFLOAT3& amount )
{
    _position.x += amount.x;
    _position.y += amount.y;
    _position.z += amount.z;
}

// Rotate this camera
void Camera::Rotate( float x, float y )
{
    _rotation.x += x;
    _rotation.y += y;
}

// Rotate this camera
void Camera::Rotate( const XMFLOAT2& amount )
{
    _rotation.x += amount.x;
    _rotation.y += amount.y;
}

// Update this camera's view matrix
void Camera::Update()
{
    // Create the rotation matrix
    XMMATRIX rotMatrix = XMMatrixRotationX( _rotation.x )
                       * XMMatrixRotationY( _rotation.y );

    // Modify the rotate the translation then modify the position
    XMStoreFloat3( &_translation, XMVector3Transform( XMLoadFloat3( &_translation ), rotMatrix ) );
    _position.x += _translation.x;
    _position.y += _translation.y;
    _position.z += _translation.z;
    _translation.x = _translation.y = _translation.z = 0.0f;

    // Modify the view direction
    XMStoreFloat3( &_viewDirection, XMVector3Transform( XMLoadFloat3( &VEC_FORWARD ), rotMatrix ) );

    // Get the new up vector
    XMFLOAT3 up;
    XMStoreFloat3( &up, XMVector3Transform( XMLoadFloat3( &VEC_UPWARD ), rotMatrix ) );

    // Update the view matrix
    XMMATRIX view = XMMatrixLookToLH( XMLoadFloat3( &_position ),
                                      XMLoadFloat3( &_viewDirection ),
                                      XMLoadFloat3( &up ) );
    XMStoreFloat4x4( &_viewMatrix, XMMatrixTranspose( view ) );
}

// Update this camera's projection matrix
void Camera::UpdateProjection( float fovy, float aspect, float nearZ, float farZ )
{
    XMMATRIX proj = XMMatrixPerspectiveFovLH( fovy, aspect, nearZ, farZ );
    XMStoreFloat4x4( &_projMatrix, XMMatrixTranspose( proj ) );
}
