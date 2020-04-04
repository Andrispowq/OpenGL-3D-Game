#version 330

layout (location = 0) out vec4 outColour;

in vec3 position_FS;
in vec2 texture_FS;
in vec3 normal_FS;
in vec3 tangent_FS;

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

struct Light
{
	vec3 position;
	vec3 colour;
	vec3 intensity;
};

const int max_lights = 10;
const float PI = 3.141592653589793;
const float emissionFactor = 3.0;

uniform Light lights[max_lights];
uniform Material material;

uniform vec3 cameraPosition;
uniform float gamma;
uniform int highDetailRange;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

vec3 getColor(sampler2D map, vec3 alternateValue, vec2 texCoords)
{
	if(alternateValue.r == -1)
		return texture(map, texCoords).rgb;
	else
		return alternateValue;
}

vec3 getColor(sampler2D map, float alternateValue, vec2 texCoords)
{
	if(alternateValue == -1)
		return texture(map, texCoords).rgb;
	else
		return vec3(alternateValue);
}

void main()
{
	vec3 albedoColour = pow(getColor(material.albedoMap, material.colour, texture_FS), vec3(gamma));
	
	float metallic = getColor(material.metallicMap, material.metallic, texture_FS).r;
	float roughness = getColor(material.roughnessMap, material.roughness, texture_FS).r;
	float occlusion = getColor(material.occlusionMap, material.ambientOcclusion, texture_FS).r;
	
	vec3 emission = getColor(material.emissionMap, material.emission, texture_FS);
	
	float dist = length(cameraPosition - position_FS);
	
	vec3 normal = normalize(normal_FS);
	vec3 bumpNormal = vec3(0);
	
	if(dist < highDetailRange && material.usesNormalMap == 1)
	{
		float attenuation = clamp(-dist / highDetailRange + 1, 0, 1);
		
		vec3 bitangent = normalize(cross(tangent_FS, normal));
		mat3 tbn = mat3(tangent_FS, normal, bitangent);
		
		bumpNormal = 2 * texture(material.normalMap, texture_FS).rbg - 1;
		bumpNormal = normalize(bumpNormal);
		bumpNormal.xz *= attenuation;
		
		normal = normalize(tbn * bumpNormal);
	}
	
	vec3 N = normalize(normal);
	vec3 V = normalize(cameraPosition - position_FS);
	vec3 R = reflect(-V, N);
	
	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedoColour, metallic);
	
    vec3 Lo = vec3(0);
	
    // reflectance equation
	for(int i = 0; i < max_lights; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(lights[i].position - position_FS);
        vec3 H = normalize(V + L);
        float dist = length(lights[i].position - position_FS);
        float attenuation = 1 / pow(dist, 2);
        vec3 radiance = lights[i].colour * lights[i].intensity * attenuation; 
        
        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlickRoughness(max(dot(H, V), 0), F0, roughness);
        
        vec3 kS = F;
        vec3 kD = vec3(1) - kS;
        kD *= 1 - metallic;
        
        vec3 numerator = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0) * max(dot(N, L), 0);
        vec3 specular = numerator / max(denominator, 0.001);
            
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0);
        Lo += (kD * albedoColour / PI + specular) * radiance * NdotL;
    }

	vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0), F0, roughness);

	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - metallic;

	vec3 irradiance = vec3(0.23, 0.78, 0.88);//texture(irradianceMap, normal_FS).rgb;	
	vec3 diffuse = irradiance * albedoColour;
	
	/*const float MAX_REFLECTION_LOD = 4.0;
	vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
	vec2 envBRDF = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
	vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);*/

	vec3 ambient = (kD * diffuse + vec3(0)) * occlusion;

	vec3 colour = ambient + Lo + max(emission * emissionFactor, 0.0);
	
	colour /= colour + vec3(1.0);
	colour = pow(colour, vec3(1.0 / gamma));
	
	outColour = vec4(colour, 1);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
	
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
	
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1 - F0) * pow(1.0 - cosTheta, 5.0);
} 