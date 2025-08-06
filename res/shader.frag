#version 330 core
in vec2 texCoord;
uniform sampler2D u_image;
out vec4 out_color;
void main() {
out_color = texture(u_image, texCoord);
};