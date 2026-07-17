#version 430
in vec2 f_TexCoord;
out vec4 FragColor;
uniform vec2 u_offsets[9];
uniform int u_edge_kernel[9];
uniform float u_blur_kernel[9];
uniform bool u_shake;
uniform bool u_chaos;
uniform bool u_confuse;
uniform sampler2D u_scene;

void main()
{
    FragColor =vec4(0.0f);
    vec3 sampler[9];
    if(u_chaos || u_shake)
    {
        for(int i =0;i<9;++i)
        {
            sampler[i] = vec3(texture(u_scene,f_TexCoord + u_offsets[i]));
        }
    }
    if(u_chaos)
    {
        for(int i=0;i<9;++i)
        {
            FragColor+=vec4(sampler[i]*u_edge_kernel[i],0.0f);
        }
            FragColor.a = 1.0f;

    }
    else if(u_confuse)
    {
        FragColor = vec4(1.0-texture(u_scene,f_TexCoord).rgb,1.0f);
    }
    else if(u_shake)
    {
        for(int i =0;i<9;++i)
        {
            FragColor+=vec4(sampler[i]*u_blur_kernel[i],0.0f);
        }
            FragColor.a = 1.0f;

    }
    else
    {
        FragColor = texture(u_scene,f_TexCoord);
    }
}