#include "global.h"
#include "api.h"

#include <QIODevice>
#include <QDateTime>
#include <QSettings>
#include <QHostInfo>

namespace app {
	Config conf;

	void setLog(const uint8_t logLevel, const QString &mess)
	{
		if(app::conf.logLevel < logLevel or app::conf.logLevel == 0) return;

		QDateTime dt = QDateTime::currentDateTime();
		QString str = dt.toString("yyyy.MM.dd [hh:mm:ss] ") + mess + "\n";

		if( app::conf.verbose ){
			printf( "%s", str.toUtf8().data() );
			fflush( stdout );
		}

		if( app::conf.logFile.isEmpty() ) return;
		QFile f;
		f.setFileName( app::conf.logFile );
		if( f.open( QIODevice::Append ) ){
			f.write( str.toUtf8() );
			f.close();
		}
	}

	bool parsArgs(int argc, char *argv[])
	{
		bool ret = true;
		for(int i=0;i<argc;i++){
			if(QString(argv[i]).indexOf("-")==0){
				if(QString(argv[i]) == "--help" or QString(argv[1]) == "-h"){
					printf("Usage: %s [OPTIONS]\n"
							"  -l <FILE>    log file\n"
							"  -v		Verbose output\n"
							"  -d. --daemon		Daemon mode\n"
							"  --stop    			Stop daemon\n"
							"  --version	print current version\n"
							"\n", argv[0]);
					ret = false;
				}
				if(QString(argv[i]) == "-l") app::conf.logFile = QString(argv[++i]);
				if(QString(argv[i]) == "-v") app::conf.verbose = true;
				if(QString(argv[i]) == "--daemon" || QString(argv[i]) == "-d") app::conf.daemonMode = true;
				if(QString(argv[i]) == "--stop") app::conf.stopMode = true;
				if(QString(argv[i]) == "--version"){
					printf( "%s\n", app::conf.version.toUtf8().data() );
					ret = false;
				}
			}
		}
		return ret;
	}

	void loadSettings()
	{
#ifdef SERVER_VERSION
		QSettings settings( "/etc/DrSmyrke/MyStats.ini", QSettings::IniFormat );
#else
		QSettings settings( "MySoft","MyStats" );
#endif
		app::conf.logFile = settings.value("MAIN/logFile",app::conf.logFile).toString();
		app::conf.apiKey = settings.value("MAIN/apiKey",app::conf.apiKey).toString();
		app::conf.apiUrl = settings.value("MAIN/apiUrl",app::conf.apiUrl).toString();
		app::conf.logLevel = settings.value("MAIN/logLevel",app::conf.logLevel).toUInt();
		app::conf.sendInterval = settings.value("MAIN/sendInterval",app::conf.sendInterval).toInt();

		if( settings.allKeys().size() == 0 ) app::saveSettings();
	}

	void saveSettings()
	{
#ifdef SERVER_VERSION
		QSettings settings( "/etc/DrSmyrke/MyStats.ini", QSettings::IniFormat );
#else
		QSettings settings( "MySoft","MyStats" );
#endif
		settings.clear();
		settings.setValue("MAIN/logFile",app::conf.logFile);
		settings.setValue("MAIN/apiKey",app::conf.apiKey);
		settings.setValue("MAIN/apiUrl",app::conf.apiUrl);
		settings.setValue("MAIN/logLevel",app::conf.logLevel);
		settings.setValue("MAIN/sendInterval",app::conf.sendInterval);
	}

	void sendData(const SendData &data)
	{
		API::Request request;
		
		request.url = app::conf.apiUrl;
		request.method = API::Method::POST;
		request.dataType = API::DataType::JSON;
		request.dataList[ "key" ]			= app::conf.apiKey;
		request.dataList[ "name" ]			= QHostInfo::localHostName();
		request.dataList[ "uptime" ]		= data.uptime;
		request.dataList[ "cpu" ]			= data.cpu;
		request.dataList[ "ram_total" ]		= QString::number( data.memTotal );
		request.dataList[ "ram_used" ]		= QString::number( data.mem );
		request.dataList[ "swap_total" ]	= QString::number( data.swapTotal );
		request.dataList[ "swap_used" ]		= QString::number( data.swap );
		request.dataList[ "process" ]		= data.process;
		
		//TODO: DISKS & ifaces
		//request.dataList[ "disks" ] = "disks JSON data";
		
		API::sendRequest( request );
	}

}
