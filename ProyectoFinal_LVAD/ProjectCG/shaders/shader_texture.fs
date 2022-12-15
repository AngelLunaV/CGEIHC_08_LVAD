#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec3 aColor;
uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoord);
}