if [ $1 ]; then
	if [ $1 = "--version" ]; then
		if [ -f "src/version.h" ]; then
			cat "src/version.h" | awk '{print $3}'
		else
			echo "N/A"
		fi
		exit 0
	fi
fi


cd src
qmake version_desktop.pro
make
make clean
qmake version_server.pro
make
cd ..
