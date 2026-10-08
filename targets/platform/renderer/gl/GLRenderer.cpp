#include "GLRenderer.h"

#include "SDL.h"
#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_stdinc.h"
#include "SDL_video.h"
#include "java/ByteBuffer.h"
#include "java/FloatBuffer.h"
#include "java/IntBuffer.h"
#include "platform/PlatformTypes.h"
#include "platform/renderer/renderer.h"
#include "minecraft/util/Log.h"

#if defined(__ANDROID__)
#include <android/log.h>
#define MCPL_LOGI(...) __android_log_print(ANDROID_LOG_INFO, "MCPL", __VA_ARGS__)
#define MCPL_LOGW(...) __android_log_print(ANDROID_LOG_WARN, "MCPL", __VA_ARGS__)
#define MCPL_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "MCPL", __VA_ARGS__)
#else
#define MCPL_LOGI(...)
#define MCPL_LOGW(...)
#define MCPL_LOGE(...)
#endif

// undefine macros from header to avoid argument mismatch
#undef glGenTextures
#undef glDeleteTextures
#undef glTexImage2D
#undef glCallLists
#undef glFog
#undef glLight
#undef glLightModel
#undef glTexGen
#undef glTexGeni
#undef glTexGenfv
#undef glTexCoordPointer
#undef glNormalPointer
#undef glColorPointer
#undef glVertexPointer
#undef glEnable
#undef glDisable
#undef glBlendFunc
#undef glDepthMask
#undef glColorMask
#undef glLineWidth
#undef glFrontFace
#undef glPolygonOffset
#undef glStencilFunc
#undef glStencilMask

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GLM_FORCE_RADIANS

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <optional>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace platform_internal {
IPlatformRenderer& PlatformRenderer_get() {
    static GLRenderer instance;
    return instance;
}
}  // namespace platform_internal

// MARK: Shaders

#define CPP_GLSL_INCLUDE

#ifdef GLES
static const char* VERT_SRC =
#include "./shaders/vertex_es.vert"

    ;
static const char* FRAG_SRC =
#include "./shaders/fragment_es.frag"

    ;

static const char* VERT_SRC_ES2 =
"precision highp float;\n"
"precision highp int;\n"
"attribute vec3  aPos;\n"
"attribute vec2  aUV0;\n"
"attribute vec4  aColor;\n"
"attribute vec3  aNormal;\n"
"attribute vec2  aLMraw;\n"
"uniform mat4  uMVP;\n"
"uniform mat4  uMV;\n"
"uniform mat3  uNormalMatrix;\n"
"uniform float uNormalSign;\n"
"uniform mat4  uTexMat0;\n"
"uniform vec4  uBaseColor;\n"
"uniform int   uLighting;\n"
"uniform vec3  uLight0Dir;\n"
"uniform vec3  uLight1Dir;\n"
"uniform vec3  uLightDiffuse;\n"
"uniform vec3  uLightAmbient;\n"
"uniform vec3  uChunkOffset;\n"
"uniform int   uFogMode;\n"
"uniform float uFogStart;\n"
"uniform float uFogEnd;\n"
"uniform float uFogDensity;\n"
"uniform vec4  uLMTransform;\n"
"uniform vec2  uGlobalLM;\n"
"varying vec2  vUV0;\n"
"varying vec2  vUV1;\n"
"varying vec4  vColor;\n"
"varying float vFogFactor;\n"
"void main() {\n"
"    vec4 aPos4   = vec4(aPos + uChunkOffset, 1.0);\n"
"    vec4 eyePos  = uMV  * aPos4;\n"
"    gl_Position  = uMVP * aPos4;\n"
"    vUV0 = (uTexMat0 * vec4(aUV0, 0.0, 1.0)).xy;\n"
"    vec2 lm = (aLMraw.x <= -500.0) ? uGlobalLM : aLMraw;\n"
"    vUV1 = (lm / 256.0) * uLMTransform.xy + uLMTransform.zw;\n"
"    bool sentinel = (aColor.x < 0.02 && aColor.y < 0.02 && aColor.z < 0.02 && aColor.w < 0.02);\n"
"    vec4 col = sentinel ? uBaseColor : vec4(aColor.w, aColor.z, aColor.y, aColor.x);\n"
"    if (uLighting == 1) {\n"
"        vec3 n = normalize(uNormalMatrix * aNormal) * uNormalSign;\n"
"        float d0 = max(dot(n, uLight0Dir), 0.0);\n"
"        float d1 = max(dot(n, uLight1Dir), 0.0);\n"
"        vColor = vec4(col.rgb * (uLightAmbient + uLightDiffuse * (d0 + d1)), col.a);\n"
"    } else {\n"
"        vColor = col;\n"
"    }\n"
"    float eDist = length(eyePos.xyz);\n"
"    if      (uFogMode == 1) vFogFactor = clamp((uFogEnd - eDist) / max(uFogEnd - uFogStart, 1e-4), 0.0, 1.0);\n"
"    else if (uFogMode == 2) vFogFactor = clamp(exp(-uFogDensity * eDist), 0.0, 1.0);\n"
"    else if (uFogMode == 3) { float d = uFogDensity * eDist; vFogFactor = clamp(exp(-d*d), 0.0, 1.0); }\n"
"    else                    vFogFactor = 1.0;\n"
"}\n";

static const char* FRAG_SRC_ES2 =
"precision highp float;\n"
"precision highp int;\n"
"uniform sampler2D uTex0;\n"
"uniform sampler2D uTex1;\n"
"uniform int   uUseTexture;\n"
"uniform int   uUseLightmap;\n"
"uniform float uAlphaRef;\n"
"uniform vec4  uFogColor;\n"
"uniform int   uFogEnable;\n"
"uniform float uInvGamma;\n"
"uniform int   uShaderPreset;\n"
"varying vec2  vUV0;\n"
"varying vec2  vUV1;\n"
"varying vec4  vColor;\n"
"varying float vFogFactor;\n"
"void main() {\n"
"    vec4 texColor = (uUseTexture != 0) ? texture2D(uTex0, vUV0) : vec4(1.0);\n"
"    vec4 c = texColor * vColor;\n"
"    if (uAlphaRef > 0.0 && c.a < uAlphaRef) discard;\n"
"    if (uUseLightmap != 0) c.rgb *= texture2D(uTex1, vUV1).rgb;\n"
"    if (uFogEnable != 0) c.rgb = mix(uFogColor.rgb, c.rgb, vFogFactor);\n"
"    if (uInvGamma > 0.01 && abs(uInvGamma - 1.0) > 0.05) {\n"
"        c.rgb = pow(clamp(c.rgb, 0.0001, 1.0), vec3(clamp(uInvGamma, 0.1, 5.0)));\n"
"    }\n"
"    gl_FragColor = c;\n"
"}\n";
#else
static const char* VERT_SRC =
#include "./shaders/vertex.vert"

    ;
static const char* FRAG_SRC =
#include "./shaders/fragment.frag"
    ;
#endif

#undef CPP_GLSL_INCLUDE

// MARK: OpenGL state

// Hello SDL and opengl 3.3
static SDL_Window* s_window = nullptr;
static SDL_GLContext s_glContext = nullptr;
static bool s_shouldClose = false;
static int s_windowWidth = 1920;
static int s_windowHeight = 1080;
static int s_reqWidth = 1920;
static int s_reqHeight = 1080;
static bool s_fullscreen = false;

static thread_local SDL_GLContext s_glCtx = nullptr;

static std::once_flag s_glCtxKeyOnce;

static const int MAX_SHARED_CTXS = 6;
static SDL_Window* s_sharedWins[MAX_SHARED_CTXS] = {};
static SDL_GLContext s_sharedCtxs[MAX_SHARED_CTXS] = {};
static int s_sharedCtxCount = 0;
static int s_nextSharedCtx = 0;
static std::mutex s_sharedMtx;
static std::mutex s_glCallMtx;
static std::thread::id s_mainThreadId;
static bool s_mainThreadSet = false;
static thread_local unsigned int s_rs_dirty_mask = 0xFFFFFFFF;
static GLuint s_defaultTex = 0;

static void appendDiagnosticLog(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buf[2048];
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    const char* mcPath = getenv("MC_PATH");
    FILE* f1 = fopen("/sdcard/LegacyMCPE/render_diagnostic.txt", "a");
    if (f1) { fputs(buf, f1); fclose(f1); }
    if (mcPath) {
        std::string p = std::string(mcPath) + "/render_diagnostic.txt";
        FILE* f2 = fopen(p.c_str(), "a");
        if (f2) { fputs(buf, f2); fclose(f2); }
    }
}

