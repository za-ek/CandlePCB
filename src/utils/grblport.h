#ifndef GRBLPORT_H
#define GRBLPORT_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

enum MACHINE_STATUS {UNKNOWN, IDLE, ALARM, RUN, HOME, HOLD, QUEUE, CHECK, DOOR};
const QStringList MACHINE_STATUS_CODE({"Unknown", "Idle", "Alarm", "Run", "Home", "Hold", "Queue", "Check", "Door"});
enum coordinates {X, Y, Z};

class GRBLPort : public QObject
{
    Q_OBJECT
public:
    GRBLPort();
    ~GRBLPort() { if(_port.isOpen()) _port.close(); }
    void resetDevice();
    void abort() { if(_status != IDLE) _aborting = true; }

    double getCoordinate(coordinates c) { return _machine_position[c]; }

    /**
     * Pass functions to port
     */
    void write(QString s) { _port.write(s.toLatin1()); }
    void setPortName(QString n) { _port.setPortName(n); }
    void setBaudRate(QString n) { _port.setBaudRate(n.toInt()); }
    bool isOpen() { return _port.isOpen(); }
    void openPort() { if (!_port.isOpen() && _port.open(QIODevice::ReadWrite)) resetDevice(); }

private:
    QSerialPort _port;

    bool _aborting = false;
    double _machine_position[3];
    bool _reseting = false;
    bool _statusReceived;
    bool _resetCompleted;
    int _status;
    bool dataIsReset(QString data);

    QTimer _timerStateQuery;

private slots:
    void onTimerStateQuery();
    void onReady();
    void onSerialPortError(QSerialPort::SerialPortError e);

signals:
    void receivedCoordinates(double x, double y, double z);
    void statusChanged(MACHINE_STATUS status);
    void resetCompleted();
    void receivedWorkCoordinates(double x, double y, double z);
    void dataReceived(QString data);
    void receivedError(int err_n, QString err);

};

#endif // GRBLPORT_H
