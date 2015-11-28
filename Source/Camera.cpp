#include "Camera.hpp"
#include "Component.hpp"
#include "GameObject.hpp"
#include "Time.hpp"
#include "Transform.hpp"
#include <Windows.h>

using namespace DirectX;

Camera* Camera::ActiveCamera = nullptr;
std::vector<Camera*> Camera::Cameras;

// Gets the yaw, pitch, and roll from a quaternion
static void GetYawPitchRollFromQuaternion( XMFLOAT4 quat, float* yaw, float* pitch, float* roll )
{
    // TODO - This is broken

    // Normalize the quaternion
    XMStoreFloat4( &quat, XMQuaternionNormalize( XMLoadFloat4( &quat ) ) );
    const float& qx = quat.x;
    const float& qy = quat.y;
    const float& qz = quat.z;
    const float& qw = quat.w;

    // From http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
    
    *pitch = asinf( 2 * ( qx * qy + qz * qw ) );

    // Test if we're rotated towards the north pole
    if ( qx *  qy + qz * qw == 0.5f )
    {
        *yaw  = 2 * atan2f( qx, qw );
        *roll = 0.0f;

        return;
    }
    // Test if we're rotated towards the south pole
    else if ( qx *  qy + qz * qw == -0.5f )
    {
        *yaw  = -2 * atan2f( qx, qw );
        *roll = 0.0f;

        return;
    }

    *yaw  = atan2f( 2 * ( qy * qw - qx * qz ), 1 - 2 * ( qy * qy - qz * qz ) );
    *roll = atan2f( 2 * ( qx * qw - qy * qz ), 1 - 2 * ( qx * qx - qz * qz ) );
}

// Creates a camera at the specified position
Camera::Camera( GameObject* gameObj )
    : Component( gameObj )
    , position( gameObj->GetTransform()->_position )
    , rotation( gameObj->GetTransform()->_rotation )
{
    _isDrawable = false;

    // Get the yaw, pitch, and roll from the transform
    float roll = 0.0f;
    GetYawPitchRollFromQuaternion( rotation, &yRotation, &xRotation, &roll );

    nearClip = 0.1f;
    farClip = 100.0f;

    XMStoreFloat4x4( &viewMatrix, XMMatrixIdentity() );
    XMStoreFloat4x4( &projMatrix, XMMatrixIdentity() );

    ActiveCamera = this;
}

// Does nothing
Camera::~Camera(){

}

// Returns the current active camera for projections
Camera* Camera::GetActiveCamera()
{
    return ActiveCamera;
}

// Gets a vector of all cameras currently in the scene
std::vector<Camera*> Camera::GetCurrentCameras(){
    return Cameras;
}

// Adds a camera to the current scene
void Camera::AddCamera(Camera* cam){
    Cameras.push_back(cam);
}

// Moves the camera relative to its orientation
void Camera::MoveRelative(float x, float y, float z)
{
    // Rotate the desired movement vector
    XMVECTOR dir = XMVector3Rotate(XMVectorSet(x, y, z, 0), XMLoadFloat4(&rotation));
    
    // Move in that direction
    XMStoreFloat3(&position, XMLoadFloat3(&position) + dir);
}

// Moves the camera in world space (not local space)
void Camera::MoveAbsolute(float x, float y, float z)
{
    position.x += x;
    position.y += y;
    position.z += z;
}

// Rotate on the X and/or Y axis
void Camera::Rotate(float x, float y)
{
    // Adjust the current rotation
    xRotation += x;
    yRotation += y;

    // Clamp the x between PI/2 and -PI/2
    xRotation = fmax(fmin(xRotation, XM_PIDIV2), -XM_PIDIV2);

    // Recreate the quaternion
    XMStoreFloat4(&rotation, XMQuaternionRotationRollPitchYaw(xRotation, yRotation, 0));
}

// Sets this to be the active camera
void Camera::SetActive(){
    ActiveCamera = this;
}

// Camera's update, which looks for key presses
void Camera::Update()
{
    UpdateViewMatrix();
}

// Creates a new view matrix based on current position and orientation
void Camera::UpdateViewMatrix()
{
    // Rotate the standard "forward" matrix by our rotation
    // This gives us our "look direction"
    XMVECTOR dir = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), XMLoadFloat4(&rotation));

    XMMATRIX view = XMMatrixLookToLH(
        XMLoadFloat3(&position),
        dir,
        XMVectorSet(0, 1, 0, 0));

    XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(view));
}

// Updates the projection matrix
void Camera::UpdateProjectionMatrix(float aspectRatio)
{
    XMMATRIX P = XMMatrixPerspectiveFovLH(
        0.25f * 3.1415926535f,		// Field of View Angle
        aspectRatio,				// Aspect ratio
        nearClip,						// Near clip plane distance
        farClip);					// Far clip plane distance
    XMStoreFloat4x4(&projMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}
