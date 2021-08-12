#include "serialport.h"

SerialPort::SerialPort() : QSerialPort()
{
    setParity(QSerialPort::NoParity);
    setDataBits(QSerialPort::Data8);
    setFlowControl(QSerialPort::NoFlowControl);
    setStopBits(QSerialPort::OneStop);
}
