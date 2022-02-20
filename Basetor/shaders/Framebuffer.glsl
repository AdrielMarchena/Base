#type vertex
#version 400 core
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
#version 400 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

uniform sampler2D u_Framebuffer;
uniform sampler2D u_GradeLut;
uniform float u_Width;
uniform float u_Height;

uniform float offsets[25];
uniform int offset_size;

uniform float kernel_5x5_slot[5*5];
uniform int kernel_size;

void main()
{
    if(kernel_size > 0)
    {
        vec3 cl = vec3(0.0f);
        for(int i = 0; i < kernel_size; i++)
            cl += vec3(texture(u_Framebuffer, v_TexCoord + offsets[i % offset_size])) * kernel_5x5_slot[i];
        o_Color = vec4(cl,1.0f);

    }
    else
        o_Color = texture(u_Framebuffer, v_TexCoord);
}

/*

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


//inside main:
	float offset_x = 1.0f / u_Width;
    float offset_y = 1.0f / u_Height;
	
    vec2 offsets[9] = vec2[]
    (
        vec2(-offset_x, offset_y),  vec2(0.0f, offset_y),  vec2(offset_x,offset_y),
        vec2(-offset_x, 0.0f),      vec2(0.0f, 0.0f),      vec2(offset_x,0.0f),
        vec2(-offset_x, -offset_y), vec2(0.0f, -offset_y), vec2(offset_x,-offset_y)
    );

*/