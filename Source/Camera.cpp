#include "Camera.hpp"
#include "Component.hpp"
#include "GameObject.hpp"
#include "Math.hpp"
#include "Time.hpp"
#include "Transform.hpp"
#include <Windows.h>

using namespace DirectX;

Camera* Camera::ActiveCamera = nullptr;
std::vector<Camera*> Camera::Cameras;

// Creates a camera at the specified position
Camera::Camera( GameObject* gameObj )
    : Component( gameObj )
    , _position( gameObj->GetTransform()->GetPosition() )
    , _rotation( gameObj->GetTransform()->GetRotation() )
{
    // Get the yaw, pitch, and roll from the transform
    XMFLOAT3 yawPitchRoll;
    Math::DecomposeQuat( _rotation, yawPitchRoll );
    xRotation = yawPitchRoll.x;
    yRotation = yawPitchRoll.y;

    nearClip = 0.1f;
    farClip = 100.0f;

    XMStoreFloat4x4( &viewMatrix, XMMatrixIdentity() );
    XMStoreFloat4x4( &projMatrix, XMMatrixIdentity() );

    ActiveCamera = this;
}

// Does nothing
Camera::~Camera()
{

}

// Returns the current active camera for projections
Camera* Camera::GetActiveCamera()
{
    return ActiveCamera;
}

// Gets a vector of all cameras currently in the scene
std::vector<Camera*> Camera::GetCurrentCameras()
{
    return Cameras;
}

// Adds a camera to the current scene
void Camera::AddCamera( Camera* cam )
{
    Cameras.push_back( cam );
}

// Moves the camera relative to its orientation
void Camera::MoveRelative( float x, float y, float z )
{
    // Rotate the desired movement vector
    XMVECTOR dir = XMVector3Rotate( XMVectorSet( x, y, z, 0 ), XMLoadFloat4( &_rotation ) );

    // Move in that direction
    XMStoreFloat3( &_position, XMLoadFloat3( &_position ) + dir );
}

// Moves the camera in world space (not local space)
void Camera::MoveAbsolute( float x, float y, float z )
{
    _position.x += x;
    _position.y += y;
    _position.z += z;
}

// Rotate on the X and/or Y axis
void Camera::Rotate( float x, float y )
{
    // Adjust the current rotation
    xRotation += x;
    yRotation += y;

    // Clamp the x between PI/2 and -PI/2
    xRotation = fmax( fmin( xRotation, XM_PIDIV2 ), -XM_PIDIV2 );

    // Recreate the quaternion
    XMStoreFloat4( &_rotation, XMQuaternionRotationRollPitchYaw( xRotation, yRotation, 0 ) );
}

// Sets this to be the active camera
void Camera::SetActive()
{
    ActiveCamera = this;
}

// Camera's update, which looks for key presses
void Camera::Update()
{
    // Set our transform's information
    Transform* transform = _gameObject->GetTransform();
    transform->SetPosition( _position );
    transform->SetRotation( _rotation );

    // Update our view matrix
    UpdateViewMatrix();
}

// Creates a new view matrix based on current position and orientation
void Camera::UpdateViewMatrix()
{
    // Calculate the forward and up vectors
    XMVECTOR rot = XMLoadFloat4( &_rotation );
    XMVECTOR dir = XMVector3Rotate( XMVectorSet( 0, 0, 1, 0 ), rot );
    XMVECTOR up  = XMVector3Rotate( XMVectorSet( 0, 1, 0, 0 ), rot );

    // Calculate and set the view matrix
    XMMATRIX view = XMMatrixLookToLH( XMLoadFloat3( &_position ), dir, up );
    XMStoreFloat4x4( &viewMatrix, XMMatrixTranspose( view ) );
}

// Updates the projection matrix
void Camera::UpdateProjectionMatrix( float aspectRatio )
{
    XMMATRIX P = XMMatrixPerspectiveFovLH(
        0.25f * 3.1415926535f,      // Field of View Angle
        aspectRatio,                // Aspect ratio
        nearClip,                   // Near clip plane distance
        farClip );                  // Far clip plane distance
    XMStoreFloat4x4( &projMatrix, XMMatrixTranspose( P ) ); // Transpose for HLSL!
}
