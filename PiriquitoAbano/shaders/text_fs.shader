#version 330 core
#define MAX_TEXTURES_SLOTS

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in vec3 v_Pos;

uniform sampler2D u_Textures[MAX_TEXTURES_SLOTS];

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
	o_Color = vec4(0.0);
	int index = int(v_TexIndex);
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Textures[index], v_TexCoord).r);
	vec4 tmp_Color = sampled * v_Color;
	if (tmp_Color.a < 0.5)
		discard;
	if (u_LightQtd < 1)
		o_Color = tmp_Color;
	else
	{
		o_Color = vec4(tmp_Color.rgb * u_Ambient.rgb, tmp_Color.a);
		for (int i = 0; i < u_LightQtd; i++)
		{
			float distance = distance(u_LightInfo[i].u_LightPos.xy, v_Pos.xy);
			float diffuse = 0.0;

			if (distance <= u_LightInfo[i].u_LightIntencity)
				diffuse = 1.0 - abs(distance / u_LightInfo[i].u_LightIntencity);

			vec4 new_color = vec4(min(tmp_Color.rgb * ((u_LightInfo[i].u_LightColor * diffuse)), tmp_Color.rgb), tmp_Color.a);

			o_Color = max(o_Color, new_color);
		}
	}
	//o_Color = tmp_Color;
}