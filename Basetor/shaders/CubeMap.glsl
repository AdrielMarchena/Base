#type vertex
#version 400 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;

out vec3 TexCoords;
out vec4 v_Color;

uniform mat4 u_ViewProj;

void main()
{
	TexCoords = a_Position;
	v_Color = vec4(a_Color,1.0);
	vec4 pos = u_ViewProj * vec4(a_Position, 1.0);
	gl_Position = pos.xyww;
}

#type fragment
#version 400 core

out vec4 o_Color;

in vec3 TexCoords;
in vec4 v_Color;

uniform samplerCube skybox;

void main()
{
	o_Color = texture(skybox,TexCoords) * v_Color;
	
}