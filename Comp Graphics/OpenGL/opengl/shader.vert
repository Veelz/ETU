#version 330 core
precision highp float;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;
out vec4 FragPosLightSpace;
out vec3 ModelFragPos;

uniform mat4 modelViewProjection;
uniform mat4 modelView;
uniform mat4 model;
uniform mat4 lightSpace;

void main()
{
	gl_Position = modelViewProjection * vec4(position, 1.0f);	
	Normal = mat3(transpose(inverse(modelView))) * normal;
	FragPos = vec3(modelView * vec4(position, 1.0f));
	ModelFragPos = vec3(model * vec4(position, 1.0f));
	TexCoord = texCoord;
	FragPosLightSpace = lightSpace * vec4(FragPos, 1.0f);
	FragPosLightSpace.xyz *= 0.5f;
	FragPosLightSpace.xyz += FragPosLightSpace.w * 0.5f;
}