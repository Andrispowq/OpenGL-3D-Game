#version 450
#extension GL_KHR_vulkan_glsl : enable

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

layout (location = 0) in vec2 texture_GS[];
layout (location = 1) in vec3 normal_GS[];

layout (location = 0) out vec3 position_FS;
layout (location = 1) out vec2 texture_FS;
layout (location = 2) out vec3 normal_FS;
layout (location = 3) out vec3 tangent_FS;

layout(set = 0, binding = 1, std140) uniform Material
{
	layout(offset = 00) vec3 colour;
	layout(offset = 16) vec3 emission;
	layout(offset = 32) int usesNormalMap;
	layout(offset = 36) float heightScale;
	layout(offset = 40) float metallic;
	layout(offset = 44) float roughness;
	layout(offset = 48) float occlusion;
} material;

layout(set = 0, binding = 2, std140) uniform Camera
{
	layout(offset = 0) mat4 m_view;
	layout(offset = 64) mat4 m_projection;
	layout(offset = 128) vec3 cameraPosition;
};

layout(set = 0, binding = 10, std140) uniform LightConditions
{
	layout(offset = 0) int highDetailRange;
	layout(offset = 4) int numberOfLights;
	layout(offset = 8) float gamma;
};

layout (set = 0, binding = 3) uniform sampler2D albedoMap;
layout (set = 0, binding = 4) uniform sampler2D displacementMap;
layout (set = 0, binding = 5) uniform sampler2D normalMap;
layout (set = 0, binding = 6) uniform sampler2D metallicMap;
layout (set = 0, binding = 7) uniform sampler2D roughnessMap;
layout (set = 0, binding = 8) uniform sampler2D occlusionMap;
layout (set = 0, binding = 9) uniform sampler2D emissionMap;

vec3 tangent;
vec3 displacement[3];

void calcTangent()
{
	vec3 v0 = gl_in[0].gl_Position.xyz;
	vec3 v1 = gl_in[1].gl_Position.xyz;
	vec3 v2 = gl_in[2].gl_Position.xyz;

    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;
	
	vec2 uv0 = texture_GS[0];
	vec2 uv1 = texture_GS[1];
	vec2 uv2 = texture_GS[2];

    vec2 deltaUV1 = uv1 - uv0;
	vec2 deltaUV2 = uv2 - uv0;
	
	float r = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	
	tangent = normalize((e1 * deltaUV2.y - e2 * deltaUV1.y) * r);
}

void main()
{
	for(int i = 0; i < displacement.length(); i++)
	{
		displacement[i] = vec3(0);
	}

	float dist = (distance(gl_in[0].gl_Position.xyz, cameraPosition)
			 + distance(gl_in[1].gl_Position.xyz, cameraPosition) 
			 + distance(gl_in[2].gl_Position.xyz, cameraPosition)) / 3;

	if(dist < highDetailRange && material.usesNormalMap == 1)
	{		
		calcTangent();
	
		if(material.heightScale > 0)
		{
			for(int i = 0; i < gl_in.length(); i++)
			{
				displacement[i] = vec3(0, 1, 0);
				float height = gl_in[i].gl_Position.y;
				
				float scale = texture(displacementMap, texture_GS[i]).r * material.heightScale;
				float attenuation = clamp(- distance(gl_in[i].gl_Position.xyz, cameraPosition) / (highDetailRange - 50) + 1, 0, 1);
				scale *= attenuation;
				
				displacement[i] *= scale;
			}
		}
	}
	
	for(int i = 0; i < gl_in.length(); i++)
	{
		vec4 worldPosition = gl_in[i].gl_Position + vec4(displacement[i], 0);
		gl_Position = m_projection * m_view * worldPosition;
		
		position_FS = worldPosition.xyz;
		texture_FS = texture_GS[i];
		normal_FS = normal_GS[i];
		tangent_FS = tangent;
		
		EmitVertex();
	}
	
	EndPrimitive();
}