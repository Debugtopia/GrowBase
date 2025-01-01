![test](https://media.discordapp.net/attachments/1263187470737080322/1320509379187048458/Untitled525_20241214210738.png?ex=67751071&is=6773bef1&hm=edb7a01e04fa5b874debda17e73a034d2cf6df76cd5a0fa45120cde0e6621bad&=&format=webp&quality=lossless&width=550&height=275)

Open sourced barebones Growtopia Server written in C++ by @xhexago on discord & telegram

Windows setup:
- download cmake from https://cmake.org/download/ - this option is only if you wish to use CMake over Microsoft Visual Studio
- if you use visual studio, download visual studio 2022 from https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2030&passive=false

- if you use visual studio, open the solution file(GrowBase.sln) from /src/ directory
- if you use CMake, run build_release.bat or build_debug, depending on the build conf you prefer
? use debug only when you're trying to debug for crashs.
? use release when you host the server to public

Config setup:
- edit the address, basePort from /bin/config.txt to start your server
? if the ip is 0.0.0.0 you don't need to change it


Linux setup:
- installing necessary packages
```bash
sudo apt-get install build-essential cmake
```
- run build_release.sh or build_debug.sh, depending on the build conf you prefer
- move the binary to /bin/ directory
```bash
# current path: GrowBase/build/
mv src/GrowBase ..bin/
```
- edit the address, basePort from /bin/config.txt to start your server

Please don't take our credits out of BaseApp.h file!
Credit us for our work

Community Server: https://discord.gg/2ynZ4cT6G6

If you got any questions on how to set it up or in general, please visit the community server above :)
