R"GLSL(
#version 300 es
precision mediump float;
precision mediump int;

uniform sampler2D uTex0;
uniform sampler2D uTex1;
uniform int   uUseTexture;
uniform int   uUseLightmap;
uniform float uAlphaRef;
uniform vec4  uFogColor;
uniform int   uFogEnable;
uniform float uInvGamma;
uniform int   uShaderPreset;

in  vec2  vUV0;
in  vec2  vUV1;
in  vec4  vColor;
in  float vFogFactor;
out vec4  oColor;

void main() {
    vec4 texColor = (uUseTexture != 0) ? texture(uTex0, vUV0) : vec4(1.0);
    vec4 c = texColor * vColor;
    if (c.a < uAlphaRef) discard;
    if (uUseLightmap != 0) c.rgb *= texture(uTex1, vUV1).rgb;
    if (uFogEnable != 0) c.rgb = mix(uFogColor.rgb, c.rgb, vFogFactor);

    c.rgb = pow(c.rgb, vec3(uInvGamma));

    if (uShaderPreset == 1) {
        // Vivid Colors
        float lum = dot(c.rgb, vec3(0.299, 0.587, 0.114));
        c.rgb = mix(vec3(lum), c.rgb, 1.35);
        c.rgb = c.rgb * vec3(1.05, 1.02, 0.96);
    } else if (uShaderPreset == 2) {
        // Cel-Shaded / Stylized
        c.rgb = floor(c.rgb * 6.0 + 0.5) / 6.0;
    } else if (uShaderPreset == 3) {
        // Tactical Night Vision
        float lum = dot(c.rgb, vec3(0.299, 0.587, 0.114));
        c.rgb = vec3(lum * 0.15, lum * 1.25, lum * 0.35);
    } else if (uShaderPreset == 4) {
        // Warm Sunset
        c.rgb = c.rgb * vec3(1.22, 0.94, 0.82);
    }

    oColor = c;
}
)GLSL";
