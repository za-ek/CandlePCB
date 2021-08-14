#include "serialport.h"

SerialPort::SerialPort() : QSerialPort()
{
    setParity(QSerialPort::NoParity);
    setDataBits(QSerialPort::Data8);
    setFlowControl(QSerialPort::NoFlowControl);
    setStopBits(QSerialPort::OneStop);
}

SerialPort::_reset() {
    write(QByteArray(1, (char)24));
    _reseting = true;
}

SerialPort::onReady()
{
    while (canReadLine()) {
        QString data = readLine().trimmed();

        // Filter prereset responses
        if (_reseting) {
            if (!dataIsReset(data)) continue;
            else {
                _reseting = false;
                m_timerStateQuery.setInterval(UserSettings::getInstance()->getInt("queryStateTime"));
            }
        }

        // Status response
        if (data[0] == '<') {
            int status = -1;

            m_statusReceived = true;

            // Update machine coordinates
            QRegExp mpx("MPos:([^,]*),([^,]*),([^,^>]*)");
            if (mpx.indexIn(data) != -1) {
                setMachinePosition(X, mpx.cap(1));
                setMachinePosition(Y, mpx.cap(2));
                setMachinePosition(Z, mpx.cap(3));
            }

            // Status
            QRegExp stx("<([^,^>]*)");
            if (stx.indexIn(data) != -1) {
                status = m_status.indexOf(stx.cap(1));

                // Undetermined status
                if (status == -1) status = 0;

                // Update status
                if (status != m_lastGrblStatus) {
                    setMachineStatus(status);
                }

                // Update controls
                ui->cmdRestoreOrigin->setEnabled(status == IDLE);
                ui->cmdSafePosition->setEnabled(status == IDLE);
                ui->cmdZeroXY->setEnabled(status == IDLE);
                ui->cmdZeroZ->setEnabled(status == IDLE);
                ui->chkTestMode->setEnabled(status != RUN && !m_processingFile);
                ui->chkTestMode->setChecked(status == CHECK);
                ui->cmdFilePause->setChecked(status == HOLD || status == QUEUE);
#ifdef WINDOWS
                if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
                    if (m_taskBarProgress) m_taskBarProgress->setPaused(status == HOLD || status == QUEUE);
                }
#endif

                // Update "elapsed time" timer
                if (m_processingFile) {
                    QTime time(0, 0, 0);
                    int elapsed = m_startTime.elapsed();
                    ui->glwVisualizer->setSpendTime(time.addMSecs(elapsed));
                }

                // Test for job complete
                if (m_processingFile && m_transferCompleted &&
                        ((status == IDLE && m_lastGrblStatus == RUN) || status == CHECK)) {
                    qDebug() << "job completed:" << m_fileCommandIndex << m_currentModel->rowCount() - 1;

                    // Shadow last segment
                    GcodeViewParse *parser = m_currentDrawer->viewParser();
                    QList<LineSegment*> list = parser->getLineSegmentList();
                    if (m_lastDrawnLineIndex < list.count()) {
                        list[m_lastDrawnLineIndex]->setDrawn(true);
                        m_currentDrawer->update(QList<int>() << m_lastDrawnLineIndex);
                    }

                    // Update state
                    m_processingFile = false;
                    m_fileProcessedCommandIndex = 0;
                    m_lastDrawnLineIndex = 0;
                    m_storedParserStatus.clear();

                    updateControlsState();

                    qApp->beep();

                    m_timerStateQuery.stop();
                    m_timerConnection.stop();

                    if(m_settings->getBool("checkedHeightMap")) {
                        if(m_heightMapMode && m_heghtMapIndex < HEIGHT_MAP_NUMBER) {
                            // Store current heightMap
                            saveHeightMap((getTodayDirectory() + "/%1.map").arg(++m_heghtMapIndex));
                            on_cmdFileSend_clicked();
                        } else if (m_heightMapMode) {
                            saveHeightMap((getTodayDirectory() + "/%1.map").arg(++m_heghtMapIndex));
                            resetHeightmap();
                            on_cmdHeightMapMode_toggled(false);

                            // Generate target heightmap
                            // Run php file stored in resources
                            QFile f(":/scripts/map.php");
                            QTemporaryFile *tmpFile = QTemporaryFile::createNativeFile(f);
                            if (tmpFile->open()) {
                                QProcess *process = new QProcess(this);
                                QStringList params;
                                qDebug() << tmpFile->fileName();
                                params << tmpFile->fileName().toLocal8Bit();
                                params << getTodayDirectory();
                                qDebug() << params;
                                process->start("php", params);
                                process->waitForFinished();
                                tmpFile->close();
                            } else {
                                qDebug() << "Error open temp file";
                            }

                            ui->chkHeightMapUse->setChecked(true);
                            useHeightmap(getTodayDirectory() + "/target.map");
                        } else {
                            QMessageBox::information(this, qApp->applicationDisplayName(), tr("Job done.\nTime elapsed: %1")
                                                 .arg(ui->glwVisualizer->spendTime().toString("hh:mm:ss")));
                        }
                    } else {
                        QMessageBox::information(this, qApp->applicationDisplayName(), tr("Job done.\nTime elapsed: %1")
                                             .arg(ui->glwVisualizer->spendTime().toString("hh:mm:ss")));
                    }

                    m_timerStateQuery.setInterval(UserSettings::getInstance()->getInt("queryStateTime"));
                    m_timerConnection.start();
                    m_timerStateQuery.start();
                }

                // Store status
                if (status != m_lastGrblStatus) m_lastGrblStatus = status;

                // Abort
                static double x = sNan;
                static double y = sNan;
                static double z = sNan;

                if (m_aborting) {
                    switch (status) {
                    case IDLE: // Idle
                        if (!m_processingFile && m_resetCompleted) {
                            m_aborting = false;
                            restoreOffsets();
                            restoreParserState();
                            return;
                        }
                        break;
                    case HOLD: // Hold
                    case QUEUE:
                        if (!m_reseting && compareCoordinates(x, y, z)) {
                            x = sNan;
                            y = sNan;
                            z = sNan;
                            grblReset();
                        } else {
                            x = getMachinePosition(X);
                            y = getMachinePosition(Y);
                            z = getMachinePosition(Z);
                        }
                        break;
                    }
                }
            }

            // Update work coordinates
            QRegExp wpx("WPos:([^,]*),([^,]*),([^,^>]*)");
            if (wpx.indexIn(data) != -1)
            {
                ui->txtWPosX->setText(wpx.cap(1));
                ui->txtWPosY->setText(wpx.cap(2));
                ui->txtWPosZ->setText(wpx.cap(3));

                // Update tool position
                if (!(status == CHECK && m_fileProcessedCommandIndex < m_currentModel->rowCount() - 1)) {
                    m_toolDrawer.setToolPosition(QVector3D(toMetric(ui->txtWPosX->text().toDouble()),
                                                           toMetric(ui->txtWPosY->text().toDouble()),
                                                           toMetric(ui->txtWPosZ->text().toDouble())));
                }

                // toolpath shadowing
                if (m_processingFile && status != CHECK) {
                    GcodeViewParse *parser = m_currentDrawer->viewParser();

                    bool toolOntoolpath = false;

                    QList<int> drawnLines;
                    QList<LineSegment*> list = parser->getLineSegmentList();

                    for (int i = m_lastDrawnLineIndex; i < list.count()
                         && list[i]->getLineNumber()
                         <= (m_currentModel->data(m_currentModel->index(m_fileProcessedCommandIndex, 4)).toInt() + 1); i++) {
                        if (list[i]->contains(m_toolDrawer.toolPosition())) {
                            toolOntoolpath = true;
                            m_lastDrawnLineIndex = i;
                            break;
                        }
                        drawnLines << i;
                    }

                    if (toolOntoolpath) {
                        foreach (int i, drawnLines) {
                            list[i]->setDrawn(true);
                        }
                        if (!drawnLines.isEmpty()) m_currentDrawer->update(drawnLines);
                    } else if (m_lastDrawnLineIndex < list.count()) {
                        qDebug() << "tool missed:" << list[m_lastDrawnLineIndex]->getLineNumber()
                                 << m_currentModel->data(m_currentModel->index(m_fileProcessedCommandIndex, 4)).toInt()
                                 << m_fileProcessedCommandIndex;
                    }
                }
            }

        } else if (data.length() > 0) {

            // Processed commands
            if (m_commands.length() > 0 && !dataIsFloating(data)
                    && !(m_commands[0].command != "[CTRL+X]" && dataIsReset(data))) {

                static QString response; // Full response string

                if ((m_commands[0].command != "[CTRL+X]" && dataIsEnd(data))
                        || (m_commands[0].command == "[CTRL+X]" && dataIsReset(data))) {

                    response.append(data);

                    // Take command from buffer
                    CommandAttributes ca = m_commands.takeFirst();
                    QTextBlock tb = ui->txtConsole->document()->findBlockByNumber(ca.consoleIndex);
                    QTextCursor tc(tb);

                    // Restore absolute/relative coordinate system after jog
                    if (ca.command.toUpper() == "$G" && ca.tableIndex == -2) {
                        if (ui->chkKeyboardControl->isChecked()) m_absoluteCoordinates = response.contains("G90");
                        else if (response.contains("G90")) sendCommand("G90", -1, m_settings->getBool("showUICommands"));
                    }

                    // Process parser status
                    if (ca.command.toUpper() == "$G" && ca.tableIndex == -3) {
                        // Update status in visualizer window
                        ui->glwVisualizer->setParserStatus(response.left(response.indexOf("; ")));

                        // Store parser status
                        if (m_processingFile) storeParserState();

                        // Process spindle state
                        if (!response.contains("M5")) {
                            m_spindleCW = response.contains("M3");
                            m_timerToolAnimation.start(25, this);
                            m_programSpeed = true;
                            ui->cmdSpindle->setChecked(true);
                            m_programSpeed = false;
                        } else {
                            m_timerToolAnimation.stop();
                            m_programSpeed = true;
                            ui->cmdSpindle->setChecked(false);
                            m_programSpeed = false;
                        }

                        // Spindle speed
                        QRegExp rx(".*S([\\d\\.]+)");
                        if (rx.indexIn(response) != -1) {
                            double speed = toMetric(rx.cap(1).toDouble()); //RPM in imperial?
                            if (fabs(ui->txtSpindleSpeed->value() - speed) < 2.54) ui->txtSpindleSpeed->setStyleSheet("color: palette(text);");
                        }

                        // Feed
                        rx.setPattern(".*F([\\d\\.]+)");
                        if (rx.indexIn(response) != -1) {
                            double feed = toMetric(rx.cap(1).toDouble());
                            double set = ui->chkFeedOverride->isChecked() ? m_originalFeed / 100 * ui->txtFeed->value()
                                                                          : m_originalFeed;
                            if (response.contains("G20")) set *= 25.4;
                            if (fabs(feed - set) < 2.54) ui->txtFeed->setStyleSheet("color: palette(text);");
                        }

                        m_updateParserStatus = true;
                    }

                    // Store origin
                    if (ca.command == "$#" && ca.tableIndex == -2) {
                        qDebug() << "Received offsets:" << response;
                        QRegExp rx(".*G92:([^,]*),([^,]*),([^\\]]*)");

                        if (rx.indexIn(response) != -1) {
                            if (m_settingZeroXY) {
                                m_settingZeroXY = false;
                                m_storedX = toMetric(rx.cap(1).toDouble());
                                m_storedY = toMetric(rx.cap(2).toDouble());
                            } else if (m_settingZeroZ) {
                                m_settingZeroZ = false;
                                m_storedZ = toMetric(rx.cap(3).toDouble());
                            }
                            ui->cmdRestoreOrigin->setToolTip(QString(tr("Restore origin:\n%1, %2, %3")).arg(m_storedX).arg(m_storedY).arg(m_storedZ));
                        }
                    }

                    // Homing response
                    if ((ca.command.toUpper() == "$H" || ca.command.toUpper() == "$T") && m_homing) {
                        m_homing = false;
                        if(m_settings->getBool("restoreZeroAfterHoming")) {
                            m_settingZeroXY = true;
                            sendCommand("G92X0Y0", -1, m_settings->getBool("showUICommands"));
                            sendCommand("$#", -2, m_settings->getBool("showUICommands"));
                        }
                    }

                    // Reset complete
                    if (ca.command == "[CTRL+X]") {
                        m_resetCompleted = true;
                        m_updateParserStatus = true;
                    }

                    // Clear command buffer on "M2" & "M30" command (old firmwares)
                    if ((ca.command.contains("M2") || ca.command.contains("M30")) && response.contains("ok") && !response.contains("[Pgm End]")) {
                        m_commands.clear();
                        m_queue.clear();
                    }

                    // Process probing on heightmap mode only from table commands
                    if (ca.command.contains("G38.2") && m_heightMapMode && ca.tableIndex > -1) {
                        // Get probe Z coordinate
                        // "[PRB:0.000,0.000,0.000:0];ok"
                        QRegExp rx(".*PRB:([^,]*),([^,]*),([^]^:]*)");
                        double z = qQNaN();
                        if (rx.indexIn(response) != -1) {
                            qDebug() << "probing coordinates:" << rx.cap(1) << rx.cap(2) << rx.cap(3);
                            z = toMetric(rx.cap(3).toDouble());
                        }

                        static double firstZ;
                        if (m_probeIndex == -1) {
                            firstZ = z;
                            z = 0;
                        } else {
                            // Calculate delta Z
                            z -= firstZ;

                            // Calculate table indexes
                            int row = trunc(m_probeIndex / m_heightMapModel.columnCount());
                            int column = m_probeIndex - row * m_heightMapModel.columnCount();
                            if (row % 2) column = m_heightMapModel.columnCount() - 1 - column;

                            // Store Z in table
                            m_heightMapModel.setData(m_heightMapModel.index(row, column), z, Qt::UserRole);
                            ui->tblHeightMap->update(m_heightMapModel.index(m_heightMapModel.rowCount() - 1 - row, column));
                            updateHeightMapInterpolationDrawer();
                        }

                        m_probeIndex++;
                    }

                    // Change state query time on check mode on
                    if (ca.command.contains(QRegExp("$[cC]"))) {
                        m_timerStateQuery.setInterval(response.contains("Enable") ? 1000 : UserSettings::getInstance()->getInt("queryStateTime"));
                    }

                    // Add response to console
                    if (tb.isValid() && tb.text() == ca.command) {

                        bool scrolledDown = ui->txtConsole->verticalScrollBar()->value() == ui->txtConsole->verticalScrollBar()->maximum();

                        // Update text block numbers
                        int blocksAdded = response.count("; ");

                        if (blocksAdded > 0) for (int i = 0; i < m_commands.count(); i++) {
                            if (m_commands[i].consoleIndex != -1) m_commands[i].consoleIndex += blocksAdded;
                        }

                        tc.beginEditBlock();
                        tc.movePosition(QTextCursor::EndOfBlock);

                        tc.insertText(" < " + QString(response).replace("; ", "\r\n"));
                        tc.endEditBlock();

                        if (scrolledDown) ui->txtConsole->verticalScrollBar()->setValue(ui->txtConsole->verticalScrollBar()->maximum());
                    }

                    // Check queue
                    if (m_queue.length() > 0) {
                        CommandQueue cq = m_queue.takeFirst();
                        while ((bufferLength() + cq.command.length() + 1) <= BUFFERLENGTH) {
                            sendCommand(cq.command, cq.tableIndex, cq.showInConsole);
                            if (m_queue.isEmpty()) break; else cq = m_queue.takeFirst();
                        }
                    }

                    // Add response to table, send next program commands
                    if (m_processingFile) {

                        // Only if command from table
                        if (ca.tableIndex > -1) {
                            m_currentModel->setData(m_currentModel->index(ca.tableIndex, 2), tr("Processed"));
                            m_currentModel->setData(m_currentModel->index(ca.tableIndex, 3), response);

                            m_fileProcessedCommandIndex = ca.tableIndex;

                            if (ui->chkAutoScroll->isChecked() && ca.tableIndex != -1) {
                                ui->tblProgram->scrollTo(m_currentModel->index(ca.tableIndex + 1, 0));
                                ui->tblProgram->setCurrentIndex(m_currentModel->index(ca.tableIndex, 1));
                            }
                        }

                        // Update taskbar progress
#ifdef WINDOWS
                        if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) {
                            if (m_taskBarProgress) m_taskBarProgress->setValue(m_fileProcessedCommandIndex);
                        }
#endif

                        // Check transfer complete (last row always blank, last command row = rowcount - 2)
                        if (m_fileProcessedCommandIndex == m_currentModel->rowCount() - 2
                                || ca.command.contains(QRegExp("M0*2|M30"))) m_transferCompleted = true;
                        // Send next program commands
                        else if (!m_fileEndSent && (m_fileCommandIndex < m_currentModel->rowCount())) sendNextFileCommands();
                    }

                    // Scroll to first line on "M30" command
                    if (ca.command.contains("M30")) ui->tblProgram->setCurrentIndex(m_currentModel->index(0, 1));

                    // Toolpath shadowing on check mode
                    if (machineStatus == CHECK) {
                        GcodeViewParse *parser = m_currentDrawer->viewParser();
                        QList<LineSegment*> list = parser->getLineSegmentList();

                        if (!m_transferCompleted && m_fileProcessedCommandIndex < m_currentModel->rowCount() - 1) {
                            int i;
                            QList<int> drawnLines;

                            for (i = m_lastDrawnLineIndex; i < list.count()
                                 && list[i]->getLineNumber()
                                 <= (m_currentModel->data(m_currentModel->index(m_fileProcessedCommandIndex, 4)).toInt()); i++) {
                                drawnLines << i;
                            }

                            if (!drawnLines.isEmpty() && (i < list.count())) {
                                m_lastDrawnLineIndex = i;
                                QVector3D vec = list[i]->getEnd();
                                m_toolDrawer.setToolPosition(vec);
                            }

                            foreach (int i, drawnLines) {
                                list[i]->setDrawn(true);
                            }
                            if (!drawnLines.isEmpty()) m_currentDrawer->update(drawnLines);
                        } else {
                            foreach (LineSegment* s, list) {
                                if (!qIsNaN(s->getEnd().length())) {
                                    m_toolDrawer.setToolPosition(s->getEnd());
                                    break;
                                }
                            }
                        }
                    }

                    response.clear();
                } else {
                    response.append(data + "; ");
                }

            } else {
                // Unprocessed responses
                qDebug() << "floating response:" << data;

                // Handle hardware reset
                if (dataIsReset(data)) {
                    qDebug() << "hardware reset";

                    m_processingFile = false;
                    m_transferCompleted = true;
                    m_fileCommandIndex = 0;

                    m_reseting = false;
                    m_homing = false;
                    m_lastGrblStatus = -1;

                    m_updateParserStatus = true;
                    m_statusReceived = true;

                    m_commands.clear();
                    m_queue.clear();

                    updateControlsState();
                }
                ui->txtConsole->appendPlainText(data);
            }
        } else {
            // Blank response
//            ui->txtConsole->appendPlainText(data);
        }
    }

}

bool SerialPort::dataIsReset(QString data) {
    return QRegExp("^GRBL|GCARVIN\\s\\d\\.\\d.").indexIn(data.toUpper()) != -1;
}

void frmMain::onTimerStateQuery()
{
    if (m_serialPort.isOpen() && m_resetCompleted && m_statusReceived) {
        m_serialPort.write(QByteArray(1, '?'));
        m_statusReceived = false;
    }

    ui->glwVisualizer->setBufferState(QString(tr("Buffer: %1 / %2")).arg(bufferLength()).arg(m_queue.length()));
}
