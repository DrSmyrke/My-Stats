#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QDir>

struct Disk{
	QString name;
	QString mount;
	QString fstype;
#if __WORDSIZE == 64
	uint64_t size = -1;
	uint64_t used = 0;
	uint64_t avail = 0;
#else
	uint32_t size = -1;
	uint32_t used = 0;
	uint32_t avail = 0;
#endif
	float usedPrz = 0;
};

struct SendData{
#if __WORDSIZE == 64
	uint64_t memTotal		= 0;
	uint64_t memFree		= 0;
	uint64_t swapTotal		= 0;
	uint64_t swapFree		= 0;
#else
	uint32_t memTotal		= 0;
	uint32_t memFree		= 0;
	uint32_t swapTotal		= 0;
	uint32_t swapFree		= 0;
#endif
	uint8_t mem				= 0;
	uint8_t swap			= 0;
	uint8_t cpu				= 0;
	std::vector<Disk> disks;
	QString uptime;
};

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
	QString apiKey;
	uint16_t sendInterval	= 0;
	QString targetUrl;
};

namespace app {
	extern Config conf;

	void setLog(const uint8_t logLevel, const QString &mess);
	bool parsArgs(int argc, char *argv[]);
	void loadSettings();
	void saveSettings();
}

#endif // GLOBAL_H
