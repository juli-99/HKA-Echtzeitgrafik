#version 330 core

struct PointLight {
vec3 pos; 
vec3 color;
float constant; 
float lin;	
float quad;
};

layout (location = 0) out vec4 out_color;


in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

uniform vec3 u_viewPos; // Camera Position
uniform sampler2D u_image;
uniform bool u_enablePointLight;
uniform PointLight u_Light;


void main() {
	float ambientStr = 0.1;
	float specularStr = 0.5;

 
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(u_Light.pos - fragPos);

	vec3 viewDir = normalize(u_viewPos - fragPos);

	float distance = length(u_Light.pos - fragPos);
	float attenuation = 1.0 / (u_Light.constant + u_Light.lin * distance + u_Light.quad * pow(distance, 2.0));

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 ambient = ambientStr * u_Light.color * attenuation;

	vec3 spec = vec3(0.0);

	vec3 diffuse = diff * u_Light.color * attenuation;

	if (diff > 0.0)
	{
		vec3 reflectDir = reflect(-lightDir, norm);
		spec = specularStr * pow(max(dot(viewDir, reflectDir), 0.0001), 32) * u_Light.color * attenuation;
	}

	vec3 phong = (ambient + diffuse + spec);

	vec4 texture = texture(u_image, texCoord);
	vec3 result = texture.rgb;
	if (u_enablePointLight)
		 result *= phong;

	out_color = vec4(result, texture.a);
}