#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProj;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;

	gl_Position = u_ViewProj * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

uniform sampler2D u_Framebuffer;
uniform sampler2D u_GradeLut;
uniform float u_Width;
uniform float u_Height;
uniform int u_UseGrade;

vec4 transform(in vec4 textureColor, in sampler2D lookupTable);

#define ONE_256 0.00390625
#define ONE_256N -0.00390625

float sharpen[9] = float[]
(
    0,-1, 0,
   -1, 5,-1,
    0,-1, 0
);

float box_blur[9] = float[]
(
    1/9,1/9,1/9,
    1/9,1/9,1/9,
    1/9,1/9,1/9
);

float edge_detection1[9] = float[]
(
   0, 0,-1,
   0, 0, 0,
  -1, 0, 0
);

float edge_detection12[9] = float[]
(
    0,-1, 0,
   -1, 4,-1,
    0,-1, 0
);

float edge_detection3[9] = float[]
(
   -1,-1,-1,
   -1, 8,-1,
   -1,-1,-1
);

float gaussian_blur3x3[9] = float[]
(
	1 * 0.0625, 2 * 0.0625, 1 * 0.0625,
	2 * 0.0625, 4 * 0.0625, 2 * 0.0625,
	1 * 0.0625, 2 * 0.0625, 1 * 0.0625
);

float gaussian_blur5x5[25] = float[]
(
	1 * ONE_256, 4 * ONE_256, 6 * ONE_256, 4 * ONE_256, 1 * ONE_256,
	4 * ONE_256,16 * ONE_256,24 * ONE_256,16 * ONE_256, 4 * ONE_256,
	6 * ONE_256,24 * ONE_256,36 * ONE_256,24 * ONE_256, 6 * ONE_256,
	4 * ONE_256,16 * ONE_256,24 * ONE_256,16 * ONE_256, 4 * ONE_256,
	1 * ONE_256, 4 * ONE_256, 6 * ONE_256, 4 * ONE_256, 1 * ONE_256
);

float unsharp_masking[25] = float[]
(
	1 * ONE_256N, 4 * ONE_256N, 6 * ONE_256N, 4 * ONE_256N, 1 * ONE_256N,
	4 * ONE_256N,16 * ONE_256N,24 * ONE_256N,16 * ONE_256N, 4 * ONE_256N,
	6 * ONE_256N,24 * ONE_256N,-476 * ONE_256N,24 * ONE_256N,6 * ONE_256N,
	4 * ONE_256N,16 * ONE_256N,24 * ONE_256N,16 * ONE_256N, 4 * ONE_256N,
	1 * ONE_256N, 4 * ONE_256N, 6 * ONE_256N, 4 * ONE_256N, 1 * ONE_256N
);

//(1, 2, 1, 2, 4, 2, 1, 2, 1) * 0.0625

void main()
{

    float offset_x = 1.0f / u_Width;
    float offset_y = 1.0f / u_Height;
	
    vec2 offsets[9] = vec2[]
    (
        vec2(-offset_x, offset_y),  vec2(0.0f, offset_y),  vec2(offset_x,offset_y),
        vec2(-offset_x, 0.0f),      vec2(0.0f, 0.0f),      vec2(offset_x,0.0f),
        vec2(-offset_x, -offset_y), vec2(0.0f, -offset_y), vec2(offset_x,-offset_y)
    );
	
	int size = 9;
	int size_offset = 9;
    vec3 cl = vec3(0.0f);
    for(int i = 0; i < size; i++)
        cl += vec3(texture(u_Framebuffer, v_TexCoord.st + offsets[i % size_offset])) * sharpen[i];
    
    vec4 color = vec4(cl,1.0f);
	//vec4 color = texture(u_Framebuffer,v_TexCoord);
    if(u_UseGrade == 1)
	    o_Color = transform(color,u_GradeLut);
    else
        o_Color = color;
}

vec4 transform(in vec4 textureColor, in sampler2D lookupTable) {
    #ifndef LUT_NO_CLAMP
        textureColor = clamp(textureColor, 0.0, 1.0);
    #endif

    mediump float blueColor = textureColor.b * 63.0;

    mediump vec2 quad1;
    quad1.y = floor(floor(blueColor) / 8.0);
    quad1.x = floor(blueColor) - (quad1.y * 8.0);

    mediump vec2 quad2;
    quad2.y = floor(ceil(blueColor) / 8.0);
    quad2.x = ceil(blueColor) - (quad2.y * 8.0);

    highp vec2 texPos1;
    texPos1.x = (quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
    texPos1.y = (quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);

    #ifdef LUT_FLIP_Y
        texPos1.y = 1.0-texPos1.y;
    #endif

    highp vec2 texPos2;
    texPos2.x = (quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
    texPos2.y = (quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);

    #ifdef LUT_FLIP_Y
        texPos2.y = 1.0-texPos2.y;
    #endif

    lowp vec4 newColor1 = texture(lookupTable, texPos1);
    lowp vec4 newColor2 = texture(lookupTable, texPos2);

    lowp vec4 newColor = mix(newColor1, newColor2, fract(blueColor));
    #ifndef LUT_NO_CLAMP
        return clamp(newColor, 0.0, 1.0);
    #endif
    return newColor;
}