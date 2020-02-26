cd src
qmake version_desktop.pro
make
make clean
qmake version_server.pro
make
cd ..
