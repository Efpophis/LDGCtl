#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <math.h>
#include <arpa/inet.h> // for ntohs
#include <unistd.h>
#include "serial.h"

using namespace std;

void tuneResponse( char r )
{
	switch (r)
	{
		case 'T':
			cout << "Good tune!" << endl;
		break;
		
		case 'M':
			cout << "OK tune.." << endl;
		break;
		
		case 'F':
			cout << "BAD tune :(" << endl;
		break;
	}
}

int main( int argc, char* argv[] )
{
	const char toggleAntCmd = 'A';
	const char memTuneCmd   = 'M';
	const char fullTuneCmd  = 'F';
	const char bypassCmd    = 'P';
	const char autoCmd      = 'C';
	const char manualCmd    = 'M';
	const char syncCmd      = 'Z';
	const char meterCmd     = 'S';
	const char ctlCmd       = 'X';
	const char wakeCmd      = ' ';
	
	if ( argc == 2 )
	{
		SerialPort port;
		
		if ( port.init( "/dev/ttyUSB1", B38400) != -1 )
		{
			string cmd = argv[1];
			const string syncResp = "000000000000000AzAz";
			char resp[20];
			
			memset( resp, 0x00, sizeof(resp) );
			
			if ( cmd == "toggle" )
			{
				port.write( &wakeCmd, sizeof(wakeCmd) );
				usleep( 1000 );
				port.write( &toggleAntCmd, sizeof(toggleAntCmd) );
				port.readFully( resp, 1 );
				cout << "Antenna " << resp << " selected." << endl;
			}
			else if ( cmd == "mem" )
			{
				port.write( &wakeCmd, sizeof(wakeCmd) );
				usleep( 1000 );
				port.write( &memTuneCmd, sizeof(memTuneCmd));
				port.readFully( resp, 1 );
				tuneResponse( resp[0] );
			}
			else if ( cmd == "full" )
			{
				port.write( &wakeCmd, sizeof(wakeCmd) );
				usleep( 1000 );
				port.write( &fullTuneCmd, sizeof(fullTuneCmd) );
				port.readFully( resp, 1 );
				tuneResponse( resp[0] );
			}
			else if ( cmd == "bypass" )
			{
				port.write( &wakeCmd, sizeof(wakeCmd) );
				usleep( 1000 );
				port.write( &bypassCmd, sizeof(bypassCmd) );
				port.readFully( resp, 1 );
				cout << "bypass response: " << resp[0] << endl;
			}
			else if ( cmd == "auto" )
			{
				port.write( &wakeCmd, sizeof(wakeCmd) );
				usleep( 1000 );
				port.write( &autoCmd, sizeof(autoCmd) );
				port.readFully( resp, 1 );
				cout << "auto response: " << resp[0] << endl;
			}
			else if ( cmd == "manual" )
			{
				port.write( &wakeCmd, sizeof(wakeCmd) );
				usleep( 1000 );
				port.write( &manualCmd, sizeof(manualCmd) );
				port.readFully( resp, 1 );
				cout << "Manual response: " << resp[0] << endl;
			}
			else if ( cmd == "sync" )
			{
				port.write( &wakeCmd, sizeof(wakeCmd) );
				usleep( 1000 );
				port.write( &syncCmd, sizeof(syncCmd) );
				port.readFully( resp, 19 );
				cout << "Sync Response: " << resp << endl;
			}
			else if ( cmd == "meter" )
			{
				port.write( &wakeCmd, sizeof(wakeCmd) );
				usleep( 1000 );
				port.write( &meterCmd, sizeof(meterCmd) );
			}
			else if ( cmd == "ctl" )
			{
				port.write( &wakeCmd, sizeof(wakeCmd) );
				usleep( 1000 );
				port.write( &ctlCmd, sizeof(ctlCmd));
			}
			else 
			{
				cout << "invalid command: " << cmd << endl;
			}
		}
		else
		{
			cout << "problem opening port" << endl;
		}
	
	}
	else
	{
		cout << "incorrect number of args" << endl;
	}
	exit(0);
}
