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
qmake version_desktop.pro || exit 1
make || exit 1
make clean || exit 1
qmake version_server.pro || exit 1
make || exit 1
cd ..
