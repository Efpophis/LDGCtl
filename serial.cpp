#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "serial.h"

using namespace std;

#ifndef RELEASE_BUILD
#define error_message(...) fprintf( stderr, __VA_ARGS__)
#else
#define error_message(...) // do nothing
#endif

SerialPort::SerialPort() : m_fd(-1)
{
}

SerialPort::~SerialPort()
{
    close(m_fd);
}

int SerialPort::init(const char* portname, int baud, int parity, int blocking)
{
    int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
            error_message("error %d opening %s: %s", errno, portname, strerror (errno));
            return -1;
    }

    m_fd = fd;
    setAttributes(baud, parity);  // set speed and parity

    setBlocking(blocking);        // set blocking

    return m_fd;
}

int SerialPort::read( void* buf, unsigned int nbytes )
{
    return ::read( m_fd, buf, nbytes );
}

int SerialPort::write( const void* buf, unsigned int nbytes )
{
    return ::write(m_fd, buf, nbytes );
}

int SerialPort::readFully( void* buf, unsigned int nbytes )
{
	int bytesRead = ::read( m_fd, buf, nbytes );
	
	while ( bytesRead != -1 && bytesRead < nbytes )
	{
		int result = ::read( m_fd, (unsigned char*)buf + bytesRead , nbytes - bytesRead );
		
		if ( result > 0 )
		{
			bytesRead += result;
		}
		else
		{	
			// error
			bytesRead = -1;			
		}			
	}
	return bytesRead;
}

int SerialPort::setAttributes (int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (m_fd, &tty) != 0)
        {
                error_message ("error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        // ? tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (m_fd, TCSANOW, &tty) != 0)
        {
                error_message ("error %d from tcsetattr", errno);
                return -1;
        }
        return 0;
}

void SerialPort::setBlocking (int should_block)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (m_fd, &tty) != 0)
    {
            error_message ("error %d from tggetattr", errno);
            return;
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 10;            // 0.5 seconds read timeout

    if (tcsetattr (m_fd, TCSANOW, &tty) != 0)
    {
        error_message ("error %d setting term attributes", errno);
    }
}
