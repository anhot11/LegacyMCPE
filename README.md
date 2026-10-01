# Legacy MCPE

<div align="center">

# 🎮 Legacy MCPE
### *Minecraft Console Legacy Edition (TU19 / 1.6.1) for Android*

[![Android Build](https://github.com/anhot11/LegacyMCPE/actions/workflows/build-android.yml/badge.svg)](https://github.com/anhot11/LegacyMCPE/actions/workflows/build-android.yml)
[![Latest Release](https://img.shields.io/github/v/release/anhot11/LegacyMCPE?color=green&label=Release)](https://github.com/anhot11/LegacyMCPE/releases/latest)
[![Platform](https://img.shields.io/badge/Platform-Android%20(ARM64)-brightgreen.svg)](https://github.com/anhot11/LegacyMCPE/releases)
[![License](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)

**Legacy MCPE** is a native Android port of the legendary **Minecraft Console Legacy Edition (Xbox 360 / PS3 - TU19 / 1.6.1)**, rewritten in modern C++23 with authentic Bedrock HD touch controls, full mobile hardware optimizations, and advanced thermal protection.

[English](#features) • [Español](#características-en-español) • [Installation](#-installation--descarga) • [Controls](#-controls--controles) • [Thermal Control](#-thermal-protection--protección-térmica)

</div>

---

## ✨ Features

- 🎮 **Authentic Bedrock & Classic PE HD Touch Controls:**
  - Directional D-Pad with authentic Bedrock arrows and sneak button.
  - Dedicated action buttons: Attack Sword (`HIT`), Interaction Hand (`USE`), Jump (`SPACE`).
  - Top in-game action bar: Third-person Perspective Eye (`F5`), Pause Menu (`ESC`), and Chat (`T`).
  - Hotbar with 9 slots and native 3-dots (`...`) Inventory button.
  - Absolutely **zero letter boxes** — 100% authentic Minecraft high-definition textures.
- 🎛️ **3 In-Game Switchable Control Themes:**
  - **Modern Bedrock (Style 0):** Separated directional buttons with tactile spacing.
  - **Classic Pocket Edition (Style 1):** Classic connected cross D-pad.
  - **Dynamic Joystick (Style 2):** Smooth analog touch joystick.
  - Adjustable size/scale and opacity sliders in the Lunar Client menu.
- 🧼 **Clean Native Menus (Auto-Hiding Controls):**
  - Virtual touch overlay automatically hides (`View.GONE`) in all menus, inventory screens, pause screens, and title screens.
  - Full native 1-tap touch navigation across all UI elements.
- 🖼️ **Modernized Bedrock Menu UI:**
  - Single **"Jugar" (Play)** button on TitleScreen with dedicated tabs for **Mundos (Worlds)** and **Servidores (Servers)**.
  - High-definition item & texture icons across all UI buttons (settings cog, back arrow, create world green plus, delete trashcan, exit door, etc.).
- 🧍 **Interactive 3D Steve in TitleScreen:**
  - Steve stands grounded on the right side of the main menu with interactive head tracking that smoothly follows your finger and idle breathing sway.
- ⚡ **Optimized Mobile Performance (30–60 FPS):**
  - Dynamic chunk column scaling based on render distance (cuts GPU memory load by 75% on Short/Tiny view distance).
  - Sky/clouds rendering bypass when clouds are disabled in options.
  - Instant world generation (< 2 seconds) with optimized 9-chunk ($3 \times 3$) mobile spawn area, eliminating connection timeouts.
- ❄️ **Built-in Hardware Thermal Protection (`modThermalProtection`):**
  - Real-time battery temperature monitoring via Android `BatteryManager`.
  - 4 customizable protection levels: Disabled, Moderate (45°C), Balanced (42°C), and Maximum Savings (38°C) to keep your phone cool during long gameplay sessions.

---

## 🇪🇸 Características (En Español)

- 🎮 **Controles Táctiles HD Nativos de Bedrock y Classic PE:** Flechas direccionales, espada de ataque, mano de interacción, botón de salto, agacharse, perspectiva F5, menú de pausa y chat. Sin letras cuadradas feas.
- 🎛️ **3 Estilos de Control Seleccionables:** Modern Bedrock (botones separados), Classic PE (cruz unida tradicional) y Joystick analógico fluido. Con escala y opacidad ajustables desde el menú de Lunar Client.
- 🧼 **Menús Limpios y Nativos:** Los controles virtuales se ocultan automáticamente en el menú principal, selección de mundos, inventario y pausa. Navegación directa con 1 toque.
- 🖼️ **Interfaz Renovada con Íconos HD:** Botón único "Jugar" con pestañas para Mundos y Servidores, e íconos temáticos en todos los botones del juego.
- 🧍 **Steve 3D Interactivo:** Steve de pie en el menú principal siguiendo con la cabeza la posición de tu dedo en la pantalla.
- ⚡ **Rendimiento Optimizado (30–60 FPS):** Carga dinámica de chunks (reducción del 75% de carga GPU en distancias cortas/mínimas) y bypass de nubes.
- ❄️ **Protección Térmica de Batería:** Monitoreo de temperatura en tiempo real con 4 niveles (Desactivado, Moderado 45°C, Equilibrado 42°C, Máximo Ahorro 38°C) para evitar sobrecalentamiento.
- 🚀 **Carga Instantánea de Mundos (< 2s):** Radio de generación de spawn optimizado a 9 chunks en Android, sin congelamientos ni desconexiones por timeout.

---

## 📥 Installation / Descarga

1. Ve a la pestaña de [**Releases**](https://github.com/anhot11/LegacyMCPE/releases).
2. Descarga la versión más reciente del archivo: **`LegacyMCPE-arm64.apk`**.
3. En tu dispositivo Android, abre el archivo APK descargado e instala la aplicación (permite la instalación de orígenes desconocidos si el sistema lo solicita).
4. Abre **Legacy MCPE** y concede los permisos de almacenamiento necesarios para guardar tus mundos en `/sdcard/LegacyMCPE/`.
5. ¡Disfruta de la auténtica experiencia de Minecraft Console Edition en tu teléfono!

---

## 🕹️ Controls / Controles

| Botón / Control | Acción en el Juego | Equivalente Teclado / Mando |
| :--- | :--- | :--- |
| **D-Pad Flecha Arriba** | Caminar hacia adelante | `W` / Stick Izquierdo Arriba |
| **D-Pad Flecha Abajo** | Caminar hacia atrás | `S` / Stick Izquierdo Abajo |
| **D-Pad Flecha Izquierda** | Desplazamiento a la izquierda | `A` / Stick Izquierdo Izquierda |
| **D-Pad Flecha Derecha** | Desplazamiento a la derecha | `D` / Stick Izquierdo Derecha |
| **Botón Central D-Pad** | Agacharse / Sneak (alternar) | `Shift` / Stick Derecho Clic |
| **Flecha Salto (Derecha)** | Saltar | `Espacio` / Botón `A` |
| **Espada (Derecha)** | Atacar / Romper bloque | Clic Izquierdo / Gatillo Derecho |
| **Mano (Derecha)** | Colocar bloque / Usar objeto | Clic Derecho / Gatillo Izquierdo |
| **Barra Hotbar (1-9)** | Seleccionar ranura rápida | Teclas `1`-`9` / Gatillos Sup. |
| **Botón `...` (Hotbar)** | Abrir Inventario / Crafteo | `E` / Botón `Y` |
| **Ojo (Superior)** | Cambiar Perspectiva (1ª / 3ª persona) | `F5` / Stick Izquierdo Clic |
| **Menú Pausa (Superior)** | Pausar juego y abrir menú | `Escape` / Botón `Start` |
| **Chat (Superior)** | Abrir Chat / Mensajes | `T` |
| **Deslizar en Pantalla** | Mover cámara / Mirar alrededor | Ratón / Stick Derecho |

---

## 🌡️ Thermal Protection / Protección Térmica

Legacy MCPE incluye un sistema pionero de control térmico por hardware para dispositivos móviles. Puedes configurarlo en cualquier momento desde **Mods (Lunar Client) ➔ Thermal Protection**:

- **Off (0):** Sin límite de temperatura.
- **Moderado (1):** Actúa a partir de los **45°C**, introduciendo pausas de microsegundos entre frames para frenar la subida térmica.
- **Equilibrado (2):** Mantiene el dispositivo estable entre **42°C y 45°C**.
- **Máximo Ahorro (3 - Recomendado por defecto):** Mantiene la batería fresca alrededor de **38°C**, garantizando sesiones largas sin degradación de batería ni quemaduras en las manos.

---

## 🛠️ Building from Source / Compilación

El proyecto utiliza CMake / Meson con NDK y toolchain moderno C++23. Se compila automáticamente para Android mediante el workflow de GitHub Actions:

```bash
# Workflow de compilación en GitHub Actions:
.github/workflows/build-android.yml
```

Los artefactos `.apk` generados se firman con `apksigner` y `zipalign` con soporte de arquitectura `arm64-v8a`.

---

## 📋 System Requirements / Requisitos del Sistema

- **Arquitectura:** Android ARM64 (`aarch64` / `arm64-v8a`).
- **Sistema Operativo:** Android 7.0 (Nougat) o superior.
- **Gráficos:** Soporte para OpenGL ES 2.0 / 3.0.
- **Almacenamiento:** Mínimo 200 MB de espacio libre.

---

## 📜 Credits & License

- Basado en el proyecto **Portable LCE** y la ingeniería inversa comunitaria de **Minecraft Console Edition (4J Studios / Mojang)**.
- Mods integrados adaptados de **Lunar Client** para Minecraft Portable LCE.
- Íconos y texturas remasterizadas estilo Bedrock Edition y Classic Pocket Edition.
- Licenciado bajo la **GNU General Public License v3.0 (GPLv3)**.
