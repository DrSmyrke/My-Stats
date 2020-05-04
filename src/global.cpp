#include "global.h"
#include <QIODevice>
#include <QDateTime>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrlQuery>

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
		app::conf.targetUrl = settings.value("MAIN/targetUrl",app::conf.targetUrl).toString();
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
		settings.setValue("MAIN/targetUrl",app::conf.targetUrl);
		settings.setValue("MAIN/logLevel",app::conf.logLevel);
		settings.setValue("MAIN/sendInterval",app::conf.sendInterval);
	}

	void sendData(const SendData &data)
	{
		QNetworkAccessManager manager;
		QUrl url( app::conf.targetUrl );
		QNetworkRequest request( url );
		QString auth = QString( "%1:%2" ).arg( "api" ).arg( app::conf.apiKey );
		request.setRawHeader( "Authorization", "Basic " + auth.toUtf8().toBase64() );
		request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );

		QUrlQuery params;
		params.addQueryItem( "cpu", QString::number( data.cpu ) );
		params.addQueryItem( "mem", QString::number( data.mem ) );
		params.addQueryItem( "memTotal", QString::number( data.memTotal ) );
		params.addQueryItem( "memFree", QString::number( data.memFree ) );
		params.addQueryItem( "swap", QString::number( data.swap ) );
		params.addQueryItem( "swapTotal", QString::number( data.swapTotal ) );
		params.addQueryItem( "swapFree", QString::number( data.swapFree ) );
		params.addQueryItem( "uptime", data.uptime );
		//TODO: DISKS

		manager.post( request, params.query().toUtf8() );
		
		//TODO: post JsonData
	}

}
