#ifndef SERIALCOM_H
#define SERIALCOM_H
#include <iostream>
#include <string>

class SerialCom
{

    public:
        SerialCom();
        SerialCom(std::string dev, int baudrate);
        ~SerialCom();
        void Write(std::string buff);
        std::string Read();
    private:
        int fd;
        int baud;
        std::string port;
        void Connect();
        void Disconnect();
        int set_interface_attribs (int fd, int speed, int parity);
        void set_blocking (int fd, bool should_block);

};
#endif