struct GLShadowState {
    bool blend;
    bool cull;
    bool depth;
    bool polygon;
    bool stencil;
    GLint blendSrc;
    GLint blendDst;
    GLboolean depthMask;
    GLboolean colorMask[4];
    float lineWidth;
    GLenum frontFace;
    float polySlope;
    float polyBias;
    GLenum stencilFunc;
    GLint stencilRef;
    GLuint stencilMask;
    GLuint stencilWriteMask;
};

static GLShadowState s_gl_state;
static unsigned int s_gl_shadow_mask = 0;

enum GLShadowBits {
    SHADOW_BLEND = 1 << 0,
    SHADOW_CULL = 1 << 1,
    SHADOW_DEPTH = 1 << 2,
    SHADOW_BLEND_FUNC = 1 << 3,
    SHADOW_DEPTH_MASK = 1 << 4,
    SHADOW_COLOR_MASK = 1 << 5,
    SHADOW_LINE_WIDTH = 1 << 6,
    SHADOW_FRONT_FACE = 1 << 7,
    SHADOW_POLY_OFFSET = 1 << 8,
    SHADOW_POLY_OFFSET_PARAMS = 1 << 9,
    SHADOW_STENCIL = 1 << 10,
    SHADOW_STENCIL_PARAMS = 1 << 11,
};

static void onFramebufferResize(int w, int h) {
#if defined(__ANDROID__) || defined(ANDROID)
    if (w < h) {
        std::swap(w, h);
    }
#endif
    if (w < 1) w = 1;
    if (h < 1) h = 1;
    s_windowWidth = w;
    s_windowHeight = h;
    glViewport(0, 0, w, h);
}

static GLuint compileShader(GLenum type, const char* src, const char* name = "") {
    while (src && (*src == '\r' || *src == '\n' || *src == ' ' || *src == '\t')) {
        src++;
    }
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    GLint ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[2048] = {0};
        glGetShaderInfoLog(s, sizeof(log), nullptr, log);
        fprintf(stderr, "[4J_Render] shader %s error:\n%s\n", name, log);
#if defined(__ANDROID__)
        __android_log_print(ANDROID_LOG_ERROR, "4J_Render", "shader %s compile error (type=0x%x):\n%s\n", name, (unsigned int)type, log);
#endif
        appendDiagnosticLog("[4J_Render] shader %s compile error (type=0x%x):\n%s\n", name, (unsigned int)type, log);
        glDeleteShader(s);
        return 0;
    }
#if defined(__ANDROID__)
    __android_log_print(ANDROID_LOG_INFO, "4J_Render", "shader %s compile OK (type=0x%x, id=%u)\n", name, (unsigned int)type, s);
#endif
    return s;
}

static GLuint linkProgram(GLuint v, GLuint f, bool isFallback = false) {
    GLuint p = glCreateProgram();
    glAttachShader(p, v);
    glAttachShader(p, f);
#ifdef GLES
    if (isFallback) {
        glBindAttribLocation(p, 0, "aPos");
        glBindAttribLocation(p, 1, "aUV0");
        glBindAttribLocation(p, 2, "aColor");
        glBindAttribLocation(p, 3, "aNormal");
        glBindAttribLocation(p, 4, "aLMraw");
    }
#endif
    glLinkProgram(p);
    GLint ok = 0;
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[2048] = {0};
        glGetProgramInfoLog(p, sizeof(log), nullptr, log);
        fprintf(stderr, "[4J_Render] link error:\n%s\n", log);
#if defined(__ANDROID__)
        __android_log_print(ANDROID_LOG_ERROR, "4J_Render", "program link error (id=%u):\n%s\n", p, log);
#endif
        appendDiagnosticLog("[4J_Render] program link error (id=%u, fallback=%d):\n%s\n", p, (int)isFallback, log);
        glDeleteProgram(p);
        return 0;
    }
#if defined(__ANDROID__)
    __android_log_print(ANDROID_LOG_INFO, "4J_Render", "program link OK (id=%u, fallback=%d)\n", p, (int)isFallback);
#endif
    return p;
}

// Shader struct
struct ShaderUniforms {
    GLuint prog = 0;

    GLint uMVP = -1, uMV = -1, uBaseColor = -1;
    GLint uTexMat0 = -1;
    GLint uNormalMatrix = -1, uNormalSign = -1;
    GLint uLighting = -1, uLight0Dir = -1, uLight1Dir = -1;
    GLint uLightDiffuse = -1, uLightAmbient = -1;
    GLint uFogMode = -1, uFogStart = -1, uFogEnd = -1;
    GLint uFogDensity = -1, uFogColor = -1, uFogEnable = -1;
    GLint uLMTransform = -1, uUseLightmap = -1, uAlphaRef = -1;
    GLint uTex0 = -1, uTex1 = -1, uGlobalLM = -1;
    GLint uUseTexture = -1;
    GLint uInvGamma = -1;
    GLint uShaderPreset = -1;
    GLint uChunkOffset = -1;

    void build(const char* vs, const char* fs) {
        GLuint v = compileShader(GL_VERTEX_SHADER, vs, "Vertex ES3");
        GLuint f = compileShader(GL_FRAGMENT_SHADER, fs, "Fragment ES3");
        if (v && f) {
            prog = linkProgram(v, f, false);
            glDeleteShader(v);
            glDeleteShader(f);
        }
#ifdef GLES
        if (!prog) {
            appendDiagnosticLog("[4J_Render] ES3 shader failed, attempting GLES 2.0 fallback...\n");
            GLuint v2 = compileShader(GL_VERTEX_SHADER, VERT_SRC_ES2, "Vertex ES2 Fallback");
            GLuint f2 = compileShader(GL_FRAGMENT_SHADER, FRAG_SRC_ES2, "Fragment ES2 Fallback");
            if (v2 && f2) {
                prog = linkProgram(v2, f2, true);
                glDeleteShader(v2);
                glDeleteShader(f2);
            }
        }
#endif
        if (!prog) {
            appendDiagnosticLog("[4J_Render] CRITICAL FATAL: Both primary and fallback shader programs failed to link!\n");
            return;
        }

#define L(x) x = glGetUniformLocation(prog, #x)
        L(uMVP);
        L(uMV);
        L(uNormalMatrix);
        L(uNormalSign);
        L(uTexMat0);
        L(uBaseColor);
        L(uLighting);
        L(uLight0Dir);
        L(uLight1Dir);
        L(uLightDiffuse);
        L(uLightAmbient);
        L(uFogMode);
        L(uFogStart);
        L(uFogEnd);
        L(uFogDensity);
        L(uFogColor);
        L(uFogEnable);
        L(uLMTransform);
        L(uUseLightmap);
        L(uAlphaRef);
        L(uTex0);
        L(uTex1);
        L(uGlobalLM);
        L(uUseTexture);
        L(uInvGamma);
        L(uShaderPreset);
        L(uChunkOffset);
#undef L

        glUseProgram(prog);
        glUniform1i(uTex0, 0);
        glUniform1i(uTex1, 1);
    }
} s_shader;

// Matrix stacks
static const int STACK_DEPTH = 64;
struct MatrixStack {
    glm::mat4 stack[STACK_DEPTH];
    int top = 0;
    MatrixStack() { stack[0] = glm::mat4(1.f); }
    glm::mat4& cur() { return stack[top]; }
    void push() {
        if (top < STACK_DEPTH - 1) {
            stack[top + 1] = stack[top];
            ++top;
        }
    }
    void pop() {
        if (top > 0) --top;
    }
    void load(const glm::mat4& m) { cur() = m; }
    void mul(const glm::mat4& m) { cur() = cur() * m; }
};
static thread_local MatrixStack s_proj, s_mv, s_tex[2];
static thread_local int s_matMode = 0;  // 0=MV 1=proj 2=tex0 3=tex1

// cache normal matrix
static thread_local bool s_normalMatDirty = true;
static thread_local glm::mat3 s_cachedNormalMat;
static thread_local float s_cachedNormalSign = 1.0f;
static thread_local bool s_matDirty = true;
static inline void markNormalDirty() { s_normalMatDirty = true; }
static inline void markMatrixDirty() { s_matDirty = true; }

static MatrixStack& activeStack() {
    switch (s_matMode) {
        case 1:
            return s_proj;
        case 2:
            return s_tex[0];
        case 3:
            return s_tex[1];
    }
    return s_mv;
}

static void flushMatrices() {
    if (s_matDirty) {
        glm::mat4 mvp = s_proj.cur() * s_mv.cur();
        glUniformMatrix4fv(s_shader.uMVP, 1, GL_FALSE, glm::value_ptr(mvp));
        glUniformMatrix4fv(s_shader.uMV, 1, GL_FALSE,
                           glm::value_ptr(s_mv.cur()));

        // Send the texture matrix to the depths of hell...
        glUniformMatrix4fv(s_shader.uTexMat0, 1, GL_FALSE,
                           glm::value_ptr(s_tex[0].cur()));
        s_matDirty = false;
    }

    if (s_shader.uNormalMatrix >= 0 && s_normalMatDirty) {
        glm::mat3 m3 = glm::mat3(s_mv.cur());
        s_cachedNormalMat = glm::transpose(glm::inverse(m3));
        s_cachedNormalSign = glm::determinant(m3) < 0.0f ? -1.0f : 1.0f;
        s_normalMatDirty = false;
        glUniformMatrix3fv(s_shader.uNormalMatrix, 1, GL_FALSE,
                           glm::value_ptr(s_cachedNormalMat));
        glUniform1f(s_shader.uNormalSign, s_cachedNormalSign);
    }
}

