#version 330 core
#define MAX_TEXTURES_SLOTS

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in vec3 v_Pos;
in vec2 v_MiddlePoint;
in float v_Radius;
in float v_Fill;
in vec3 v_Rad_Fill_Th;

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

float DistancePvP(vec2 a, vec2 b)
{
	return float(sqrt(pow(b.x - a.x,2) + pow(b.y - a.y,2)));
}

void main()
{
	o_Color = vec4(0.0);
	int index = int(v_TexIndex);;
	vec4 tmp_Color = texture(u_Textures[index], v_TexCoord) * v_Color;
	/*if (tmp_Color.a < 1.0)
		discard;*/

	float dist_pvp = DistancePvP(v_MiddlePoint, v_Pos.xy);
	//if (dist_pvp == v_Radius) //Circunference

	if (dist_pvp > v_Rad_Fill_Th.x) // Outside
		discard;
	if (dist_pvp < v_Rad_Fill_Th.x - v_Rad_Fill_Th.z && v_Rad_Fill_Th.y == 0) //Inside
		discard;
	if (u_LightQtd < 1)
		o_Color = tmp_Color;
	else
		for (int i = 0; i < u_LightQtd; i++)
		{
			float distance = distance(u_LightInfo[i].u_LightPos.xy, v_Pos.xy);
			float diffuse = 0.0;

			if (distance <= u_LightInfo[i].u_LightIntencity)
				diffuse = 1.0 - abs(distance / u_LightInfo[i].u_LightIntencity);

			vec4 new_color = vec4(min(tmp_Color.rgb * ((u_LightInfo[i].u_LightColor * diffuse) + u_Ambient), tmp_Color.rgb), tmp_Color.a);

			o_Color = (o_Color + new_color) / 2;
		}
	//o_Color = tmp_Color;
}