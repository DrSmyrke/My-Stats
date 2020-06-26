#include "core.h"

Core::Core(QObject *parent) : QObject(parent)
{
	m_tickCounter		= 0;
	m_secCounter		= 0;
	m_sendSecCounter	= 0;

	m_pHWMonitor = new HWMonitor( this );
	m_pTimer = new QTimer( this );
		m_pTimer->setInterval(250);

		connect( m_pTimer, &QTimer::timeout, this, &Core::slot_update );
}

Core::~Core()
{
	app::setLog( 1, "Core::deinititialize" );
	if( m_pTimer->isActive() ){
		m_pTimer->stop();
	}
}

void Core::init()
{
	app::setLog( 1, "Core::inititialize" );
	m_pTimer->start();
}

void Core::slot_update()
{
	m_pHWMonitor->slot_timer();

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

void Core::sendStats()
{
	if( app::conf.apiUrl.isEmpty() ){
		app::setLog( 2, "MainWindow::sendStats	conf.apiUrl.isEmpty" );
		return;
	}

	SendData data = m_pHWMonitor->getSendData();

	app::sendData( data );
}
