#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Component.hpp"


class Camera : public Component
{
    static Camera* ActiveCamera;
    static std::vector<Camera*> Cameras;

public:
    Camera(GameObject* gameObj);
    ~Camera();

    /// <summary>
    /// Gets the currently active camera.
    /// </summary>
    static Camera* GetActiveCamera();

    /// <summary>
    /// Returns a list of all cameras in scene
    /// </summary>
    static std::vector<Camera*> GetCurrentCameras();
    static void AddCamera(Camera* cam);

    // Transformations
    void MoveRelative(float x, float y, float z);
    void MoveAbsolute(float x, float y, float z);
    void Rotate(float x, float y);

    // Updating
    void Update() override;
    void UpdateViewMatrix();
    void UpdateProjectionMatrix(float aspectRatio);

    // Mutators
    void SetNearClip(const float nClip) { nearClip = nClip; }
    void SetFarClip(const float fClip){ farClip = fClip; }
    void SetActive();

    // Getters
    DirectX::XMFLOAT3 GetPosition() const { return _position; }
    DirectX::XMFLOAT4X4 GetView() const { return viewMatrix; }
    DirectX::XMFLOAT4X4 GetProjection() const { return projMatrix; }

private:
    // Camera matrices
    DirectX::XMFLOAT4X4 viewMatrix;
    DirectX::XMFLOAT4X4 projMatrix;

    float nearClip;
    float farClip;

    // Transformations
    DirectX::XMFLOAT3 _position;
    DirectX::XMFLOAT4 _rotation;
    float xRotation;
    float yRotation;
};

