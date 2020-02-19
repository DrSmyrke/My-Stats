#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	m_tickCounter		= 0;
	m_secCounter		= 0;
	m_sendSecCounter	= 0;
	m_prewWidth			= -1;

	m_pTimer = new QTimer(this);
		m_pTimer->setInterval(250);
	m_pHWMonitorWidget = new HWMonitorWidget(this);
		m_pHWMonitorWidget->setMouseTracking(true);

	setCentralWidget(m_pHWMonitorWidget);
	//setWindowFlags(Qt::WindowStaysOnBottomHint | Qt::CustomizeWindowHint | Qt::Tool);
	setWindowFlags(Qt::Popup | Qt::ToolTip);
	setBaseSize( 270, 240 );
	setFixedWidth( 270 );
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowIcon( QIcon("://img/monitor.png") );

	connect(m_pHWMonitorWidget,&HWMonitorWidget::signal_heightChangeRequest,this,[this](const uint16_t newValue){
		this->setFixedHeight( newValue );
		if( !this->isHidden() ){
			if( m_prewWidth < 0 ) m_prewWidth = this->size().width();
			setWindowAction();
		}
	});
	connect( m_pTimer, &QTimer::timeout, this, &MainWindow::slot_update );
	connect(m_pHWMonitorWidget,&HWMonitorWidget::signal_diskClicked,this,[this]( const QString &path, const QString &name ){
		QMessageBox msgBox(this);
		msgBox.setText("You are selected device [" + name + "]");
		msgBox.setInformativeText(tr("Device mount to: ") + path);
		msgBox.setStandardButtons(QMessageBox::Open | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		msgBox.setButtonText(QMessageBox::Discard,tr("Eject"));
		msgBox.setWindowFlags( Qt::ToolTip );
		int ret = msgBox.exec();
		QString dev;
		QProcess proc;
		switch (ret) {
			case QMessageBox::Open: mf::startDetached("xdg-open", QStringList()<<path); break;
			case QMessageBox::Discard:
				dev = name.left( name.length() - 1 );
				proc.start( "sync" );
				proc.waitForFinished();
				proc.start( "udisks", QStringList()<<"--umount"<<name );
				proc.waitForFinished();
				proc.start( "udisksctl", QStringList()<<"unmount"<<"-b"<<name );
				proc.waitForFinished();
				proc.start( "udisks", QStringList()<<"--detach"<<dev );
				proc.waitForFinished();
				proc.start( "udisksctl", QStringList()<<"power-off"<<"-b"<<dev );
				proc.waitForFinished();
			break;
			default: break;
		}
	});
	m_pTimer->start();

	setWindowAction();
}

MainWindow::~MainWindow()
{

}

void MainWindow::slot_update()
{
	m_pHWMonitorWidget->slot_update();

	if( m_tickCounter++ >= ( 1000 / m_pTimer->interval() ) ){
		m_tickCounter = 0;
		m_secCounter++;
		m_sendSecCounter++;
	}

	if( app::conf.sendInterval == 0 ) return;
	if( m_sendSecCounter == app::conf.sendInterval ){
		sendStats();
		m_sendSecCounter = 0;
	}
}

void MainWindow::setWindowAction()
{
	if( m_prewWidth < 0 ) return;

	int primaryScreenNum = QApplication::desktop()->primaryScreen();
	auto primaryScreen = QApplication::desktop()->screenGeometry( primaryScreenNum );
	//m_screen = QApplication::desktop()->screenGeometry(this);

	//auto y = QCursor::pos().y();
	//qDebug()<<QApplication::desktop()->geometry()<<primaryScreen;

	if( app::conf.showData ){
		if( m_prewWidth != this->size().width() ){
			this->setFixedWidth( m_prewWidth );
		}
		this->move( primaryScreen.x() + primaryScreen.width() - this->width() - 5, primaryScreen.y() + primaryScreen.height() - this->height() - 33 );
	}else{
		m_prewWidth = this->size().width();
		this->setFixedWidth( 5 );
		this->move( primaryScreen.x() + primaryScreen.width() - 5, primaryScreen.y() + primaryScreen.height() - this->height() - 33 );
	}

	//this->move( primaryScreen.x() + primaryScreen.width() - this->width() - 5, primaryScreen.y() + primaryScreen.height() - this->height() - 33 );
}

void MainWindow::sendStats()
{
	if( app::conf.targetUrl.isEmpty() ){
		app::setLog( 2, "MainWindow::sendStats	conf.targetUrl.isEmpty" );
		return;
	}

	SendData data = m_pHWMonitorWidget->getSendData();

	app::sendData( data );
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
	m_mouseClick = true;
	m_xOffset = event->windowPos().x();
	m_yOffset = event->windowPos().y();

	//Q_UNUSED(event);
	QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
	if( !m_pHWMonitorWidget->mouseClickToObject() ){
		app::conf.showData = !app::conf.showData;
		setWindowAction();
	}

	QMainWindow::mouseReleaseEvent(event);
}
