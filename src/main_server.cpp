#include <QCoreApplication>
#include <QDir>
#include <QTranslator>
#include <QLocale>
#include "global.h"
#include "core.h"
//#include "../../../HomeNET/client.h"
//#include "../../../HomeNET/searcher.h"
//#include "../../../HomeNET/myproto.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	app::conf.version = QString("%1.%2").arg(APP_VER_FIRST).arg(APP_VER_SECOND);

	auto localeName = QLocale::system().name();
	QTranslator translator(&a);
	if(translator.load(localeName,"://lang/")) a.installTranslator(&translator);

	app::loadSettings();
	if( !app::parsArgs(argc, argv) ) return 0;
	if( !app::conf.logFile.isEmpty() ) QFile( app::conf.logFile ).remove();

	//myproto::appData.type = app_type_mystats;
	//Searcher searcher(&a);
	//Client client( socket_type_client, &a );
	//client.open( QHostAddress::LocalHost, myproto::conf.port );

	app::conf.showData = true;

	Core core;
	core.init();

	//searcher.start();

	return a.exec();
}