// Render state
struct RenderState {
    glm::vec4 baseColor = {1, 1, 1, 1};
    glm::vec4 fogColor = {0, 0, 0, 1};
    float fogStart = 0, fogEnd = 1000, fogDensity = 0;
    int fogMode = 0;
    bool fogEnable = false;
    bool alphaTest = true;
    float alphaRef = 0.1f;
    float gamma = 1.0f;
    bool useTexture = true, useLightmap = false, lighting = false;
    glm::vec3 l0 = {0.173913f, 0.869565f, -0.608696f};
    glm::vec3 l1 = {-0.173913f, 0.869565f, 0.608696f};
    glm::vec3 ldiff = {0.6f, 0.6f, 0.6f};
    glm::vec3 lamb = {0.4f, 0.4f, 0.4f};
    glm::vec4 lmt = {1, 1, 0, 0};
    glm::vec2 globalLM = {240.f, 240.f};  // fullbright default
    int activeTexture = 0;
};

enum RenderDirtyBits {
    DIRTY_BASECOLOR = 1 << 0,
    DIRTY_LIGHTING = 1 << 1,
    DIRTY_FOG = 1 << 2,
    DIRTY_ALPHA = 1 << 3,
    DIRTY_GAMMA = 1 << 4,
    DIRTY_TEXTURE = 1 << 5,
    DIRTY_LMT = 1 << 6,
    DIRTY_GLOBAL_LM = 1 << 7,
};

static inline void markDirty(unsigned int bit) { s_rs_dirty_mask |= bit; }

static thread_local RenderState s_rs;

// track currently bound program to avoid iggy shitting up
static GLuint s_boundProgram = 0;

static void glShadowSetBlend(bool e) {
    if (!(s_gl_shadow_mask & SHADOW_BLEND) || s_gl_state.blend != e) {
        if (e)
            ::glEnable(GL_BLEND);
        else
            ::glDisable(GL_BLEND);
        s_gl_state.blend = e;
        s_gl_shadow_mask |= SHADOW_BLEND;
    }
}

static void glShadowSetCull(bool e) {
    if (!(s_gl_shadow_mask & SHADOW_CULL) || s_gl_state.cull != e) {
        if (e)
            ::glEnable(GL_CULL_FACE);
        else
            ::glDisable(GL_CULL_FACE);
        s_gl_state.cull = e;
        s_gl_shadow_mask |= SHADOW_CULL;
    }
}

static void glShadowSetDepthTest(bool e) {
    if (!(s_gl_shadow_mask & SHADOW_DEPTH) || s_gl_state.depth != e) {
        if (e)
            ::glEnable(GL_DEPTH_TEST);
        else
            ::glDisable(GL_DEPTH_TEST);
        s_gl_state.depth = e;
        s_gl_shadow_mask |= SHADOW_DEPTH;
    }
}

static void glShadowSetBlendFunc(GLint s, GLint d) {
    if (!(s_gl_shadow_mask & SHADOW_BLEND_FUNC) || s_gl_state.blendSrc != s ||
        s_gl_state.blendDst != d) {
        ::glBlendFunc(s, d);
        s_gl_state.blendSrc = s;
        s_gl_state.blendDst = d;
        s_gl_shadow_mask |= SHADOW_BLEND_FUNC;
    }
}

static void glShadowSetDepthMask(GLboolean e) {
    if (!(s_gl_shadow_mask & SHADOW_DEPTH_MASK) || s_gl_state.depthMask != e) {
        ::glDepthMask(e);
        s_gl_state.depthMask = e;
        s_gl_shadow_mask |= SHADOW_DEPTH_MASK;
    }
}

static void glShadowSetColorMask(GLboolean r, GLboolean g, GLboolean b,
                                 GLboolean a) {
    if (!(s_gl_shadow_mask & SHADOW_COLOR_MASK) ||
        s_gl_state.colorMask[0] != r || s_gl_state.colorMask[1] != g ||
        s_gl_state.colorMask[2] != b || s_gl_state.colorMask[3] != a) {
        ::glColorMask(r, g, b, a);
        s_gl_state.colorMask[0] = r;
        s_gl_state.colorMask[1] = g;
        s_gl_state.colorMask[2] = b;
        s_gl_state.colorMask[3] = a;
        s_gl_shadow_mask |= SHADOW_COLOR_MASK;
    }
}

static void glShadowSetLineWidth(float w) {
    if (!(s_gl_shadow_mask & SHADOW_LINE_WIDTH) || s_gl_state.lineWidth != w) {
        ::glLineWidth(w);
        s_gl_state.lineWidth = w;
        s_gl_shadow_mask |= SHADOW_LINE_WIDTH;
    }
}

static void glShadowSetFrontFace(GLenum mode) {
    if (!(s_gl_shadow_mask & SHADOW_FRONT_FACE) ||
        s_gl_state.frontFace != mode) {
        ::glFrontFace(mode);
        s_gl_state.frontFace = mode;
        s_gl_shadow_mask |= SHADOW_FRONT_FACE;
    }
}

static void glShadowSetPolygonOffset(float slope, float bias) {
    bool enable = (slope != 0.0f || bias != 0.0f);
    if (!(s_gl_shadow_mask & SHADOW_POLY_OFFSET) ||
        s_gl_state.polygon != enable) {
        if (enable)
            ::glEnable(GL_POLYGON_OFFSET_FILL);
        else
            ::glDisable(GL_POLYGON_OFFSET_FILL);
        s_gl_state.polygon = enable;
        s_gl_shadow_mask |= SHADOW_POLY_OFFSET;
    }
    if (enable) {
        if (!(s_gl_shadow_mask & SHADOW_POLY_OFFSET_PARAMS) ||
            s_gl_state.polySlope != slope || s_gl_state.polyBias != bias) {
            ::glPolygonOffset(slope, bias);
            s_gl_state.polySlope = slope;
            s_gl_state.polyBias = bias;
            s_gl_shadow_mask |= SHADOW_POLY_OFFSET_PARAMS;
        }
    }
}

static void glShadowSetStencil(GLenum fn, uint8_t ref, uint8_t fmask,
                               uint8_t wmask) {
    if (!(s_gl_shadow_mask & SHADOW_STENCIL) || !s_gl_state.stencil) {
        ::glEnable(GL_STENCIL_TEST);
        s_gl_state.stencil = true;
        s_gl_shadow_mask |= SHADOW_STENCIL;
    }
    if (!(s_gl_shadow_mask & SHADOW_STENCIL_PARAMS) ||
        s_gl_state.stencilFunc != fn || s_gl_state.stencilRef != (GLint)ref ||
        s_gl_state.stencilMask != fmask ||
        s_gl_state.stencilWriteMask != wmask) {
        ::glStencilFunc(fn, ref, fmask);
        ::glStencilMask(wmask);
        s_gl_state.stencilFunc = fn;
        s_gl_state.stencilRef = (GLint)ref;
        s_gl_state.stencilMask = fmask;
        s_gl_state.stencilWriteMask = wmask;
        s_gl_shadow_mask |= SHADOW_STENCIL_PARAMS;
    }
}
static thread_local bool s_chunkOffsetValid = false;
static thread_local glm::vec3 s_chunkOffset;

