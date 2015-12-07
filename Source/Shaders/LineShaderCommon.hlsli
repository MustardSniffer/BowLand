/// <summary>
/// Our constant buffer for external data.
/// </summary>
cbuffer __extern__ : register( b0 )
{
    matrix World;
    matrix Projection;
    float4 LineColor;
};

/// <summary>
/// Defines the information that is passed from the program to the vertex shader.
/// </summary>
struct ProgramToVertex
{
    float2 Position : SV_POSITION;
};

/// <summary>
/// Defines the information that is passed from the vertex shader to the pixel shader.
/// </summary>
struct VertexToPixel
{
    float4 Position : SV_POSITION;
};
