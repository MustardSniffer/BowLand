#pragma once

#include <DirectXMath.h>

/// <summary>
/// Defines a static math class.
/// </summary>
class Math
{
    // Hide instance methods
    Math() = delete;
    ~Math() = delete;
    Math( const Math& ) = delete;
    Math( Math&& ) = delete;
    Math& operator=( const Math& ) = delete;
    Math& operator=( Math&& ) = delete;

public:
    /// <summary>
    /// Decomposes a quaternion into yaw, pitch, and roll.
    /// </summary>
    /// <param name="quat">The quaternion.</param>
    /// <param name="rotation">The rotation to fill. X will be pitch, Y will be yaw, Z will be roll.</param>
    static void DecomposeQuat( const DirectX::XMFLOAT4& quat, DirectX::XMFLOAT3& rotation );
};
