# Image Processing in Qt

# Installation Qt 5.1.1 in Ubuntu 20.04
- Download here: https://download.qt.io/archive/qt/5.1/5.1.1/qt-linux-opensource-5.1.1-x86_64-offline.run.mirrorlist
- Install Qt: open Terminal and type the following commands
		$ cd Download (the directory store the current download file)
		$ ls
		$ chmod +x qt-linux-opensource-5.1.1-x86_64-offline.run
		$ ls
		$ ./qt-linux-opensource-5.1.1-x86_64-offline.run
- After install, launch Qt and create a new project and run it. If there are some error occur like "Usr/bin/ld cannot find -lGL". Fix this error by installing two packages:
		$ sudo apt-get install mesa-common-dev
		$ sudo apt-get install libglu1-mesa-dev
- If there is an error like 'Gtk-Message: Failed to load module "canberra-gtk-module"'. Fix:
		$ $ sudo apt install libcanberra-gtk-module libcanberra-gtk3-module
