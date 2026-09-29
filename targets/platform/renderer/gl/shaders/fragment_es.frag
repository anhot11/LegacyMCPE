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

    oColor = c;
}
)GLSL";