static void pushRenderState() {
    if (!s_shader.prog) {
        ::glClearColor(0.8f, 0.1f, 0.1f, 1.0f);
        ::glClear(GL_COLOR_BUFFER_BIT);
        return;
    }

    // only call glUseProgram when something actually changed the binding
    if (s_boundProgram != s_shader.prog) {
        glUseProgram(s_shader.prog);
        s_boundProgram = s_shader.prog;
        s_matDirty = true;
        s_normalMatDirty = true;
        s_rs_dirty_mask = 0xFFFFFFFF;
    }

    if (s_rs_dirty_mask) {
        if (s_rs_dirty_mask & DIRTY_BASECOLOR)
            glUniform4fv(s_shader.uBaseColor, 1,
                         glm::value_ptr(s_rs.baseColor));
        if (s_rs_dirty_mask & DIRTY_LIGHTING) {
            glUniform1i(s_shader.uLighting, s_rs.lighting ? 1 : 0);
            glUniform3fv(s_shader.uLight0Dir, 1, glm::value_ptr(s_rs.l0));
            glUniform3fv(s_shader.uLight1Dir, 1, glm::value_ptr(s_rs.l1));
            glUniform3fv(s_shader.uLightDiffuse, 1, glm::value_ptr(s_rs.ldiff));
            glUniform3fv(s_shader.uLightAmbient, 1, glm::value_ptr(s_rs.lamb));
        }
        if (s_rs_dirty_mask & DIRTY_FOG) {
            glUniform1i(s_shader.uFogMode, s_rs.fogMode);
            glUniform1f(s_shader.uFogStart, s_rs.fogStart);
            glUniform1f(s_shader.uFogEnd, s_rs.fogEnd);
            glUniform1f(s_shader.uFogDensity, s_rs.fogDensity);
            glUniform4fv(s_shader.uFogColor, 1, glm::value_ptr(s_rs.fogColor));
            glUniform1i(s_shader.uFogEnable, s_rs.fogEnable ? 1 : 0);
        }
        if (s_rs_dirty_mask & DIRTY_TEXTURE) {
            glUniform1i(s_shader.uUseTexture, s_rs.useTexture ? 1 : 0);
            glUniform1i(s_shader.uUseLightmap, s_rs.useLightmap ? 1 : 0);
        }
        if (s_rs_dirty_mask & DIRTY_ALPHA) {
            float effAlpha = s_rs.alphaTest ? s_rs.alphaRef : -1.0f;
            glUniform1f(s_shader.uAlphaRef, effAlpha);
        }
        if (s_rs_dirty_mask & DIRTY_GAMMA)
            glUniform1f(s_shader.uInvGamma, 1.0f / s_rs.gamma);
        if (s_rs_dirty_mask & DIRTY_LMT)
            glUniform4fv(s_shader.uLMTransform, 1, glm::value_ptr(s_rs.lmt));
        if (s_rs_dirty_mask & DIRTY_GLOBAL_LM)
            glUniform2fv(s_shader.uGlobalLM, 1, glm::value_ptr(s_rs.globalLM));
        s_rs_dirty_mask = 0;
    }
    if (s_shader.uChunkOffset >= 0) {
        if (s_chunkOffsetValid) {
            glUniform3f(s_shader.uChunkOffset, s_chunkOffset.x, s_chunkOffset.y, s_chunkOffset.z);
        } else {
            glUniform3f(s_shader.uChunkOffset, 0.0f, 0.0f, 0.0f);
        }
    }
    if (s_shader.uShaderPreset >= 0) {
        static int s_appliedPreset = -1;
        extern int s_currentShaderPreset;
        if (s_appliedPreset != s_currentShaderPreset) {
            s_appliedPreset = s_currentShaderPreset;
            glUniform1i(s_shader.uShaderPreset, s_appliedPreset);
        }
    }
    flushMatrices();
}

int s_currentShaderPreset = 0;
void PlatformRenderer_SetShaderPreset(int preset) {
    s_currentShaderPreset = preset;
}


static GLuint s_sVAO_std = 0, s_sVBO_std = 0;
static GLsizeiptr s_streamVBOSize = 0;

static void bindStdAttribs() {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (void*)12);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 32, (void*)20);
    glVertexAttribPointer(3, 3, GL_BYTE, GL_TRUE, 32, (void*)24);
    glVertexAttribIPointer(4, 2, GL_SHORT, 32, (void*)28);
}

static void initStreamingVAOs() {
    glGenVertexArrays(1, &s_sVAO_std);
    glGenBuffers(1, &s_sVBO_std);
    glBindVertexArray(s_sVAO_std);
    glBindBuffer(GL_ARRAY_BUFFER, s_sVBO_std);
    bindStdAttribs();
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Chunk buffer pool (shared, protected by s_glCallMtx)
struct ChunkDrawCall {
    GLenum prim;
    GLint first;
    GLsizei count;
};

// Thread-safe deferred deletion of OpenGL ES buffers and VAOs.
// In Android (Adreno/Mali), calling glDeleteBuffers/glDeleteVertexArrays from
// background worker threads (like eRenderChunkUpdateThread) without an active EGL context
// causes driver crashes, memory leaks and GL_INVALID_OPERATION.
static std::vector<GLuint> s_deferredDeleteVBOs;
static std::vector<GLuint> s_deferredDeleteVAOs;
static std::mutex s_deferredDeleteMtx;

static void QueueDeferredGLDeletes(GLuint vbo, GLuint vao) {
    if (!vbo && !vao) return;
    std::lock_guard<std::mutex> lock(s_deferredDeleteMtx);
    if (vbo) s_deferredDeleteVBOs.push_back(vbo);
    if (vao) s_deferredDeleteVAOs.push_back(vao);
}

static void ProcessDeferredGLDeletes() {
    std::vector<GLuint> vbos;
    std::vector<GLuint> vaos;
    {
        std::lock_guard<std::mutex> lock(s_deferredDeleteMtx);
        if (s_deferredDeleteVBOs.empty() && s_deferredDeleteVAOs.empty()) return;
        vbos.swap(s_deferredDeleteVBOs);
        vaos.swap(s_deferredDeleteVAOs);
    }
    if (!vaos.empty()) {
        glDeleteVertexArrays((GLsizei)vaos.size(), vaos.data());
    }
    if (!vbos.empty()) {
        glDeleteBuffers((GLsizei)vbos.size(), vbos.data());
    }
}

struct ChunkBuffer {
    GLuint vbo = 0;
    // each chunks has its one VAO now
    GLuint vao = 0;
    std::vector<ChunkDrawCall> draws;
    std::vector<uint8_t> rawVerts;
    bool valid = false;
    bool vboReady = false;
    void destroy() {
        if (vbo || vao) {
            QueueDeferredGLDeletes(vbo, vao);
            vbo = 0;
            vao = 0;
        }
        draws.clear();
        rawVerts.clear();
        valid = false;
        vboReady = false;
    }
};

static std::unordered_map<int, ChunkBuffer> s_chunkPool;
static int s_nextListBase = 1;

// Per-thread recording state
static thread_local int s_recListId = -1;
static thread_local std::vector<uint8_t> s_recVerts;
static thread_local std::vector<ChunkDrawCall> s_recDraws;

// Primitive helpers
static bool isQuadPrim(int pt) {
    return (pt == 0x0007 /*GL_QUADS*/ ||
            pt == (int)GLRenderer::PRIMITIVE_TYPE_QUAD_LIST);
}

static GLenum mapPrim(int pt) {
    if (isQuadPrim(pt)) return GL_TRIANGLES;
    switch (pt) {
        case 0:
            return GL_TRIANGLES;
        case 1:
            return GL_LINES;
        case 2:
            return GL_TRIANGLE_FAN;
        case 3:
            return GL_LINE_STRIP;
        case 4:
            return GL_TRIANGLES;
        case 5:
            return GL_TRIANGLE_STRIP;
        case 6:
            return GL_TRIANGLE_FAN;
        default:
            return GL_TRIANGLES;
    }
}

// MARK: Renderer impl

// Initialises the renderer
void GLRenderer::Initialise() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "[4J_Render] SDL_Init: %s\n", SDL_GetError());
        return;
    }
    SDL_DisplayMode dm;
#if defined(__ANDROID__) || defined(ANDROID)
    if (SDL_GetCurrentDisplayMode(0, &dm) == 0) {
        s_windowWidth = std::max(dm.w, dm.h);
        s_windowHeight = std::min(dm.w, dm.h);
    }
    s_fullscreen = true;
#else
    if (s_reqWidth > 0 && s_reqHeight > 0) {
        s_windowWidth = s_reqWidth;
        s_windowHeight = s_reqHeight;
    } else if (SDL_GetCurrentDisplayMode(0, &dm) == 0) {
        s_windowWidth = (int)(dm.w * 0.4f);
        s_windowHeight = (int)(dm.h * 0.4f);
    }
#endif
#ifdef GLES
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
#endif
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    Uint32 wf = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
#if defined(__ANDROID__) || defined(ANDROID)
    wf |= SDL_WINDOW_FULLSCREEN;
#else
    if (s_fullscreen) wf |= SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif
    s_window = SDL_CreateWindow("Minecraft Console Edition",
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                s_windowWidth, s_windowHeight, wf);
    if (!s_window) {
        fprintf(stderr, "[4J_Render] Window: %s\n", SDL_GetError());
        return;
    }
    s_glContext = SDL_GL_CreateContext(s_window);
    if (!s_glContext) {
        fprintf(stderr, "[4J_Render] Context: %s\n", SDL_GetError());
        return;
    }
#ifndef GLES
    gl3_load();
