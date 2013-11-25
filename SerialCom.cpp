#include "SerialCom.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>
#include <memory>

SerialCom::SerialCom() : port("/dev/ttyUSB0"), baud(B57600)
{
    Connect();
}

SerialCom::SerialCom(std::string dev, int baudrate): port(dev), baud(baudrate)
{
    Connect();
}

void SerialCom::Connect()
{
    const char* portname = port.c_str();
    fd = open(portname,O_RDWR | O_NOCTTY | O_SYNC );

    if (fd < 0 ) {
        std::cerr << "Error opening " << port << std::endl;
        exit(1);
    }

    set_interface_attribs(fd, baud, 0);
    set_blocking(fd, false);
}

SerialCom::~SerialCom()
{
    Disconnect();
}

void SerialCom::Disconnect()
{
    if (fd) {
        close(fd);
    }
}

int SerialCom::set_interface_attribs (int fd, int speed, int parity)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);

    if (tcgetattr (fd, &tty) != 0) {
        printf ("error %d from tcgetattr", errno);
        return -1;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_iflag &= ~IGNBRK;         // ignore break signal
    tty.c_lflag = 0;                // no signaling chars, no echo,
    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
    tty.c_iflag |= IXON ; // shut off xon/xoff ctrl
    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (fd, TCSANOW, &tty) != 0) {
        printf ("error %d from tcsetattr", errno);
        return -1;
    }

    return 0;
}

void SerialCom::set_blocking (int fd, bool should_block)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);

    if (tcgetattr (fd, &tty) != 0) {
        printf ("error %d from tggetattr", errno);
        return;
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    { printf ("error %d setting term attributes", errno); }
}

void SerialCom::Write(std::string buf)
{
    const char* to_write = buf.c_str();
    write(fd, to_write, strlen(to_write));
}

std::string SerialCom::Read()
{
    // std::unique_ptr<char> buf(new char(1023));
    char buf[1024];
    read(fd, buf, 1024);
    std::string ret;

    for (size_t i = 0 ; i != '\0'; i++) {
        ret += buf[i];
    }

    return ret;
}
