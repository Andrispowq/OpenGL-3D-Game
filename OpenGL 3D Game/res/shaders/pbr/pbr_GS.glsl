#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

struct Material
{
	sampler2D albedoMap;
	sampler2D normalMap;
	sampler2D displacementMap;
	sampler2D metallicMap;
	sampler2D roughnessMap;
	sampler2D occlusionMap;
	sampler2D emissionMap;
	
	vec3 colour;
	int usesNormalMap;
	float heightScale;
	float metallic;
	float roughness;
	float ambientOcclusion;
	vec3 emission;
};

in vec2 texture_GS[];
in vec3 normal_GS[];

out vec3 position_FS;
out vec2 texture_FS;
out vec3 normal_FS;
out vec3 tangent_FS;

uniform Material material;

uniform mat4 m_view;
uniform mat4 m_projection;

uniform vec3 cameraPosition;
uniform int highDetailRange;

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
				
				float scale = texture(material.displacementMap, texture_GS[i]).r * material.heightScale;
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