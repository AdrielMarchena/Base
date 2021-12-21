#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in int a_EntityID;

uniform mat4 u_ViewProj;

out vec4 v_Color;
out float v_TexIndex;
out vec2 v_TexCoord;
out vec3 v_Pos;

layout(location = 3) out flat int v_EntityID;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_Pos = a_Position;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProj * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core
#define MAX_TEXTURES_SLOTS 16

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

in vec4 v_Color;
in float v_TexIndex;
in vec2 v_TexCoord;
in vec3 v_Pos;

layout(location = 3) in flat int v_EntityID;

uniform sampler2D u_Textures[MAX_TEXTURES_SLOTS];

float median(float r, float g, float b) 
{
    return max(min(r, g), min(max(r, g), b));
}

float screenPxRange() {
	int index = int(v_TexIndex);
    vec2 unitRange = vec2(5.0)/vec2(textureSize(u_Textures[index], 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(v_TexCoord);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

void main()
{
	
	//vec2 flipped_tex_Coords = vec2(v_TexCoord.x, 1.0 - v_TexCoord.y);
	vec2 flipped_tex_Coords = v_TexCoord;
	vec2 pos = flipped_tex_Coords.xy;
	int index = int(v_TexIndex);
	vec3 sample = texture(u_Textures[index],flipped_tex_Coords.xy).rgb;
	ivec2 sz = textureSize(u_Textures[index],0).xy;
	float dx = dFdx(pos.x) * sz.x;
	float dy = dFdy(pos.y) * sz.y;
	float toPixels = 8.0 * inversesqrt(dx * dx + dy * dy);
	float sigDist = median(sample.r,sample.g,sample.b);
	float w = fwidth(sigDist);
	float opacity = smoothstep(0.5 - w,0.5 + w,sigDist);
	
	o_Color = vec4(v_Color.rgb,opacity);
	
	
	/*
	o_Color = vec4(0.0);
	int index = int(v_TexIndex);
	vec2 textureUV = vec2(textureSize(u_Textures[index],0));
	vec2 pos = v_TexCoord;

	vec2 res = vec2(1366,728);
	
    pos *= (textureUV / res.xy);
    pos /= res.xy;

	vec2 msdfUnit = 5.0/vec2(textureSize(u_Textures[index],0));
	vec3 t = texture(u_Textures[index],pos).rgb;
	float sigDist = median(t.r, t.g, t.b) - 0.5;
	sigDist *= dot(msdfUnit,0.5/fwidth(pos));
	float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
	vec4 bgColor = vec4(vec3(0.0),1.0);
	o_Color = mix(bgColor, v_Color, opacity);
	*/
	
	/*
	o_Color = vec4(0.0);
	int index = int(v_TexIndex);
	vec3 msd = texture(u_Textures[index],v_TexCoord).rgb;
	float sd = median(msd.r,msd.g,msd.b);
	float screenPxDistance = screenPxRange()*(sd - 0.5);
	//float screenPxDistance = 1.0;
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	vec4 bgColor = vec4(vec3(0.0),1.0);
    o_Color = mix(bgColor, v_Color, opacity);
	*/

	o_EntityID = v_EntityID;
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