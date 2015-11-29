#include "Math.hpp"

using namespace DirectX;

// Decompose a quaternion
void Math::DecomposeQuat( const DirectX::XMFLOAT4& quat, DirectX::XMFLOAT3& rotation )
{
    // Get the normalized quaternion
    XMFLOAT4 normalizedQuat;
    XMStoreFloat4( &normalizedQuat, XMQuaternionNormalize( XMLoadFloat4( &quat ) ) );

    // Get some helper variables
    const float& q0 = normalizedQuat.x;
    const float& q1 = normalizedQuat.y;
    const float& q2 = normalizedQuat.z;
    const float& q3 = normalizedQuat.w;
    float& pitch = rotation.x;
    float& yaw   = rotation.y;
    float& roll  = rotation.z;


    // Adapted from https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Conversion

    // Get the yaw (rotation around the Y axis)
    yaw = -asinf( 2 * ( q0 * q2 - q3 * q1 ) );

    // Get the pitch (rotation around the X axis)
    pitch = atan2f( 2.0f * ( q0 * q3 + q1 * q2 ),
                    1.0f - 2.0f * ( q2 * q2 + q3 * q3 ) );
    pitch = XM_PI - pitch;

    // If the pitch is directly up or down, then roll is 0
    if ( fabsf( fabsf( pitch ) - XM_PIDIV2 ) < 0.001f )
    {
        roll = 0.0f;
    }
    else
    {
        // Get the roll (rotation around the Z axis)
        roll = atan2f( 2.0f * ( q0 * q1 + q2 * q3 ),
                       1.0f - 2.0f * ( q1 * q1 + q2 * q2 ) );
    }
}
