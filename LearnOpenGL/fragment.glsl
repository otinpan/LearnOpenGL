#version 330 core
out vec4 FragColor;
in vec3 ourColor;

uniform bool useChangeColor;
uniform vec4 changeColor;

void main() {
    if (useChangeColor)
        FragColor = changeColor;
    else
        FragColor = vec4(ourColor, 1.0);
}
