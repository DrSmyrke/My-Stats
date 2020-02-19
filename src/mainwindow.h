#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QTimer>
#include "hwmonitorwidget.h"
#include "global.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void slot_update();
private:
	HWMonitorWidget* m_pHWMonitorWidget;
	QTimer *m_pTimer;
	QNetworkAccessManager* m_pManager;

	bool m_mouseClick = false;
	bool m_mouseMove = false;
	qreal m_xOffset = 0;
	qreal m_yOffset = 0;
	int m_prewWidth;
	uint16_t m_tickCounter;
	uint16_t m_secCounter;
	uint16_t m_sendSecCounter;

	void setWindowAction();
	void sendStats();
protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // MAINWINDOW_H
