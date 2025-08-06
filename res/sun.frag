#version 330 core
out vec4 FragColor;
uniform vec3 u_EmissiveColor;
void main()
{
FragColor = vec4(u_EmissiveColor, 1.0);
}