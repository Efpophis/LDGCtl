#include <iostream>
#include <string>
#include <unistd.h>
#include "serial.h"


using namespace std;

int main( int argc, char* argv[] )
{
	const unsigned char off_cmd[] = { 0xff, 0x01, 0x00 };
	const unsigned char on_cmd[]  = { 0xff, 0x01, 0x01 };
	
	SerialPort port;
	
	if ( port.init( "/dev/ttyUSB2" ) != -1 )
	{
		if ( argc == 2 )
		{
			string status = argv[1];
			
			if ( status == "on" )
			{
				port.write( on_cmd, sizeof( on_cmd ) );
			}
			else if ( status == "reset" )
			{
				port.write( off_cmd, sizeof(off_cmd) );
				sleep(1);
				port.write( on_cmd, sizeof( on_cmd ) );
			}
			else if ( status == "off" )
			{
				port.write( off_cmd, sizeof( off_cmd ) );
			}
			else
			{
				cout << "invalid command: " << status << endl;
			}
			
		}
		else
		{
			cout << "invalid argument" << endl;
		}
	}
	else
	{
		cout << "Error initializing port" << endl;
	}
	return 0;
}
