# 🛡️ Guía de Compatibilidad de GPUs y Renderizado en Android (LegacyMCPE)

Este documento detalla la arquitectura de renderizado, las trampas conocidas de controladores gráficos en Android (especialmente ARM Mali y controladores estrictos) y las salvaguardas implementadas para garantizar que **jamás vuelva a ocurrir un fallo de pantalla en blanco/color plano (`glClearColor`)**.

---

## 🛑 Las 6 Trampas Críticas de GPUs Móviles en Android

### 1. El bug de Byte 0 en `#version` (ARM Mali)
* **Causa:** Controladores ARM Mali (Bifrost, Valhall en chips MediaTek Helio/Dimensity y Exynos) rechazan cualquier shader GLSL ES donde la directiva `#version` no esté exactamente en la línea 1, columna 1 (byte 0).
* **Trampa en C++:** Strings en formato crudo `R"GLSL(\n#version 300 es...` introducen un byte `\n` oculto al inicio. En GPUs PowerVR/Adreno funciona, pero en Mali falla la compilación del shader.
* **Salvaguarda:**
  1. Los archivos `.vert` y `.frag` comienzan exactamente con `R"GLSL(#version 300 es` sin saltos de línea ni espacios.
  2. `GLRenderer.cpp` contiene un sanitizador en tiempo de ejecución (`while (*src == '\r' || *src == '\n' || *src == ' ') src++;`) que limpia cualquier whitespace antes de enviar el shader al driver.
  3. `scripts/validate_shaders.py` se ejecuta automáticamente en GitHub Actions para bloquear cualquier commit que rompa esta regla.

### 2. Disparidad de Precisión en Shaders GLSL ES
* **Causa:** OpenGL ES exige declarar precisión (`precision highp float; precision highp int;`). Si un `varying` o variable entre Vertex y Fragment tiene precisiones diferentes, o si en Fragment falta la precisión entera (`highp int`), controladores estrictos provocan errores de enlace silenciosos.
* **Salvaguarda:** Ambos shaders (`vertex_es.vert` y `fragment_es.frag`) declaran idénticamente `precision highp float; precision highp int; precision lowp sampler2D;`.

### 3. Comparación de Color Flotante (`sentinel`)
* **Causa:** Las comparaciones exactas de punto flotante como `all(equal(aColor, vec4(0.0)))` fallan en GPUs móviles debido a imprecisiones de normalización (`0.00001f` en vez de `0.0f`). Cuando fallaba, los quads de interfaz y botones se coloreaban con alpha `0` y eran descartados por `discard`.
* **Salvaguarda:** Usar umbrales con epsilon:
  ```glsl
  bool sentinel = (aColor.x < 0.02 && aColor.y < 0.02 && aColor.z < 0.02 && aColor.w < 0.02);
  ```

### 4. Samplers Incompletos y Texturas Vacías
* **Causa:** En OpenGL ES, dibujar con una unidad de textura habilitada que apunta a textura `0` o textura incompleta hace que el driver descarte los píxeles.
* **Salvaguarda:**
  * Se genera e inicializa una textura blanca sólida de 1x1 píxeles (`s_defaultTex`).
  * Toda llamada a `TextureBind()` y `TextureBindVertex()` vincula por defecto `s_defaultTex` a `GL_TEXTURE0` y `GL_TEXTURE1` cuando el id es `<= 0`.

### 5. Arquitectura de Doble Fallback (ES 3.0 -> ES 2.0 / GLSL 1.00)
* **Salvaguarda:** Si por cualquier motivo un dispositivo antiguo o controlador defectuoso no puede compilar o enlazar los shaders GLSL ES 3.0, el motor conmuta automáticamente en caliente a un juego de shaders estándar ES 2.0 / GLSL ES 1.00 (`VERT_SRC_ES2` y `FRAG_SRC_ES2`), garantizando el 100% de compatibilidad en cualquier dispositivo Android.

### 6. Limitador de Fotogramas a 60 FPS (Control Térmico)
* **Causa:** Los bucles de presentación sin límite consumen el 100% de un núcleo de CPU, generando calor extremo y drenaje de batería.
* **Salvaguarda:** Limitador de tiempo de cuadro fijado a 16.6 ms (60 FPS) mediante `std::this_thread::sleep_for` en `GLRenderer::Present()`.

---

## 🔍 Registro de Diagnóstico Automático

El motor escribe automáticamente en cada arranque un informe en:
`/sdcard/Android/data/y.MinecraftLegacyP/files/render_diagnostic.txt`

Contiene:
- Fabricante de GPU (`GL Vendor`)
- Modelo de Chip (`GL Renderer`)
- Versión de OpenGL ES y GLSL
- Estado de compilación y enlace de shaders
- Registro de las primeras 10 llamadas de dibujo con su código de error GL
