#ifndef HWMONITOR_H
#define HWMONITOR_H

#include <QObject>
#include "global.h"
#include "myfunctions.h"

struct UiData{
	QString time = "00:00:00";
	QString uptime = "00:00:00";
	QString date = "-- 00.00.0000";
	QString procCount = "0";
	float cpu = 0;
	float mem = 0;
	float swap = 0;
	QString memUsed = "0 b";
	QString memTotal = "0 b";
	QString swapUsed = "0 b";
	QString swapTotal = "0 b";
	std::vector<QString> swaps;
	std::vector<Iface> ifaces;
	std::vector<Disk> disks;
};

class HWMonitor : public QObject
{
	Q_OBJECT
public:
	explicit HWMonitor(QObject *parent = 0);
	UiData getData() { return m_data; }
	SendData getSendData() { return m_sendData; }
public slots:
	void slot_timer();
private:
	UiData m_data;
	SendData m_sendData;

	void getUptime();
	void getCpu();
	void getMem();
	void getProcess();
	void getIfaces();
	void getDevs();
};

#endif // HWMONITOR_H
