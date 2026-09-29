#!/usr/bin/env python3
import glob
import os

old_block = """if platform_is_winrt or platform_is_haiku
  add_languages(
    'cpp',
    native: false,
  )
  all_sources += [cxx_sources]
endif"""

android_block = """if platform_is_android
  add_languages(
    'cpp',
    native: false,
  )
  all_sources += [files('src/hidapi/android/hid.cpp')]
endif"""

for p in glob.glob("subprojects/SDL2-*/meson.build"):
    with open(p, "r") as f:
        c = f.read()
    if android_block not in c and old_block in c:
        c = c.replace(old_block, old_block + "\n\n" + android_block)
        with open(p, "w") as f:
            f.write(c)
        print(f"Successfully patched {p}")
    else:
        print(f"Skipping {p}, already patched or block not found")
