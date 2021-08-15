#include "grblport.h"
#include "usersettings.h"

GRBLPort::GRBLPort()
{
    _port.setParity(QSerialPort::NoParity);
    _port.setDataBits(QSerialPort::Data8);
    _port.setFlowControl(QSerialPort::NoFlowControl);
    _port.setStopBits(QSerialPort::OneStop);

    connect(&_timerStateQuery, SIGNAL(timeout()), this, SLOT(onTimerStateQuery()));
    connect(&_port, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(onSerialPortError(QSerialPort::SerialPortError)));
    _timerStateQuery.start();
}

void GRBLPort::resetDevice() {
    _port.write(QByteArray(1, (char)24));
    _reseting = true;
}

void GRBLPort::onReady()
{
    while (_port.canReadLine()) {
        QString data = _port.readLine().trimmed();

        // Filter prereset responses
        if (_reseting) {
            if (QRegExp("^GRBL|GCARVIN\\s\\d\\.\\d.").indexIn(data.toUpper()) != -1) continue;
            else {
                _reseting = false;
                _timerStateQuery.setInterval(UserSettings::getInstance()->getInt("queryStateTime"));
            }
        }

        // Status response
        if (data[0] == '<') {
            int status = -1;

            _statusReceived = true;

            // Update machine coordinates
            QRegExp mpx("MPos:([^,]*),([^,]*),([^,^>]*)");
            if (mpx.indexIn(data) != -1) {
                _machine_position[0] = mpx.cap(1).toDouble();
                _machine_position[1] = mpx.cap(2).toDouble();
                _machine_position[2] = mpx.cap(3).toDouble();
                emit receivedCoordinates(_machine_position[0], _machine_position[1], _machine_position[2]);
            }

            // Status
            QRegExp stx("<([^,^>]*)");
            if (stx.indexIn(data) != -1) {
                status = MACHINE_STATUS_CODE.indexOf(stx.cap(1));

                // Undetermined status
                if (status == -1) status = 0;

                // Update status
                _timerStateQuery.stop();
                emit statusChanged(static_cast<MACHINE_STATUS>(status));
                _timerStateQuery.setInterval(UserSettings::getInstance()->getInt("queryStateTime"));
                _timerStateQuery.start();

                // Abort
                static double x = sNan;
                static double y = sNan;
                static double z = sNan;

                if (_aborting && (status == HOLD || status == QUEUE)) {
                    if (!_reseting && (_machine_position[0] == x && _machine_position[1] == y && _machine_position[2] == z)) {
                        x = sNan;
                        y = sNan;
                        z = sNan;

                        emit resetCompleted();
                    } else {
                        x = _machine_position[0];
                        y = _machine_position[1];
                        z = _machine_position[2];
                    }
                }
            }

            // Update work coordinates
            QRegExp wpx("WPos:([^,]*),([^,]*),([^,^>]*)");
            if (wpx.indexIn(data) != -1)
            {
                emit receivedWorkCoordinates(wpx.cap(1).toDouble(), wpx.cap(2).toDouble(), wpx.cap(3).toDouble());
            }

        } else if (data.length() > 0) {
            emit dataReceived(data);
        } else {
            // Blank response
//            ui->txtConsole->appendPlainText(data);
        }
    }

}

void GRBLPort::onTimerStateQuery()
{
    if (_port.isOpen() && _resetCompleted && _statusReceived) {
        _port.write(QByteArray(1, '?'));
        _statusReceived = false;
    }
}

void GRBLPort::onSerialPortError(QSerialPort::SerialPortError error)
{
    static QSerialPort::SerialPortError previousError;

    if (error != QSerialPort::NoError && error != previousError) {
        emit receivedError(error, _port.errorString());
        if (_port.isOpen()) {
            _port.close();
//            updateControlsState();
        }
        previousError = error;
    }
//    if (error == QSerialPort::ResourceError) m_serialPort.close();
}