#endif
    const char* glVendor = (const char*)glGetString(GL_VENDOR);
    const char* glRenderer = (const char*)glGetString(GL_RENDERER);
    const char* glVersion = (const char*)glGetString(GL_VERSION);
    const char* glslVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    Log::info("[4J_Render] GL Vendor: %s\n", glVendor ? glVendor : "null");
    Log::info("[4J_Render] GL Renderer: %s\n", glRenderer ? glRenderer : "null");
    Log::info("[4J_Render] GL Version: %s\n", glVersion ? glVersion : "null");
    Log::info("[4J_Render] GLSL Version: %s\n", glslVersion ? glslVersion : "null");
    int fw = 0, fh = 0;
    SDL_GL_GetDrawableSize(s_window, &fw, &fh);
    if (fw <= 0 || fh <= 0) {
        SDL_GetWindowSize(s_window, &fw, &fh);
    }
    onFramebufferResize(fw, fh);
    Log::info("[4J_Render] Initial window size: %dx%d, framebuffer: %dx%d\n", s_windowWidth, s_windowHeight, fw, fh);

    appendDiagnosticLog("=== 4J_Render Startup Diagnostics ===\n");
    appendDiagnosticLog("GL Vendor: %s\n", glVendor ? glVendor : "null");
    appendDiagnosticLog("GL Renderer: %s\n", glRenderer ? glRenderer : "null");
    appendDiagnosticLog("GL Version: %s\n", glVersion ? glVersion : "null");
    appendDiagnosticLog("GLSL Version: %s\n", glslVersion ? glslVersion : "null");
    appendDiagnosticLog("Window size: %dx%d, Framebuffer: %dx%d\n", s_windowWidth, s_windowHeight, fw, fh);

    glShadowSetDepthTest(true);
    ::glDepthFunc(GL_LEQUAL);
#ifdef GLES
    glClearDepthf(1.0f);
#else
    glClearDepth(1.0);
#endif
    glShadowSetBlend(true);
    glShadowSetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadowSetCull(true);
    ::glCullFace(GL_BACK);
    ::glClearColor(0.1f, 0.12f, 0.16f, 1.0f);
    glViewport(0, 0, s_windowWidth, s_windowHeight);
    s_shader.build(VERT_SRC, FRAG_SRC);
    if (!s_shader.prog) {
        Log::info("[4J_Render] FATAL: Shader program failed to compile or link!\n");
        appendDiagnosticLog("[4J_Render] FATAL: Shader program failed to compile or link!\n");
        ::glClearColor(0.8f, 0.1f, 0.1f, 1.0f); // Bright red indicates shader failure
    } else {
        Log::info("[4J_Render] Shader program compiled & linked successfully (prog=%u)\n", s_shader.prog);
        appendDiagnosticLog("[4J_Render] Shader program compiled & linked successfully (prog=%u)\n", s_shader.prog);
    }
    initStreamingVAOs();

    if (!s_defaultTex) {
        glGenTextures(1, &s_defaultTex);
        glBindTexture(GL_TEXTURE_2D, s_defaultTex);
        uint32_t whitePixel = 0xFFFFFFFF;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, &whitePixel);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, s_defaultTex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, s_defaultTex);
    }
    appendDiagnosticLog("Default 1x1 white texture: %u, VAO: %u, VBO: %u\n", s_defaultTex, s_sVAO_std, s_sVBO_std);

    s_mainThreadId = std::this_thread::get_id();
    s_mainThreadSet = true;
    s_glCtx = s_glContext;

    SDL_GL_MakeCurrent(s_window, s_glContext);
#if !defined(__ANDROID__) && !defined(ANDROID)
    for (int i = 0; i < MAX_SHARED_CTXS; i++) {
        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
        SDL_Window* w = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED, 1, 1,
                                         SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
        if (!w) break;
        SDL_GLContext ctx = SDL_GL_CreateContext(w);
        if (!ctx) {
            SDL_DestroyWindow(w);
            break;
        }
        s_sharedWins[s_sharedCtxCount] = w;
        s_sharedCtxs[s_sharedCtxCount] = ctx;
        s_sharedCtxCount++;
    }
#endif
    SDL_GL_MakeCurrent(s_window, s_glContext);
    pushRenderState();

#if defined(__ANDROID__) || defined(ANDROID) || defined(ENABLE_VSYNC)
    SDL_GL_SetSwapInterval(1);
#else
    SDL_GL_SetSwapInterval(0);
#endif
}

void GLRenderer::InitialiseContext() {
    if (!s_window) return;

    if (s_mainThreadSet && std::this_thread::get_id() == s_mainThreadId) {
        SDL_GL_MakeCurrent(s_window, s_glContext);
        s_glCtx = s_glContext;
        return;
    }

    if (s_glCtx) {
        for (int i = 0; i < s_sharedCtxCount; i++) {
            if (s_sharedCtxs[i] == s_glCtx) {
                SDL_GL_MakeCurrent(s_sharedWins[i], s_glCtx);
                return;
            }
        }
        return;
    }

    SDL_GLContext shared = nullptr;
    {
        std::lock_guard<std::mutex> lk(s_sharedMtx);
        if (s_nextSharedCtx < s_sharedCtxCount)
            shared = s_sharedCtxs[s_nextSharedCtx++];
    }
    if (!shared) return;

    for (int i = 0; i < s_sharedCtxCount; i++) {
        if (s_sharedCtxs[i] == shared)
            SDL_GL_MakeCurrent(s_sharedWins[i], shared);
    }
    s_glCtx = shared;
}

void GLRenderer::StartFrame() {
    ProcessDeferredGLDeletes();
    Set_matrixDirty();
    int w = 0, h = 0;
    SDL_GL_GetDrawableSize(s_window, &w, &h);
    if (w <= 0 || h <= 0) {
        SDL_GetWindowSize(s_window, &w, &h);
    }
#if defined(__ANDROID__) || defined(ANDROID)
    if (w < h) {
        std::swap(w, h);
    }
#endif
    s_windowWidth = w > 0 ? w : 1;
    s_windowHeight = h > 0 ? h : 1;
    glViewport(0, 0, s_windowWidth, s_windowHeight);
}

void GLRenderer::Present() {
    if (!s_window) return;
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT)
            s_shouldClose = true;
        else if (ev.type == SDL_WINDOWEVENT) {
            if (ev.window.event == SDL_WINDOWEVENT_CLOSE)
                s_shouldClose = true;
            else if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
                onFramebufferResize(ev.window.data1, ev.window.data2);
        }
    }
    glFlush();
    SDL_GL_SwapWindow(s_window);

#if !defined(__ANDROID__)
    // 60 FPS frame limiter for platforms without hardware vsync compositor
    static auto s_lastFrameTime = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    auto elapsedUs = std::chrono::duration_cast<std::chrono::microseconds>(now - s_lastFrameTime).count();
    const int64_t targetFrameUs = 16666; // 60 FPS
    if (elapsedUs > 0 && elapsedUs < targetFrameUs) {
        std::this_thread::sleep_for(std::chrono::microseconds(targetFrameUs - elapsedUs));
    }
    s_lastFrameTime = std::chrono::steady_clock::now();
#endif
}

void GLRenderer::SetWindowSize(int w, int h) {
    s_reqWidth = w;
    s_reqHeight = h;
}

void GLRenderer::SetFullscreen(bool fs) { s_fullscreen = fs; }

bool GLRenderer::ShouldClose() { return !s_window || s_shouldClose; }

void GLRenderer::GetFramebufferSize(int& w, int& h) {
    w = s_windowWidth;
    h = s_windowHeight;
}

void GLRenderer::Close() { s_window = nullptr; }

void GLRenderer::Shutdown() {
    {
        std::lock_guard<std::mutex> lk(s_glCallMtx);
        for (auto& kv : s_chunkPool) kv.second.destroy();
        s_chunkPool.clear();
    }
    glDeleteVertexArrays(1, &s_sVAO_std);
    glDeleteBuffers(1, &s_sVBO_std);
    if (s_shader.prog) glDeleteProgram(s_shader.prog);
    if (s_glContext) {
        SDL_GL_DeleteContext(s_glContext);
        s_glContext = nullptr;
    }
    if (s_window) {
        SDL_DestroyWindow(s_window);
        s_window = nullptr;
    }
    for (int i = 0; i < s_sharedCtxCount; i++) {
        if (s_sharedCtxs[i]) SDL_GL_DeleteContext(s_sharedCtxs[i]);
        if (s_sharedWins[i]) SDL_DestroyWindow(s_sharedWins[i]);
    }
    SDL_Quit();
}

