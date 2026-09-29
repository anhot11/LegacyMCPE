R"GLSL(
#version 330 core
layout(location=0) in vec3  aPos;
layout(location=1) in vec2  aUV0;
layout(location=2) in vec4  aColor;
layout(location=3) in vec3  aNormal;
layout(location=4) in ivec2 aLMraw;

uniform mat4  uMVP;
uniform mat4  uMV;
uniform mat3  uNormalMatrix;
uniform float uNormalSign;
uniform mat4  uTexMat0;
uniform vec4  uBaseColor;
uniform int   uLighting;
uniform vec3  uLight0Dir;
uniform vec3  uLight1Dir;
uniform vec3  uLightDiffuse;
uniform vec3  uLightAmbient;
uniform vec3  uChunkOffset;
uniform int   uFogMode;
uniform float uFogStart;
uniform float uFogEnd;
uniform float uFogDensity;
uniform vec4  uLMTransform;
uniform vec2  uGlobalLM;

out vec2  vUV0;
out vec2  vUV1;
out vec4  vColor;
out float vFogFactor;

void main() {
    vec4 aPos4   = vec4(aPos + uChunkOffset, 1.0);
    vec4 eyePos  = uMV  * aPos4;
    gl_Position  = uMVP * aPos4;
    vUV0 = (uTexMat0 * vec4(aUV0, 0.0, 1.0)).xy; 

    vec2 lm = (aLMraw.x <= -500) ? uGlobalLM : vec2(aLMraw);
    vUV1 = (lm / 256.0) * uLMTransform.xy + uLMTransform.zw;

    bool sentinel = (aColor == vec4(0.0));
    vec4 col = sentinel ? uBaseColor : aColor.abgr;
    if (uLighting == 1) {
        vec3 n = normalize(uNormalMatrix * aNormal) * uNormalSign;

        float d0 = max(dot(n, uLight0Dir), 0.0);
        float d1 = max(dot(n, uLight1Dir), 0.0);
        vColor = vec4(col.rgb * (uLightAmbient + uLightDiffuse * clamp(d0 + d1, 0.0, 1.0)), col.a);
    } else {
        vColor = col;
    }

    float eDist = length(eyePos.xyz);
    if      (uFogMode == 1) vFogFactor = clamp((uFogEnd - eDist) / max(uFogEnd - uFogStart, 1e-4), 0.0, 1.0);
    else if (uFogMode == 2) vFogFactor = clamp(exp(-uFogDensity * eDist), 0.0, 1.0);
    else if (uFogMode == 3) { float d = uFogDensity * eDist; vFogFactor = clamp(exp(-d*d), 0.0, 1.0); }
    else                    vFogFactor = 1.0;
}
)GLSL";