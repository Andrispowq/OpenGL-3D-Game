#version 430

layout (location = 0) out vec4 out_colour;

layout (location = 0) in vec3 position_FS;

uniform samplerCube environmentMap;

const float PI = 3.141592653589793;

void main()
{		
    vec3 normal = normalize(position_FS);
	
	vec3 irradiance = vec3(0);
	
	vec3 up = vec3(0, 1, 0);
	vec3 right = cross(up, normal);
	up = cross(normal, right);

	float sampleDelta = 0.025;
	float nrSamples = 0.0;
	
	for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
		for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
		{
			// spherical to cartesian (in tangent space)
			vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
			// tangent space to world
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal; 

			irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples++;
		}
	}
	
	irradiance = PI * irradiance * (1.0 / float(nrSamples));
    
    out_colour = vec4(irradiance, 1.0);
}