void GLRenderer::DrawVertices(ePrimitiveType ptype, int count, void* dataIn,
                              eVertexType vType, ePixelShaderType) {
    if (count <= 0 || !dataIn) return;

    bool wasQuad = isQuadPrim((int)ptype);
    GLenum glMode = mapPrim((int)ptype);
    static thread_local std::vector<uint8_t> stdData;
    static thread_local std::vector<uint8_t> triData;
    stdData.clear();
    triData.clear();

    if (vType == VERTEX_TYPE_COMPRESSED) {
        stdData.resize((size_t)count * 32);
        const int16_t* src = (const int16_t*)dataIn;
        uint8_t* dst = stdData.data();
        for (int i = 0; i < count; i++) {
            float* dstF = (float*)dst;

            // Position: int16 / 1024
            dstF[0] = src[0] / 1024.0f;
            dstF[1] = src[1] / 1024.0f;
            dstF[2] = src[2] / 1024.0f;

            // int16 / 8192
            dstF[3] = src[4] / 8192.0f;
            dstF[4] = src[5] / 8192.0f;

            // RGB565 −32768
            {
                uint16_t packed = (uint16_t)((int)src[3] + 32768);
                dst[20] = 255;
                dst[21] = (uint8_t)((packed & 0x1F) * 255 / 31);          // B
                dst[22] = (uint8_t)(((packed >> 5) & 0x3F) * 255 / 63);   // G
                dst[23] = (uint8_t)(((packed >> 11) & 0x1F) * 255 / 31);  // R
            }
            dst[24] = 0;
            dst[25] = 127;  // +Y (up)
            dst[26] = 0;
            dst[27] = 0;

            // Lightmap
            {
                int16_t* dstS = (int16_t*)(dst + 28);
                dstS[0] = src[6];
                dstS[1] = src[7];
            }

            src += 8;
            dst += 32;
        }
        dataIn = stdData.data();
    }

    static const size_t stride = 32;
    if (wasQuad) {
        int numQuads = count / 4;
        int triVerts = numQuads * 6;
        triData.resize((size_t)triVerts * stride);
        const uint8_t* src = (const uint8_t*)dataIn;
        uint8_t* dst = triData.data();
        for (int q = 0; q < numQuads; q++) {
            const uint8_t* v0 = src + (q * 4 + 0) * stride;
            const uint8_t* v1 = src + (q * 4 + 1) * stride;
            const uint8_t* v2 = src + (q * 4 + 2) * stride;
            const uint8_t* v3 = src + (q * 4 + 3) * stride;
            // Triangle 1: 0,1,2
            memcpy(dst + 0 * stride, v0, stride);
            memcpy(dst + 1 * stride, v1, stride);
            memcpy(dst + 2 * stride, v2, stride);
            // Triangle 2: 0,2,3
            memcpy(dst + 3 * stride, v0, stride);
            memcpy(dst + 4 * stride, v2, stride);
            memcpy(dst + 5 * stride, v3, stride);
            dst += 6 * stride;
        }
        dataIn = triData.data();
        count = triVerts;
        glMode = GL_TRIANGLES;
    }

    size_t bytes = (size_t)count * stride;

    if (s_recListId >= 0) {
        int first = (int)(s_recVerts.size() / stride);
        s_recVerts.insert(s_recVerts.end(), (const uint8_t*)dataIn,
                          (const uint8_t*)dataIn + bytes);
        s_recDraws.push_back({glMode, first, (GLsizei)count});
        return;
    }

    std::lock_guard<std::mutex> lk(s_glCallMtx);
    pushRenderState();

    if (s_shader.uChunkOffset >= 0) {
        glUniform3f(s_shader.uChunkOffset, 0.0f, 0.0f, 0.0f);
        s_chunkOffset = {0.0f, 0.0f, 0.0f};
        s_chunkOffsetValid = true;
    }

    if (!s_sVAO_std) {
        initStreamingVAOs();
    }
    if (s_sVAO_std) {
        glBindVertexArray(s_sVAO_std);
    }
    glBindBuffer(GL_ARRAY_BUFFER, s_sVBO_std);

    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)bytes, dataIn, GL_STREAM_DRAW);
    bindStdAttribs();
    s_streamVBOSize = (GLsizeiptr)bytes;

    static int s_loggedDraws = 0;
    if (s_loggedDraws < 10) {
        s_loggedDraws++;
        GLenum errB = glGetError();
        appendDiagnosticLog("Draw #%d: mode=%u, count=%d, bytes=%zu, prog=%u, vao=%u, vbo=%u, errB=0x%x\n",
            s_loggedDraws, glMode, count, bytes, s_shader.prog, s_sVAO_std, s_sVBO_std, errB);
    }

    glDrawArrays(glMode, 0, count);

    if (s_loggedDraws <= 10) {
        GLenum errA = glGetError();
        if (errA != GL_NO_ERROR) {
            appendDiagnosticLog("Draw #%d glDrawArrays error: 0x%x\n", s_loggedDraws, errA);
        }
    }

    if (s_sVAO_std) {
        glBindVertexArray(0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLRenderer::ReadPixels(int x, int y, int w, int h, void* buf) {
    if (!buf) return;
    std::lock_guard<std::mutex> lk(s_glCallMtx);
    glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buf);
}

int GLRenderer::CBuffCreate(int count) {
    std::lock_guard<std::mutex> lk(s_glCallMtx);
    int b = s_nextListBase;
    s_nextListBase += count;
    return b;
}

void GLRenderer::CBuffDelete(int first, int count) {
    std::lock_guard<std::mutex> lk(s_glCallMtx);
    for (int i = first; i < first + count; i++) {
        auto it = s_chunkPool.find(i);
        if (it != s_chunkPool.end()) {
            it->second.destroy();
            s_chunkPool.erase(it);
        }
    }
}

void GLRenderer::CBuffDeleteAll() {
    std::lock_guard<std::mutex> lk(s_glCallMtx);
    size_t count = s_chunkPool.size();
    for (auto& kv : s_chunkPool) {
        kv.second.destroy();
    }
    s_chunkPool.clear();
    s_nextListBase = 1;
    ProcessDeferredGLDeletes();
    MCPL_LOGI("CBuffDeleteAll: Cleared %zu chunk buffers and flushed deferred GL deletes", count);
}

void GLRenderer::CBuffStart(int index, bool) {
    s_recListId = index;
    s_recVerts.clear();
    s_recDraws.clear();
}

void GLRenderer::CBuffEnd() {
    if (s_recListId < 0) return;
    std::lock_guard<std::mutex> lk(s_glCallMtx);
    ChunkBuffer& cb = s_chunkPool[s_recListId];
    cb.destroy();
    if (s_recVerts.empty()) {
        s_chunkPool.erase(s_recListId);
        s_recListId = -1;
        return;
    }
    cb.rawVerts = std::move(s_recVerts);
    cb.draws = std::move(s_recDraws);
    cb.valid = true;
    cb.vboReady = false;
    s_recListId = -1;
}

void GLRenderer::CBuffClear(int index) {
    std::lock_guard<std::mutex> lk(s_glCallMtx);
    auto it = s_chunkPool.find(index);
    if (it != s_chunkPool.end()) {
        it->second.destroy();
        s_chunkPool.erase(it);
    }
}

bool GLRenderer::CBuffCall(int index, bool) {
    std::lock_guard<std::mutex> lk(s_glCallMtx);
    auto it = s_chunkPool.find(index);
    if (it == s_chunkPool.end() || !it->second.valid) {
        return false;
    }
    ChunkBuffer& cb = it->second;
    if (!cb.vboReady) {
        if (cb.rawVerts.empty()) {
            return false;
        }

        glGenVertexArrays(1, &cb.vao);
        glGenBuffers(1, &cb.vbo);
        glBindVertexArray(cb.vao);
        glBindBuffer(GL_ARRAY_BUFFER, cb.vbo);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cb.rawVerts.size(),
                     cb.rawVerts.data(), GL_STATIC_DRAW);
        bindStdAttribs();  // single time bindstdattrib
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        cb.rawVerts.clear();
        cb.rawVerts.shrink_to_fit();
        cb.vboReady = true;
    }

    pushRenderState();

    glBindVertexArray(cb.vao);
    for (const auto& dc : cb.draws) glDrawArrays(dc.prim, dc.first, dc.count);
    glBindVertexArray(0);

    return true;
}

void GLRenderer::MatrixMode(int t) {
    if (t == GL_PROJECTION)
        s_matMode = 1;
    else if (t == GL_TEXTURE)
        s_matMode = 2;
    else
        s_matMode = 0;
}

