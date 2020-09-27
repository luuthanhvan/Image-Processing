# Image Processing in Qt Creator

# Installation Qt 5.1.1 for Windows 10: https://download.qt.io/archive/qt/5.1/5.1.1/qt-windows-opensource-5.1.1-mingw48_opengl-x86-offline.exe.mirrorlist

# Installation Qt 5.1.1 for Ubuntu 20.04
- Download here: https://download.qt.io/archive/qt/5.1/5.1.1/qt-linux-opensource-5.1.1-x86_64-offline.run.mirrorlist
- Install Qt: Open Terminal and type the following commands
		
		$ cd Download # the directory store the current download file
		$ ls
		$ chmod +x qt-linux-opensource-5.1.1-x86_64-offline.run
		$ ls
		$ ./qt-linux-opensource-5.1.1-x86_64-offline.run
		
- After install, launch Qt and create a new project and run it. If there are some errors occur like "Usr/bin/ld cannot find -lGL". Fix this error by installing two packages:
		
		$ sudo apt-get install mesa-common-dev
		$ sudo apt-get install libglu1-mesa-dev
		
- If there is an error like 'Gtk-Message: Failed to load module "canberra-gtk-module"'. Fix:
		
		$ sudo apt install libcanberra-gtk-module libcanberra-gtk3-module
