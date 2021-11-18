#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in int a_EntityID;

uniform mat4 u_ViewProj;

out vec3 v_Pos;
out vec4 v_Color;
flat out int v_EntityID;

void main()
{
	v_Color = a_Color;
	v_Pos = a_Position;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProj * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

in vec3 v_Pos;
in vec4 v_Color;
flat in int v_EntityID;

struct LightInfo
{
	vec3 u_LightPos;
	vec4 u_LightColor;
	float u_LightIntencity;
};

uniform LightInfo u_LightInfo[20];
uniform float u_LightQtd;
uniform vec3 u_Ambient;

void main()
{
	//vec4 tmp_Color = v_Color;
	//if (v_Color.a < 1.0)
		//discard;
	o_Color = vec4(v_Color.rgb * u_Ambient.rgb, v_Color.a);
	if (u_LightQtd < 1)
		o_Color = v_Color;
	else
		for (int i = 0; i < u_LightQtd; i++)
		{
			float distance = distance(u_LightInfo[i].u_LightPos.xy, v_Pos.xy);
			float diffuse = 0.0;

			if (distance <= u_LightInfo[i].u_LightIntencity)
				diffuse = 1.0 - abs(distance / u_LightInfo[i].u_LightIntencity);

			vec4 new_color = vec4(min(v_Color.rgb * ((u_LightInfo[i].u_LightColor.rgb * diffuse)), v_Color.rgb), v_Color.a);

			o_Color = max(o_Color, new_color);
		}
	o_EntityID = v_EntityID;
}