void GLRenderer::MatrixSetIdentity() {
    activeStack().load(glm::mat4(1.f));
    markMatrixDirty();
    if (s_matMode == 0) markNormalDirty();
}
void GLRenderer::MatrixPush() {
    activeStack().push();
    // push doesn't change cur() so no dirty needed but mark anyway to be safe
    // ;w;
    markMatrixDirty();
    if (s_matMode == 0) markNormalDirty();
}
void GLRenderer::MatrixPop() {
    activeStack().pop();
    markMatrixDirty();
    if (s_matMode == 0) markNormalDirty();
}
void GLRenderer::MatrixTranslate(float x, float y, float z) {
    activeStack().mul(glm::translate(glm::mat4(1.f), {x, y, z}));
    markMatrixDirty();
    if (s_matMode == 0) markNormalDirty();
}
void GLRenderer::MatrixRotate(float a, float x, float y, float z) {
    activeStack().mul(glm::rotate(glm::mat4(1.f), glm::radians(a), {x, y, z}));
    markMatrixDirty();
    if (s_matMode == 0) markNormalDirty();
}
void GLRenderer::MatrixScale(float x, float y, float z) {
    activeStack().mul(glm::scale(glm::mat4(1.f), {x, y, z}));
    markMatrixDirty();
    if (s_matMode == 0) markNormalDirty();
}
void GLRenderer::MatrixPerspective(float fovy, float asp, float zn, float zf) {
    s_proj.cur() = glm::perspective(glm::radians(fovy), asp, zn, zf);
    markMatrixDirty();
}
void GLRenderer::MatrixOrthogonal(float l, float r, float b, float t, float zn,
                                  float zf) {
    s_proj.cur() = glm::ortho(l, r, b, t, zn, zf);
    markMatrixDirty();
}
void GLRenderer::MatrixMult(float* m) {
    activeStack().mul(glm::make_mat4(m));
    markMatrixDirty();
    if (s_matMode == 0) markNormalDirty();
}
const float* GLRenderer::MatrixGet(int t) {
    static float buf[16];
    glm::mat4* m = (t == GL_MODELVIEW_MATRIX)    ? &s_mv.cur()
                   : (t == GL_PROJECTION_MATRIX) ? &s_proj.cur()
                                                 : nullptr;
    if (m) memcpy(buf, glm::value_ptr(*m), 64);
    return buf;
}

void GLRenderer::Set_matrixDirty() {
    // iggy wipes opengl state
    s_boundProgram = 0;
    s_rs_dirty_mask = 0xFFFFFFFF;
    s_gl_shadow_mask = 0;
    s_normalMatDirty = true;  // normal matrix dirt after iggy reset
    s_matDirty = true;
    s_chunkOffsetValid = false;
    if (s_shader.prog) {
        glUseProgram(s_shader.prog);
        s_boundProgram = s_shader.prog;
    }
}

void GLRenderer::Clear(int f) { glClear(f); }
void GLRenderer::SetClearColour(const float c[4]) {
    glClearColor(c[0], c[1], c[2], c[3]);
}
bool GLRenderer::IsWidescreen() { return true; }
bool GLRenderer::IsHiDef() { return true; }
void GLRenderer::StateSetColour(float r, float g, float b, float a) {
    glm::vec4 v = {r, g, b, a};
    if (s_rs.baseColor != v) {
        s_rs.baseColor = v;
        markDirty(DIRTY_BASECOLOR);
    }
}
void GLRenderer::SetChunkOffset(float x, float y, float z) {
    if (s_shader.uChunkOffset < 0) return;
    glm::vec3 v = {x, y, z};
    if (!s_chunkOffsetValid || s_chunkOffset != v) {
        s_chunkOffset = v;
        s_chunkOffsetValid = true;
    }
    if (s_boundProgram == s_shader.prog) {
        glUniform3f(s_shader.uChunkOffset, x, y, z);
    }
}
void GLRenderer::StateSetDepthMask(bool e) {
    glShadowSetDepthMask(e ? GL_TRUE : GL_FALSE);
}
void GLRenderer::StateSetBlendEnable(bool e) { glShadowSetBlend(e); }
void GLRenderer::StateSetBlendFunc(int s, int d) { glShadowSetBlendFunc(s, d); }
void GLRenderer::StateSetDepthFunc(int f) { ::glDepthFunc(f); }
void GLRenderer::StateSetFaceCull(bool e) { glShadowSetCull(e); }
void GLRenderer::StateSetFaceCullCW(bool e) {
    glShadowSetFrontFace(e ? GL_CW : GL_CCW);
}
void GLRenderer::StateSetLineWidth(float w) {
#ifndef GLES
    glShadowSetLineWidth(w);
#else
    (void)w;
#endif
}
void GLRenderer::StateSetWriteEnable(bool r, bool g, bool b, bool a) {
    glShadowSetColorMask(r, g, b, a);
}
void GLRenderer::StateSetDepthTestEnable(bool e) { glShadowSetDepthTest(e); }
void GLRenderer::StateSetAlphaTestEnable(bool e) {
    if (s_rs.alphaTest != e) {
        s_rs.alphaTest = e;
        markDirty(DIRTY_ALPHA);
    }
}
void GLRenderer::StateSetAlphaFunc(int, float p) {
    if (s_rs.alphaRef != p) {
        s_rs.alphaRef = p;
        markDirty(DIRTY_ALPHA);
    }
}
void GLRenderer::StateSetDepthSlopeAndBias(float s, float b) {
    glShadowSetPolygonOffset(s, b);
}
void GLRenderer::StateSetBlendFactor(unsigned int col) {
    float a = ((col >> 24) & 0xFF) / 255.f;
    float r = ((col >> 16) & 0xFF) / 255.f;
    float g = ((col >> 8) & 0xFF) / 255.f;
    float b = (col & 0xFF) / 255.f;
    glBlendColor(r, g, b, a);
}
void GLRenderer::StateSetFogEnable(bool e) {
    if (s_rs.fogEnable != e) {
        s_rs.fogEnable = e;
        markDirty(DIRTY_FOG);
    }
}
void GLRenderer::StateSetFogMode(int mode) {
    int v = (mode == GL_LINEAR) ? 1
            : (mode == GL_EXP)  ? 2
            : (mode == 0x0801)  ? 3
                                : 0;
    if (s_rs.fogMode != v) {
        s_rs.fogMode = v;
        markDirty(DIRTY_FOG);
    }
}
void GLRenderer::StateSetFogNearDistance(float d) {
    if (s_rs.fogStart != d) {
        s_rs.fogStart = d;
        markDirty(DIRTY_FOG);
    }
}
void GLRenderer::StateSetFogFarDistance(float d) {
    if (s_rs.fogEnd != d) {
        s_rs.fogEnd = d;
        markDirty(DIRTY_FOG);
    }
}
void GLRenderer::StateSetFogDensity(float d) {
    if (s_rs.fogDensity != d) {
        s_rs.fogDensity = d;
        markDirty(DIRTY_FOG);
    }
}
void GLRenderer::StateSetFogColour(float r, float g, float b) {
    glm::vec4 v = {r, g, b, 1};
    if (s_rs.fogColor != v) {
        s_rs.fogColor = v;
        markDirty(DIRTY_FOG);
    }
}
void GLRenderer::StateSetLightingEnable(bool e) {
    if (s_rs.lighting != e) {
        s_rs.lighting = e;
        markDirty(DIRTY_LIGHTING);
    }
}
void GLRenderer::StateSetLightColour(int, float r, float g, float b) {
    glm::vec3 v = {r, g, b};
    if (s_rs.ldiff != v) {
        s_rs.ldiff = v;
        markDirty(DIRTY_LIGHTING);
    }
}
void GLRenderer::StateSetLightAmbientColour(float r, float g, float b) {
    glm::vec3 v = {r, g, b};
    if (s_rs.lamb != v) {
        s_rs.lamb = v;
        markDirty(DIRTY_LIGHTING);
    }
}
void GLRenderer::StateSetLightDirection(int light, float x, float y, float z) {
    glm::vec3 d = glm::normalize(glm::mat3(s_mv.cur()) * glm::vec3(x, y, z));
    if (light == 0) {
        if (s_rs.l0 != d) {
            s_rs.l0 = d;
            markDirty(DIRTY_LIGHTING);
        }
    } else {
        if (s_rs.l1 != d) {
            s_rs.l1 = d;
            markDirty(DIRTY_LIGHTING);
        }
    }
}
void GLRenderer::StateSetViewport(eViewportType) {
    glViewport(0, 0, s_windowWidth, s_windowHeight);
}
void GLRenderer::StateSetVertexTextureUV(float u, float v) {
    glm::vec2 val = {u, v};
    if (s_rs.globalLM != val) {
        s_rs.globalLM = val;
        markDirty(DIRTY_GLOBAL_LM);
    }
}
void GLRenderer::StateSetStencil(int fn, uint8_t ref, uint8_t fmask,
                                 uint8_t wmask) {
    glShadowSetStencil(fn, ref, fmask, wmask);
}
void GLRenderer::StateSetTextureEnable(bool e) {
    if (s_rs.activeTexture == 0 && s_rs.useTexture != e) {
        s_rs.useTexture = e;
        markDirty(DIRTY_TEXTURE);
    }
}
void GLRenderer::StateSetActiveTexture(int tex) {
    s_rs.activeTexture = (tex == 0x84C1 /*GL_TEXTURE1*/) ? 1 : 0;
    ::glActiveTexture(tex);
}

