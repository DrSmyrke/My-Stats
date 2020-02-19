#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QTimer>
#include "hwmonitor.h"
#include "global.h"

class Core : public QObject
{
	Q_OBJECT
public:
	explicit Core(QObject *parent = nullptr);
	void init();
private slots:
	void slot_update();
private:
	QTimer *m_pTimer;
	HWMonitor* m_pHWMonitor;

	uint16_t m_tickCounter;
	uint16_t m_secCounter;
	uint16_t m_sendSecCounter;

	void sendStats();
protected:
};

#endif // CORE_H
