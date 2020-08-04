#version 330 core
out vec4 FragColor;

in vec2 tex_uv; 

uniform sampler2D u_texture;

void main()
{
    FragColor = texture(u_texture, vec2(tex_uv.x,-tex_uv.y));
} 