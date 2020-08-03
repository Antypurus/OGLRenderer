#version 330 core
out vec4 FragColor;

in vec2 tex_uv; 

uniform vec4 u_color;
uniform sampler2D u_texture;

void main()
{
    FragColor = texture(u_texture, -tex_uv);
} 