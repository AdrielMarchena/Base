#version 330 core

layout(location = 0) out vec4 o_Color;

in vec3 v_Pos;
in vec4 v_Color;

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

	if (u_LightQtd < 1)
		o_Color = v_Color;
	else
		for (int i = 0; i < u_LightQtd; i++)
		{
			float distance = distance(u_LightInfo[i].u_LightPos.xy, v_Pos.xy);
			float diffuse = 0.0;

			if (distance <= u_LightInfo[i].u_LightIntencity)
				diffuse = 1.0 - abs(distance / u_LightInfo[i].u_LightIntencity);

			vec4 new_color = vec4(min(v_Color.rgb * ((u_LightInfo[i].u_LightColor * diffuse) + u_Ambient), v_Color.rgb), v_Color.a);

			o_Color = (o_Color + new_color) / 2;
		}
	//o_Color = v_Color;
}