#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_Radius;
layout(location = 5) in float a_Thick;
layout(location = 6) in float a_Fade;
layout(location = 7) in int a_EntityID;

uniform mat4 u_ViewProj;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out vec3 v_Pos;
out float v_Radius;
out float v_Thick;
out float v_Fade;

layout(location = 3) out flat int v_EntityID;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_Pos = a_Position;

	v_Radius = a_Radius;
	v_Fade = a_Fade;
	v_Thick = a_Thick;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProj * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core
#define MAX_TEXTURES_SLOTS 16

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in vec3 v_Pos;
in float v_Radius;
in float v_Thick;
in float v_Fade;

layout(location = 3) in flat int v_EntityID;

uniform sampler2D u_Textures[MAX_TEXTURES_SLOTS];

void main()
{
	int index = int(v_TexIndex);
	vec4 tmp_Color = texture(u_Textures[index], v_TexCoord) * v_Color;
	
	//vec2 uv = v_Pos.xy * 2.0 - v_Radius;

	float vt = v_Thick * (v_Radius * 0.5);

	float dist = (v_Radius * 0.5) - length(v_Pos.xy);

	vec4 col = vec4(smoothstep(0.0,v_Fade,dist));
	col *= vec4(smoothstep(vt + v_Fade,vt,dist));

	tmp_Color *= col;
	o_Color = tmp_Color;

	o_EntityID = v_EntityID;
}