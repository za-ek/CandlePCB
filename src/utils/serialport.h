#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>

class SerialPort : public QSerialPort
{
public:
    SerialPort();
    onReady();
    _reset();
private:
    bool _reseting = false;
    bool dataIsReset(QString data);
};

#endif // SERIALPORT_H
