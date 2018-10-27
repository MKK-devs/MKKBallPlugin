# MKKBallPlugin
VideoFx plugin a virtual DJ 8-hoz. MKK-s bálok kivetítésére.

# Hogyan működik?
Ez a plugin a forráskódok alapján egy .dll fájlt generál. Ehhez használható a felépített Visual Studio project.

# Visual Studio
A Visual Studio projekt fájl amennyiben nem képes lefordulni, az alábbi projektbeállításokat kell megfontolni: (Properties)
Általánosságban
- General/Windows SDK Platform verziója up-to-date

LCPSTR
- General/CharacterSet = Use Multi-Byte Character Set

"Cannot open source file..."
- VC++ Directories/Include Directories tartalmazza: $(IncludePath);$(DXSDK_DIR)Include
- VC++ Directories/Library Directories tartalmazza: $(LibraryPath);$(DXSDK_DIR)Lib\x64 (x86 a 32 Bites verzión)

"Unexpected end of file, while looking for precompiled header"
- C/C++/Precompiled Headers/Precompiled Headers = Not Using Precompiled headers

# Telepítés
A kész /Debug/MKKBallPlugin.dll fájl kell bemásolni a VirtualDJ megfelelő mappájába:
/Documents/VirtualDJ/Plugins/VideoEffect (Ez az alapméretezett elérés Windowson)

Ezután nincs más munka, csak meg kell nyitni a VirtualDJ-t, és a Video panelen, a VideoEffects-ek közül be kell kapcsolni az MKKBallPlugint. Jobb gombbal kattintva lehet előhívni a beállításokat. Ennek a kezelőfelülete a VirtualDJ sajátja.
