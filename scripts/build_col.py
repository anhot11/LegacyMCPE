#!/usr/bin/env python3
'''
Copyright (c) 2026 Pyogenics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
'''

'''
Colour table builder
--------------------------------------------------
Usage: build_col.py path/to/HTMLColours.xml path/to/output.col

For format see Common/Colours/ColourTable.cpp
'''

from sys import argv
import xml.etree.ElementTree as ET
from struct import pack


def pack_stream(format, stream, *data):
    packedData = pack(format, *data)
    stream.write(packedData)


def write_string(string, stream):
    string = string.encode("utf-8")
    pack_stream(">H", stream, len(string))
    stream.write(string)


target_file_path = argv[1]
output_file_path = argv[2]

colours_file_tree = ET.parse(target_file_path)
colours_root = colours_file_tree.getroot()

# Read colour data
colours = {}
for colour_data in colours_root:
    name = colour_data.attrib["name"]
    value = int(colour_data.attrib["value"], 16)

    colours[name] = value
print(f"Processed {len(colours)} from {target_file_path}")

# Write col file
TARGET_COL_VERSION = 0
with open(output_file_path, "wb") as col_file:
    # Write header
    pack_stream(">2I", col_file,
        TARGET_COL_VERSION,
        len(colours)
    )

    # Write colours
    for name, value in colours.items():
        write_string(name, col_file)
        pack_stream(">I", col_file, value)

    print(f"Wrote {col_file.tell()} bytes to {output_file_path}")
