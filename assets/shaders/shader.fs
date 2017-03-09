#version 410

in vec3 normalV;

out vec4 fragColor;

const vec3 LIGHT_DIR=vec3(0.0, 1.0, 1.0);

float diffuse(vec3 lightDir, vec3 normal)
{
    return max(0.0, dot(normal, lightDir));
}
void main()
{
    float diff = diffuse(normalize(LIGHT_DIR), normalize(normalV));
    diff += diffuse(normalize(vec3(1.0, 0.0, 0.0)), normalize(normalV));
    fragColor = vec4(vec3(1.0) * diff, 1.0);
}
