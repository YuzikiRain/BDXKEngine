struct Vertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

//当前材质球信息
cbuffer MaterialInfo : register(b0)
{
float4 float0_3;
float4 float4_7;
float4 vector0;
float4 vector1;
float4 vector2;
float4 vector3;
float4 vector4;
float4 vector5;
float4x4 matrix0;
float4x4 matrix1;
float4x4 matrix2;
float4x4 matrix3;
};
Texture2D Texture2D0 : register(t0);
SamplerState SamplerState0 : register(s0);
Texture2D Texture2D1 : register(t1);
SamplerState SamplerState1 : register(s1);
Texture2D Texture2D2 : register(t2);
SamplerState SamplerState2 : register(s2);
Texture2D Texture2D3 : register(t3);
SamplerState SamplerState3 : register(s3);
//当前世界信息
cbuffer WorldInfo : register(b1)
{
float4 Environment;
float4 Time;
};
TextureCube Skybox : register(t4);
SamplerState SkyboxSampler : register(s4);
//当前相机信息
cbuffer CameraInfo : register(b2)
{
float4x4 WorldToView;
float4x4 ViewToClip;
float4 CameraPosition;
}
//当前物体信息
cbuffer ObjectInfo : register(b3)
{
float4x4 ObjectToWorld;
}
//当前灯光信息
cbuffer LightInfo : register(b4)
{
float4 LightPosition;
float4 LightNormal;
float4 LightColor;
float4x4 WorldToLightView;
float4x4 ViewToLightClip;
int LightOrder;
int LightType;
}
Texture2D ShadowMap : register(t5);
SamplerState ShadowMapSampler : register(s5);
TextureCube ShadowMapCube : register(t6);
SamplerState ShadowMapCubeSampler : register(s6);



float3 ObjectToWorldVector(float3 worldVector)
{
    float4x4 ObjectToWorldVector = ObjectToWorld;
    ObjectToWorldVector._14_24_34_44 = float4(0, 0, 0, 1);

    return (float3)mul(ObjectToWorldVector, float4(worldVector, 1));
}
float4 ObjectToClipPos(float3 objectPosition)
{
    float4 svPosition = float4(objectPosition, 1);
    svPosition = mul(ObjectToWorld, svPosition);
    svPosition = mul(WorldToView, svPosition);
    svPosition = mul(ViewToClip, svPosition);
    return svPosition;
}
float2 ClipToUVPos(float4 clipPosition)
{
    float2 result = clipPosition.xy / clipPosition.w;
    result.x = result.x / 2 + 0.5f;
    result.y = result.y / -2 + 0.5f;
    return result;
}
float DecodeShadowMap(float3 worldPosition)
{
    float depth;
    float depthOfMap;

    if (LightType == 1)
    {
        depthOfMap = ShadowMapCube.Sample(ShadowMapCubeSampler, worldPosition - LightPosition.xyz).z;
        depth = distance(worldPosition, LightPosition.xyz);
    }
    else
    {
        float4 view = mul(WorldToLightView, float4(worldPosition, 1));
        float4 clip = mul(ViewToLightClip, view);
        depthOfMap = ShadowMap.Sample(ShadowMapSampler, ClipToUVPos(clip)).z;
        depth = view.z;
    }

    if (depth > depthOfMap + 0.01f)
        return 0;
    else
        return 1;
}
//float3 DecodeSkybox(float3 worldPosition)
//{
//    return ;
//}