# GEGL Extrusion 2 Gimp Plugin. (Fork of GEGL Long Shadow) 

GEGL does a Long Shadow but with Pixel Data. As opposed to a Color fill. 

![image](https://github.com/LinuxBeaver/GEGL-Extrusion-2----Fork-of-GEGL-Long-Shadow/assets/78667207/96e58c1d-2d07-4c58-b5cd-c57ee986ce49)

![image preview](preview1.png )

![image preview](preview2.png )

## Instructions 
If you do not use the optional binarys then you must manually compile and put the .so/dll files in gegl-0.4/plug-ins. Then restart Gimp and go to Gimp's GEGL operations.

## OS specific location to put GEGL Filter binaries 

Windows
C:\Users\USERNAME\AppData\Local\gegl-0.4\plug-ins
 
 Linux 
 /home/(USERNAME)/.local/share/gegl-0.4/plug-ins
 
 Linux (Flatpak)
 /home/(USERNAME)/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins

 ![image](https://github.com/LinuxBeaver/GEGL-Extrusion-2----Fork-of-GEGL-Long-Shadow/assets/78667207/dc6d4b6b-d1e8-4f3d-a4bf-a13036dc466a)



## Compiling and Installing

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```

If you have an older version of gegl you may need to copy to `~/.local/share/gegl-0.3/plug-ins`
instead (on Ubuntu 18.04 for example).



### Windows

The easiest way to compile this project on Windows is by using msys2.  Download
and install it from here: https://www.msys2.org/

Open a msys2 terminal with `C:\msys64\mingw64.exe`.  Run the following to
install required build dependencies:

```bash
pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl
```

Then build the same way you would on Linux:

```bash
meson setup --buildtype=release build
ninja -C build
```