int GLRenderer::TextureCreate() {
    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return (int)id;
}
void GLRenderer::TextureFree(int i) {
    GLuint id = (GLuint)i;
    glDeleteTextures(1, &id);
}
void GLRenderer::TextureBind(int idx) {
    glActiveTexture(GL_TEXTURE0);
    if (idx <= 0 && s_defaultTex) {
        glBindTexture(GL_TEXTURE_2D, s_defaultTex);
    } else {
        glBindTexture(GL_TEXTURE_2D, idx < 0 ? 0 : (GLuint)idx);
    }
}
void GLRenderer::TextureBindVertex(int idx, bool scaleLight) {
    if (idx < 0) {
        if (s_rs.useLightmap) {
            s_rs.useLightmap = false;
            markDirty(DIRTY_TEXTURE);
        }
        glActiveTexture(GL_TEXTURE1);
        if (s_defaultTex) {
            glBindTexture(GL_TEXTURE_2D, s_defaultTex);
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glActiveTexture(GL_TEXTURE0);
        return;
    }
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, (GLuint)idx);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glActiveTexture(GL_TEXTURE0);
    if (!s_rs.useLightmap) {
        s_rs.useLightmap = true;
        markDirty(DIRTY_TEXTURE);
    }
    glm::vec4 newLmt = scaleLight
                           ? glm::vec4{1.f, 1.f, 8.f / 256.f, 8.f / 256.f}
                           : glm::vec4{1.f, 1.f, 0.f, 0.f};
    if (s_rs.lmt != newLmt) {
        s_rs.lmt = newLmt;
        markDirty(DIRTY_LMT);
    }
}
void GLRenderer::TextureSetTextureLevels(int l) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, l > 0 ? l - 1 : 0);
    if (l > 1)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_NEAREST_MIPMAP_LINEAR);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
int GLRenderer::TextureGetTextureLevels() { return 1; }
void GLRenderer::TextureData(int w, int h, void* d, int lvl, eTextureFormat) {
    glTexImage2D(GL_TEXTURE_2D, lvl, GL_RGBA, w, h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, d);
    if (lvl == 0) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        GLint maxLvl = 1000;
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, &maxLvl);
        if (maxLvl <= 0 || maxLvl == 1000) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}
void GLRenderer::TextureDataUpdate(int xo, int yo, int w, int h, void* d,
                                   int lvl) {
    glTexSubImage2D(GL_TEXTURE_2D, lvl, xo, yo, w, h, GL_RGBA, GL_UNSIGNED_BYTE,
                    d);
}
void GLRenderer::TextureSetParam(int p, int v) {
    glTexParameteri(GL_TEXTURE_2D, p, v);
}

static int stbLoad(unsigned char* data, int w, int h, D3DXIMAGE_INFO* info,
                   int** out) {
    int* px = new int[w * h];
    for (int i = 0; i < w * h; i++) {
        unsigned char r = data[i * 4], g = data[i * 4 + 1], b = data[i * 4 + 2],
                      a = data[i * 4 + 3];
        px[i] = (a << 24) | (r << 16) | (g << 8) | b;
    }
    if (info) {
        info->Width = w;
        info->Height = h;
    }
    *out = px;
    return 0;  // Success
}
int GLRenderer::LoadTextureData(const char* fn, D3DXIMAGE_INFO* i, int** o) {
    int w, h, c;
    unsigned char* d = stbi_load(fn, &w, &h, &c, 4);
    if (!d) return -1;  // Failure
    int hr = stbLoad(d, w, h, i, o);
    stbi_image_free(d);
    return hr;
}
int GLRenderer::LoadTextureData(uint8_t* pb, uint32_t nb, D3DXIMAGE_INFO* i,
                                int** o) {
    int w, h, c;
    unsigned char* d = stbi_load_from_memory(pb, (int)nb, &w, &h, &c, 4);
    if (!d) return -1;  // Failure
    int hr = stbLoad(d, w, h, i, o);
    stbi_image_free(d);
    return hr;
}

// TODO: TO REMOVE SOON.
void GLRenderer::UpdateGamma(unsigned short usGamma) {
    constexpr unsigned short GAMMA_MAX = 32768;
    s_rs.gamma = 0.5f + ((float)(usGamma) * (1.0f / GAMMA_MAX));
}

// MARK: C hooks

int glGenTextures_4J() {
    return PlatformRenderer.TextureCreate();
}

void glGenTextures_4J(int n, unsigned int* textures) {
    for (int i = 0; i < n; i++) {
        textures[i] = (unsigned int)PlatformRenderer.TextureCreate();
    }
}

void glDeleteTextures_4J(int id) {
    GLuint uid = (GLuint)id;
    ::glDeleteTextures(1, &uid);
}

void glDeleteTextures_4J(int n, const unsigned int* textures) {
    ::glDeleteTextures(n, textures);
}

// MARK: LinuxStubs

#ifdef GLES
extern "C" {
extern void glClearDepthf(float depth);
void glClearDepth(double depth) { glClearDepthf((float)depth); }
void glTexGeni(unsigned int, unsigned int, int) {}
void glTexGenfv(unsigned int, unsigned int, const float*) {}
void glTexCoordPointer(int, unsigned int, int, const void*) {}
void glNormalPointer(unsigned int, int, const void*) {}
void glColorPointer(int, unsigned int, int, const void*) {}
void glVertexPointer(int, unsigned int, int, const void*) {}
void glEndList(void) {}
void glCallLists(int, unsigned int, const void*) {}
}
#endif

inline int* getIntPtr(IntBuffer* buf) {
    return buf ? (int*)buf->getBuffer() + buf->position() : nullptr;
}
inline void* getBytePtr(ByteBuffer* buf) {
    return buf ? (char*)buf->getBuffer() + buf->position() : nullptr;
}

void glGenTextures_4J(IntBuffer* buf) {
    if (!buf) return;
    int n = buf->limit() - buf->position();
    int* dst = getIntPtr(buf);
    for (int i = 0; i < n; i++) dst[i] = PlatformRenderer.TextureCreate();
}

void glDeleteTextures_4J(IntBuffer* buf) {
    if (!buf) return;
    int n = buf->limit() - buf->position();
    int* src = getIntPtr(buf);
    for (int i = 0; i < n; i++) PlatformRenderer.TextureFree(src[i]);
}

void glTexImage2D_4J(int target, int level, int internalformat, int width,
                     int height, int border, int format, int type,
                     ByteBuffer* pixels) {
    (void)target;
    (void)internalformat;
    (void)border;
    (void)format;
    (void)type;
    PlatformRenderer.TextureData(width, height, getBytePtr(pixels), level,
                                 IPlatformRenderer::TEXTURE_FORMAT_RxGyBzAw);
}

void glLight_4J(int light, int pname, FloatBuffer* params) {
    const float* p = params->_getDataPointer();
    int idx = (light == 0x4001) ? 1 : 0;
    if (pname == 0x1203)
        PlatformRenderer.StateSetLightDirection(idx, p[0], p[1], p[2]);
    else if (pname == 0x1201)
        PlatformRenderer.StateSetLightColour(idx, p[0], p[1], p[2]);
    else if (pname == 0x1200)
        PlatformRenderer.StateSetLightAmbientColour(p[0], p[1], p[2]);
}

void glLightModel_4J(int pname, FloatBuffer* params) {
    if (pname == 0x0B53) {
        const float* p = params->_getDataPointer();
        PlatformRenderer.StateSetLightAmbientColour(p[0], p[1], p[2]);
    }
}

void glFog_4J(int pname, FloatBuffer* params) {
    const float* p = params->_getDataPointer();
    if (pname == 0x0B66) PlatformRenderer.StateSetFogColour(p[0], p[1], p[2]);
}

void glGetFloat_4J(int pname, FloatBuffer* params) {
    const float* m = PlatformRenderer.MatrixGet(pname);
    if (m) memcpy(params->_getDataPointer(), m, 16 * sizeof(float));
}

void glCallLists_4J(IntBuffer* lists) {
    if (!lists) return;
    int count = lists->limit() - lists->position();
    int* ids = getIntPtr(lists);
    for (int i = 0; i < count; i++)
        (void)PlatformRenderer.CBuffCall(ids[i], false);
}

void glReadPixels_4J(int x, int y, int w, int h, int f, int t, ByteBuffer* p) {
    (void)f;
    (void)t;
    PlatformRenderer.ReadPixels(x, y, w, h, getBytePtr(p));
}

// dead stubs
void glTexCoordPointer_4J(int, int, FloatBuffer*) {}
void glNormalPointer_4J(int, ByteBuffer*) {}
void glColorPointer_4J(int, bool, int, ByteBuffer*) {}
void glVertexPointer_4J(int, int, FloatBuffer*) {}
void glEndList_4J(int) {}
void glTexGen_4J(int, int, FloatBuffer*) {}

#include <stdio.h>
#include <string.h>

void glGetFloat(int pname, FloatBuffer* params) {
    glGetFloat_4J(pname, params);
}