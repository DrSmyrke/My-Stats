#include "version.h"
#include "global.h"

#ifdef SERVER_VERSION
	#include <QCoreApplication>
	#include <unistd.h>   //fork(3), chdir(3), sysconf(3)
	#include <signal.h>   //signal(3)
	#include "core.h"
	#include "myfunctions.h"
#else
	#include <QApplication>
	#include "mainwindow.h"
#endif

#include <QFile>
#include <QTranslator>
#include <QLocale>
//#include "../../../HomeNET/client.h"
//#include "../../../HomeNET/searcher.h"
//#include "../../../HomeNET/myproto.h"

#ifdef SERVER_VERSION
static void signalExitHandler(int sig)
{
	Q_UNUSED( sig )
	qApp->exit();
}
#endif

int main(int argc, char *argv[])
{
	app::conf.version = QString("%1.%2").arg(APP_VER_FIRST).arg(APP_VER_SECOND);
	app::loadSettings();
	if( !app::parsArgs(argc, argv) ) return 0;
	
#ifdef SERVER_VERSION
	int pid = -1;

	if( app::conf.stopMode ){
		if( !QDir( "/proc" ).exists() ){
			printf( "/proc path not found!!!\n" );
			return EXIT_FAILURE;
		}

		pid = mf::pidOfProc( APP_NAME );
		if( pid != -1 ){
			printf( "Stopping %s PID: [%d] ...\n", APP_NAME, pid );
			kill( pid, SIGTERM );
		}

		return EXIT_SUCCESS;
	}
	QCoreApplication a(argc, argv);
#else
	QApplication a(argc, argv);
#endif

	auto localeName = QLocale::system().name();
	QTranslator translator(&a);
	if(translator.load(localeName,"://lang/")) a.installTranslator(&translator);

	if( !app::conf.logFile.isEmpty() ) QFile( app::conf.logFile ).remove();

	//myproto::appData.type = app_type_mystats;
	//Searcher searcher(&a);
	//Client client( socket_type_client, &a );
	//client.open( QHostAddress::LocalHost, myproto::conf.port );

	app::conf.showData = true;

#ifdef SERVER_VERSION
	signal(SIGINT, signalExitHandler);

	if( app::conf.daemonMode ){

		pid = mf::pidOfProc( APP_NAME );
		if( pid != -1 ){
			printf( "Daemon is running %s PID: [%d] ...\n", APP_NAME, pid );
			return EXIT_FAILURE;
		}

		pid = fork();
		if (pid < 0) exit(EXIT_FAILURE);
		if (pid > 0) exit(EXIT_SUCCESS);
		signal( SIGCHLD, SIG_IGN );
		signal( SIGHUP, SIG_IGN );
		signal( SIGTERM, signalExitHandler );
		signal( SIGKILL, signalExitHandler );
		signal( SIGSTOP, signalExitHandler );
		chdir( "/" );
		for( int x = sysconf(_SC_OPEN_MAX); x >= 0; x--){
			close (x);
		}
		//reopen stdin, stdout, stderr
		stdin = fopen( "/dev/null", "r" );		//fd=0
		stdout = fopen( "/dev/null", "w+" );	//fd=1
		stderr = fopen( "/dev/null", "w+" );	//fd=2
	}

	Core core;
	core.init();
#else
	MainWindow w;
	w.show();
#endif

	//searcher.start();

	return a.exec();
}
