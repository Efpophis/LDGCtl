#ifndef SERIAL_H
#define SERIAL_H

#include <termios.h>

class SerialPort
{
    public:
        SerialPort();
        virtual ~SerialPort();
        
        int init(const char* portname, int baud=B9600, int parity=0, int blocking=1);
        
        int write( const void* buf, unsigned int nbytes );
        int read( void* buf, unsigned int nbytes );
        
        //void close();
        
        void setBlocking( int should_block );
           
    private:
        int m_fd;
        
        int setAttributes(int speed, int parity);
};

#endif
