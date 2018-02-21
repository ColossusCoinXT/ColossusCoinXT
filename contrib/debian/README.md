
Debian
====================
This directory contains files used to package znoded/znode-qt
for Debian-based Linux systems. If you compile znoded/znode-qt yourself, there are some useful files here.

## znode: URI support ##


znode-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install znode-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your znodeqt binary to `/usr/bin`
and the `../../share/pixmaps/znode128.png` to `/usr/share/pixmaps`

znode-qt.protocol (KDE)

