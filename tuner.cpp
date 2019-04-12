#include <iostream>
#include <iomanip>
#include <string>
#include <math.h>
#include <arpa/inet.h> // for ntohs
#include "serial.h"

using namespace std;

float decodePwr( unsigned short raw )
{
	float result = -1.0;
	
	if ( raw < 256 )
	{
		// 0 .. 100 w
		result = (((float)(raw)) / 255.0) * 100.0;
	}
	else
	{
		// 101 .. 1000
		raw -= 256.0;
		result = 100.0 + (900.0 * (((float)raw)/768.0));
	}
	return result;
}

int main( int argc, char* argv[] )
{
    SerialPort port;
    int bytesRead = 0;
    unsigned char buf[8+1];
    
    if ( port.init("/dev/ttyUSB1", B38400) != -1 )
    {
    
        while ( true )
        {
            bytesRead = port.read( buf, 8 );
            
            if ( bytesRead == 1 )
            {
                // response message
                cout << "Response: " << (char)buf[0] << endl;
            }
            else if ( bytesRead == 8 )
            {
                // meter telemetry
                unsigned short *p = (unsigned short*)buf;
                unsigned short fwd = ntohs(*p++);  // fwd power
                unsigned short ref = ntohs(*p++);  // ref power
                unsigned short wtf = ntohs(*p++);  // don't know
                unsigned short eom = ntohs(*p++);  // always 0x3b3b ";;"
                
                if ( eom == 0x3b3b ) 
                {
                	if ( fwd > 0 )
                	{
		                float wattsFwd = decodePwr(fwd);
		                float wattsRef = decodePwr(ref);
		                float rawRatio = sqrt( wattsRef / wattsFwd );
		                float vswr     = (1.0 + rawRatio ) / (1.0 - rawRatio );
		                
		                cout << "Forward:   " << wattsFwd << " W" << endl;
		                cout << "Reflected: " << wattsRef << " W" << endl;
		                cout << "VSWR:      " << vswr << " : 1" << endl;
                    }
                }
                else
                {
                    //cout << "unknown read from port: " << 
                    //"fwd: " << std::hex << fwd << endl << 
                    //"ref: " << ref << endl <<
                    //"wtf: " << ref << endl <<
                    //"eom: " << eom << endl;                    
                }              
            }
            else
            {
                //cout << "bytes read == " << bytesRead << endl;
            }
        }
    }
    else
    {
    	cout << "problem opening port" << endl;
    }
    return 0;
}
