#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in vec2 a_MiddlePoint;
layout(location = 5) in vec3 a_Rad_Fill_Th;

uniform mat4 u_ViewProj;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out vec3 v_Pos;
out vec2 v_MiddlePoint;
//out vec3 v_Rad_Fill_Th;
out float v_Radius;
out float v_Fill;
out float v_Thick;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_Pos = a_Position;
	v_MiddlePoint = a_MiddlePoint;
	//v_Rad_Fill_Th = a_Rad_Fill_Th;

	v_Radius = a_Rad_Fill_Th.x;
	v_Fill = a_Rad_Fill_Th.y;
	v_Thick = a_Rad_Fill_Th.z;

	gl_Position = u_ViewProj * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core
#define MAX_TEXTURES_SLOTS 8

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in vec3 v_Pos;
in vec2 v_MiddlePoint;
//in vec3 v_Rad_Fill_Th;

in float v_Radius;
in float v_Fill;
in float v_Thick;

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
	return float(sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)));
}

void main()
{
	int index = int(v_TexIndex);;
	vec4 tmp_Color = texture(u_Textures[index], v_TexCoord) * v_Color;
	/*if (tmp_Color.a <= 0.0)
		discard;*/
	float dist_pvp = DistancePvP(v_MiddlePoint, v_Pos.xy);
	/*if (dist_pvp == v_Radius) //Circunference
		discard;*/
	if (dist_pvp > v_Radius) // Outside
		tmp_Color.a = 0.0;
	if (dist_pvp < v_Radius - v_Thick && v_Fill == 0) //Inside
		tmp_Color.a = 0.0;
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

			vec4 new_color = vec4(min(tmp_Color.rgb * (u_LightInfo[i].u_LightColor.rgb * diffuse), tmp_Color.rgb), tmp_Color.a);

			o_Color = max(o_Color, new_color);
		}
	}
}