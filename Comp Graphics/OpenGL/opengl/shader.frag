#version 330 core
precision highp float;

struct Material {
    vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
    vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
in vec4 FragPosLightSpace;
in vec3 ModelFragPos;

out vec4 outColor;

uniform Material material;
uniform Light gLight;
uniform vec3 aLight_position;
uniform vec3 aLight_color_1;
uniform vec3 aLight_color_2;
uniform sampler2D shadowMap;
uniform sampler2D textureBox;
uniform samplerCube environmentMap;
uniform mat4 inverseViewNormalMatrix;
uniform vec3 color;
uniform int texture_type;
uniform vec3 viewSpaceLightDir;
uniform float spotOuterAngle;
uniform float spotInnerAngle;

vec3 calculate_ambient_light() {

	return gLight.ambient * material.ambient;
}

vec3 calculate_diffuse_light(vec3 normal, vec3 lightDir) {
	float diff = max(dot(normal, lightDir), 0.0f);

	return gLight.diffuse * diff * material.diffuse;
}

vec3 calculate_specular_light(vec3 normal, vec3 lightDir, vec3 viewDir) {
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	return gLight.specular * spec * material.specular;
}

vec3 calculate_diffuse_color(vec3 normal, vec3 viewDir) {
	if (texture_type == 1) {
		return texture(textureBox, TexCoord).xyz;
	}
	else if (texture_type == 2) {
		vec3 R = (inverseViewNormalMatrix * vec4(reflect(-viewDir, normal), 1.0f)).xyz;

		return texture(environmentMap, R).xyz;
	}
	else {
		return color;
	}
}

float calculate_shadow() {
	float closestDepth = texture(shadowMap, FragPosLightSpace.xy / FragPosLightSpace.w).x; 
    float currentDepth = FragPosLightSpace.z / FragPosLightSpace.w;
	return ((currentDepth < closestDepth) ? 1.0f : 0.0f);
}

float exp_step(float edge0, float edge1, float x) {
	if (x < edge0)
		return 0.0f;
	if (x > edge1)
		return 1.0f;
	return (-exp(x) + exp(edge0)) / (-exp(edge1) + exp(edge0));
}

void main() {	
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(gLight.position - FragPos);
	vec3 viewDir = normalize(-FragPos);
	// ambient light
	vec3 ambient = vec3(0.0f);
	// diffuse light
	vec3 diffuse = vec3(0.0f);
	// specular light
	vec3 specular = vec3(0.0f);

	ambient = calculate_ambient_light();
	diffuse = calculate_diffuse_light(norm, lightDir);
	specular = calculate_specular_light(norm, lightDir, viewDir);
	
	vec3 diffuseColor = calculate_diffuse_color(norm, viewDir);

	float dst = distance(aLight_position, ModelFragPos);
	float edge0 = 0.54f;
	float edge1 = 0.70f;
	float edge2 = 0.8f;

	if (dst < edge1) {
		float u = exp_step(edge0, edge1, dst);
		diffuseColor = mix(aLight_color_1, 0.6 * aLight_color_2, u);
	}
	else if (dst < edge2) {
		float u = exp_step(edge1, edge2, dst);
		diffuseColor = mix(0.6 * aLight_color_2, diffuseColor, u);
	}

	// shadow calculation
	float shadowCoeff = calculate_shadow();

	float angle = dot(lightDir, -viewSpaceLightDir);
	float spotAttenuation = smoothstep(spotOuterAngle, spotInnerAngle, angle);
		
	vec3 result = (ambient + spotAttenuation * shadowCoeff * (diffuse + specular)) * diffuseColor;

	outColor = vec4(result, 1.0f);
}