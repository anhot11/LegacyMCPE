# Portable LCE Android Launcher
You might not like that I'm using WAF to build the APK, but I find it very convenient.    
To build the launcher:  
1. Compile Portable LCE 
2. Place libMinecraftClient.so in the ```android/lib/arm64-v8a folder```  
3. Run the build with the command ```python3 waf configure -T debug && python3 waf build```
  
Compiled APK will be located at ```build/android/mod-signed.apk```
Launcher is incredibly simple, so don’t expect anything more.
