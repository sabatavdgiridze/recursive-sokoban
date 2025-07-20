#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;

void main() {
    vec2 pixelStep = 1.0 / vec2(textureSize(texture0, 0));
    vec4 sum = vec4(0.0);
    for (int i = -4; i <= 4; i++) {
        for (int j = -4; j <= 4; j++) {
            sum += texture(texture0, fragTexCoord + vec2(i, j) * pixelStep);
        }
    }
    finalColor = sum / 81.0;
}