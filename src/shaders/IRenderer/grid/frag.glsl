#version 330 core
in vec3 nearPoint;
in vec3 farPoint;

out vec4 FragColor;

uniform mat4 view;
uniform mat4 projection;
uniform float nearPlane;
uniform float farPlane;

vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) {
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 gridLines = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(gridLines.x, gridLines.y);
    float minimumz = min(derivative.y, 1.0);
    float minimumx = min(derivative.x, 1.0);

    vec4 color = vec4(0.35, 0.35, 0.35, 1.0 - min(line, 1.0));

    // z axis (red), fades toward origin
    if (fragPos3D.x > -1.0 * minimumx && fragPos3D.x < 1.0 * minimumx && drawAxis)
    color = vec4(1.0, 0.3, 0.3, color.a);
    // x axis (blue)
    if (fragPos3D.z > -1.0 * minimumz && fragPos3D.z < 1.0 * minimumz && drawAxis)
    color = vec4(0.3, 0.3, 1.0, color.a);

    return color;
}

float computeDepth(vec3 pos) {
    vec4 clipSpacePos = projection * view * vec4(pos, 1.0);
    return (clipSpacePos.z / clipSpacePos.w);
}

float computeLinearDepth(vec3 pos) {
    vec4 clipSpacePos = projection * view * vec4(pos, 1.0);
    float clipSpaceDepth = (clipSpacePos.z / clipSpacePos.w) * 2.0 - 1.0;
    float linearDepth = (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - clipSpaceDepth * (farPlane - nearPlane));
    return linearDepth / farPlane; // normalize
}

void main() {
    // t is where the ray from near to far plane crosses y=0 (the ground plane)
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    if (t <= 0.0)
    discard;

    gl_FragDepth = computeDepth(fragPos3D) * 0.5 + 0.5;

    float linearDepth = computeLinearDepth(fragPos3D);
    float fading = max(0, (0.5 - linearDepth));

    vec4 gridColor = grid(fragPos3D, 1.0, true) + grid(fragPos3D, 0.1, true);
    gridColor.a *= fading;

    if (gridColor.a < 0.01)
    discard;

    FragColor = gridColor;
}