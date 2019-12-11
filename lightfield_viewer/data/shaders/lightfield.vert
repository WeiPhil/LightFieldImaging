#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 4) in vec2 aTextCoord;

out vec2 TexCoord;

uniform vec3 image_properties;

void main() {
    gl_Position = vec4(aPosition.x / 1.777777777 * image_properties.x / image_properties.y, aPosition.yz, 1.0);
    TexCoord = aTextCoord;
}