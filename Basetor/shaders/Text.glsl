#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_ViewProj;

out vec4 v_Color;
out float v_TexIndex;
out vec2 v_TexCoord;
out vec3 v_Pos;

void main()
{
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	v_Pos = a_Position;

	gl_Position = u_ViewProj * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core
#define MAX_TEXTURES_SLOTS 8

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in float v_TexIndex;
in vec2 v_TexCoord;
in vec3 v_Pos;

uniform sampler2D u_Textures[MAX_TEXTURES_SLOTS];

float median(float r, float g, float b)
{
	return max(min(r, g), min(max(r, g), b));
}

void main()
{
	o_Color = vec4(0.0);
	int index = v_TexIndex;
	vec3 msd = texture(u_Textures[index],v_TexCoord).rgb;
	float sd = median(msd.r,msd.g,msd.b);
	float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    o_Color = mix(bgColor, fgColor, opacity) * v_Color;
}



/*

in vec2 texCoord;
out vec4 color;
uniform sampler2D msdf;

uniform vec4 bgColor;
uniform vec4 fgColor;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
    vec3 msd = texture(msdf, texCoord).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    color = mix(bgColor, fgColor, opacity);
}

*/