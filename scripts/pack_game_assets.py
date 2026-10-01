#!/usr/bin/env python3
import os
import sys
import zipfile

def pack_assets():
    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    if len(sys.argv) > 1:
        target_zip = os.path.abspath(sys.argv[1])
    else:
        target_zip = os.path.join(repo_root, "MCPL-Data.zip")
    
    # 1. MediaWindows64.arc
    arc_path = os.path.join(repo_root, "targets", "resources", "Common", "Media", "MediaWindows64.arc")
    if not os.path.exists(arc_path):
        # Check build directory
        alt_arc = os.path.join(repo_root, "build_android", "targets", "resources", "MediaWindows64.arc")
        if os.path.exists(alt_arc):
            arc_path = alt_arc
        else:
            print(f"ERROR: MediaWindows64.arc not found at {arc_path} or {alt_arc}", file=sys.stderr)
            sys.exit(1)

    # 2. Common/res directory
    res_dir = os.path.join(repo_root, "targets", "resources", "Common", "res")
    if not os.path.exists(res_dir):
        print(f"ERROR: Common/res not found at {res_dir}", file=sys.stderr)
        sys.exit(1)

    # 3. gamecontrollerdb.txt
    controller_db = os.path.join(repo_root, "android-launcher", "android", "assets", "gamecontrollerdb.txt")
    if not os.path.exists(controller_db):
        print(f"ERROR: gamecontrollerdb.txt not found at {controller_db}", file=sys.stderr)
        sys.exit(1)

    print(f"Creating game asset bundle: {target_zip}")
    os.makedirs(os.path.dirname(target_zip), exist_ok=True)
    
    file_count = 0
    total_bytes = 0
    with zipfile.ZipFile(target_zip, 'w', compression=zipfile.ZIP_DEFLATED, compresslevel=6) as zf:
        # Add MediaWindows64.arc
        arc_arcname = "Common/Media/MediaWindows64.arc"
        print(f"  Adding {arc_arcname} ({os.path.getsize(arc_path):,} bytes)...")
        zf.write(arc_path, arcname=arc_arcname)
        file_count += 1
        total_bytes += os.path.getsize(arc_path)

        # Add gamecontrollerdb.txt
        print(f"  Adding gamecontrollerdb.txt ({os.path.getsize(controller_db):,} bytes)...")
        zf.write(controller_db, arcname="gamecontrollerdb.txt")
        file_count += 1
        total_bytes += os.path.getsize(controller_db)

        # Add all files in Common/res
        for root, dirs, files in os.walk(res_dir):
            for file in files:
                full_path = os.path.join(root, file)
                rel_path = os.path.relpath(full_path, os.path.join(repo_root, "targets", "resources"))
                zf.write(full_path, arcname=rel_path)
                file_count += 1
                total_bytes += os.path.getsize(full_path)

    zip_size = os.path.getsize(target_zip)
    print(f"Successfully packed {file_count} files ({total_bytes:,} uncompressed bytes) into {target_zip}")
    print(f"Compressed bundle size: {zip_size / (1024*1024):.2f} MB")

if __name__ == "__main__":
    pack_assets()
