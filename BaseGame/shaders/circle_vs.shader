#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in vec2 a_MiddlePoint;
layout(location = 5) in vec3 a_Rad_Fill_Th;

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out vec3 v_Pos;
out vec2 v_MiddlePoint;
out vec3 v_Rad_Fill_Th;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_Pos = a_Position;
	v_MiddlePoint = a_MiddlePoint;
	v_Rad_Fill_Th = a_Rad_Fill_Th;

	gl_Position = u_ViewProj * u_Transform * vec4(a_Position, 1.0);
}