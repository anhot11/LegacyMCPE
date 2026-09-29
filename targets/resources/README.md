# Minecraft.Client Asset Structure
This is the minimum asset structure needed for the game to function.
```
AssetStructure
├── Common
│   ├── Media
│   │   ├── MediaWindows64.arc
│   │   └── font
│   │       └── *
│   ├── Trial
│   │   └── TrialLevel.mcs
│   └── res
│       └── *
├── Durango
│   └── Sound
│       └── Minecraft.msscmp
├── Windows64Media
│   └── DLC
│       └── *
├── Windows64
│   └── GameHDD
└── music
    ├── cds
    │   └── *.binka
    └── music
        └── *.binka
```
## Windows64Media.arc contents
The contents of the the arc file can be seen in the `.txt` files in this folder (`movies.txt`, `media.txt`, etc...).
- `languages.loc` is built from `Platform/Windows64Media/loc`
- `HTMLColours.col` is built from `HTMLColours.xml`
```
MediaWindows64.arc
├── Graphics
│   ├── MinecraftIcon.png
│   ├── SaveChest.png
│   └── TexturePackIcon.png
├── *.swf
├── HTMLColours.col
├── Tutorial.pck
├── languages.loc
└── splashes.txt

```
## Asset locations
### Generic assets should be contained inside this folder:
- `movies/` - Flash SWF files for Iggy
- `graphics/` - Contains images used by the client (only `SaveChest.png` `MinecraftIcon.png`, `TexturePackIcon.png` are packed into the arc)
- `font/` - Fonts, duh
- `res/` - Textures and other miscellaneous data
- `music/` - Contains background music as well as music discs
- `levels/` - Contains some premade worlds (`Tutorial` is unused as we pull it from somewhere else, atleast on Windows64 asset base)
### Platform assets are contained in Minecraft.Client/Platform/
- `Windows64Media/loc/` - Localisation/language data that we use as a source to build the languages.loc (this file gets included in arc)
- `Windows64Media/Media/` - Contains a bunch of Windows64 customised swfs and also the tutorial level (`Tutorial.pck`), these should be included in arc
- `Windows64Media/DLC/` - Windows64 DLC (only has skin packs)
- `DurangoMedia/DLC/` - The actual DLC folder you want to use
