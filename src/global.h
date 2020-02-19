#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QDir>

struct Config{
	bool showData			= true;
	bool verbose			= true;
	uint8_t logLevel		= 3;
#ifdef __linux__
	QString logFile			= "/tmp/MyStats.log";
#elif _WIN32
	QString logFile			= QDir::homePath() + "/MyStats.log";
#endif
	QString version;
};

namespace app {
	extern Config conf;

	void setLog(const uint8_t logLevel, const QString &mess);
	bool parsArgs(int argc, char *argv[]);
	void loadSettings();
	void saveSettings();
}

#endif // GLOBAL_H
