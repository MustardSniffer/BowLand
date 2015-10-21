#pragma once
#include <DirectXMath.h>


class Camera
{
    static Camera* ActiveCamera;

public:
    Camera(float x, float y, float z);
    ~Camera();

    /// <summary>
    /// Gets the currently active camera.
    /// </summary>
    static Camera* GetActiveCamera();

    // Transformations
    void MoveRelative(float x, float y, float z);
    void MoveAbsolute(float x, float y, float z);
    void Rotate(float x, float y);

    // Updating
    void Update(float dt);
    void UpdateViewMatrix();
    void UpdateProjectionMatrix(float aspectRatio);

    // Getters
    DirectX::XMFLOAT3 GetPosition() const { return position; }
    DirectX::XMFLOAT4X4 GetView() const { return viewMatrix; }
    DirectX::XMFLOAT4X4 GetProjection() const { return projMatrix; }

private:
    // Camera matrices
    DirectX::XMFLOAT4X4 viewMatrix;
    DirectX::XMFLOAT4X4 projMatrix;

    // Transformations
    DirectX::XMFLOAT3 startPosition;
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 rotation;
    float xRotation;
    float yRotation;
};

