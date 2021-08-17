/********************************************************************************
** Form generated from reading UI file 'frmsettings.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRMSETTINGS_H
#define UI_FRMSETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <widgets/groupbox.h>
#include "widgets/colorpicker.h"

QT_BEGIN_NAMESPACE

class Ui_frmSettings
{
public:
    QGridLayout *gridLayout_13;
    QHBoxLayout *horizontalLayout;
    QPushButton *cmdDefaults;
    QSpacerItem *horizontalSpacer;
    QPushButton *cmdOK;
    QPushButton *cmdCancel;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_19;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_4;
    QGridLayout *gridLayout_20;
    GroupBox *grpGRBL;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout_2;
    QSpinBox *acceleration;
    QSpinBox *rapidSpeed;
    QComboBox *cboUnits;
    QLabel *label_10;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_14;
    QLabel *label_11;
    QSpinBox *queryStateTime;
    QLabel *label_15;
    GroupBox *grpParser;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_32;
    QSpacerItem *horizontalSpacer_14;
    QGridLayout *gridLayout_7;
    QRadioButton *radArcLengthMode;
    QDoubleSpinBox *arcDegree;
    QSpacerItem *horizontalSpacer_15;
    QDoubleSpinBox *arcLength;
    QRadioButton *arcDegreeMode;
    GroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_17;
    QLineEdit *touchCommand;
    QLabel *label_33;
    QLineEdit *safeCommand;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_8;
    QCheckBox *moveOnRestore;
    QComboBox *cboRestoreMode;
    QSpacerItem *horizontalSpacer_16;
    QGridLayout *gridLayout_8;
    QSpinBox *spindleSpeedMin;
    QLabel *label_13;
    QLabel *label_12;
    QSpinBox *spindleSpeedMax;
    QSpacerItem *horizontalSpacer_9;
    QCheckBox *restoreZeroAfterHoming;
    GroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_11;
    QGridLayout *gridLayout_9;
    QLabel *label_38;
    QSpacerItem *horizontalSpacer_8;
    QSpinBox *heightMapProbingFeed;
    QCheckBox *checkedHeightMap;
    GroupBox *grpConnection;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QComboBox *port;
    QToolButton *cmdRefresh;
    QLabel *label_2;
    QComboBox *baud;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_10;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_34;
    QSpacerItem *horizontalSpacer_7;
    QComboBox *cboUnitsCommon;
    QWidget *tab_5;
    QGridLayout *gridLayout_14;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_16;
    GroupBox *grpPanels;
    QGridLayout *gridLayout_16;
    QCheckBox *panelHeightmap;
    QCheckBox *panelFeed;
    QCheckBox *panelJog;
    QCheckBox *panelSpindle;
    GroupBox *grpColors;
    QVBoxLayout *verticalLayout_7;
    QGridLayout *gridLayout_6;
    QLabel *label_20;
    QLabel *label_21;
    ColorPicker *toolpathZMovementColor;
    QLabel *label_24;
    ColorPicker *toolpathHighlightColor;
    ColorPicker *toolpathStartColor;
    QLabel *label_25;
    QLabel *label_26;
    ColorPicker *toolpathEndColor;
    QLabel *label_27;
    ColorPicker *visualizerBackgroundColor;
    ColorPicker *visualizerTextColor;
    QLabel *label_22;
    QLabel *label_28;
    QLabel *label_19;
    ColorPicker *toolColor;
    QSpacerItem *verticalSpacer;
    QLabel *label_16;
    ColorPicker *toolpathDrawnColor;
    ColorPicker *toolpathNormalColor;
    QLabel *label_23;
    QGroupBox *grpConsole;
    QVBoxLayout *verticalLayout_12;
    QCheckBox *showProgramCommands;
    QCheckBox *autoCompletion;
    QCheckBox *showUICommands;
    GroupBox *groupBox;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_29;
    QComboBox *cboFontSize;
    QLabel *label_30;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_31;
    QSpacerItem *horizontalSpacer_3;
    QWidget *tab_3;
    QGridLayout *gridLayout_15;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_3;
    QGridLayout *gridLayout_18;
    GroupBox *grpVisualizer;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout_5;
    QCheckBox *antialiasing;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_8;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_9;
    QCheckBox *vsync;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QComboBox *cboFps;
    QCheckBox *simplify;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QDoubleSpinBox *lineWidth;
    QCheckBox *msaa;
    QCheckBox *zBuffer;
    QDoubleSpinBox *simplifyPrecision;
    GroupBox *grpTool;
    QGridLayout *gridLayout;
    QDoubleSpinBox *toolAngle;
    QDoubleSpinBox *toolLength;
    QLabel *label_3;
    QLabel *lblToolLength;
    QDoubleSpinBox *toolDiameter;
    QLabel *label_4;
    QLabel *lblToolAngle;
    QComboBox *cboToolType;
    QSpacerItem *horizontalSpacer_4;
    QWidget *tab_2;
    QGridLayout *gridLayout_10;
    QScrollArea *scrollArea_4;
    QWidget *scrollAreaWidgetContents_5;
    QGridLayout *gridLayout_21;
    QPlainTextEdit *pcb2gcodeArgs;
    QLabel *label_40;

    void setupUi(QDialog *frmSettings)
    {
        if (frmSettings->objectName().isEmpty())
            frmSettings->setObjectName(QString::fromUtf8("frmSettings"));
        frmSettings->resize(520, 538);
        frmSettings->setStyleSheet(QString::fromUtf8("/*QWidget {\n"
"	font-size: 9pt;\n"
"}*/\n"
"\n"
"QSpinBox, QDoubleSpinBox {\n"
"	padding-top: 1px;\n"
"	padding-bottom: 1px;\n"
"}\n"
"\n"
"QGroupBox {\n"
"	border: none;\n"
"	padding-top: 16;\n"
"	font-weight: bold;\n"
"}"));
        frmSettings->setModal(true);
        gridLayout_13 = new QGridLayout(frmSettings);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cmdDefaults = new QPushButton(frmSettings);
        cmdDefaults->setObjectName(QString::fromUtf8("cmdDefaults"));
        cmdDefaults->setAutoDefault(false);

        horizontalLayout->addWidget(cmdDefaults);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cmdOK = new QPushButton(frmSettings);
        cmdOK->setObjectName(QString::fromUtf8("cmdOK"));
        cmdOK->setAutoDefault(false);

        horizontalLayout->addWidget(cmdOK);

        cmdCancel = new QPushButton(frmSettings);
        cmdCancel->setObjectName(QString::fromUtf8("cmdCancel"));
        cmdCancel->setAutoDefault(false);

        horizontalLayout->addWidget(cmdCancel);


        gridLayout_13->addLayout(horizontalLayout, 1, 0, 1, 1);

        tabWidget = new QTabWidget(frmSettings);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_19 = new QGridLayout(tab);
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        scrollArea_3 = new QScrollArea(tab);
        scrollArea_3->setObjectName(QString::fromUtf8("scrollArea_3"));
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_4 = new QWidget();
        scrollAreaWidgetContents_4->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_4"));
        scrollAreaWidgetContents_4->setGeometry(QRect(0, 0, 464, 680));
        gridLayout_20 = new QGridLayout(scrollAreaWidgetContents_4);
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        grpGRBL = new GroupBox(scrollAreaWidgetContents_4);
        grpGRBL->setObjectName(QString::fromUtf8("grpGRBL"));
        verticalLayout_5 = new QVBoxLayout(grpGRBL);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        acceleration = new QSpinBox(grpGRBL);
        acceleration->setObjectName(QString::fromUtf8("acceleration"));
        QFont font;
        font.setPointSize(9);
        acceleration->setFont(font);
        acceleration->setAlignment(Qt::AlignCenter);
        acceleration->setButtonSymbols(QAbstractSpinBox::NoButtons);
        acceleration->setMaximum(99999);

        gridLayout_2->addWidget(acceleration, 1, 4, 1, 1);

        rapidSpeed = new QSpinBox(grpGRBL);
        rapidSpeed->setObjectName(QString::fromUtf8("rapidSpeed"));
        rapidSpeed->setFont(font);
        rapidSpeed->setAlignment(Qt::AlignCenter);
        rapidSpeed->setButtonSymbols(QAbstractSpinBox::NoButtons);
        rapidSpeed->setMaximum(99999);

        gridLayout_2->addWidget(rapidSpeed, 1, 1, 1, 1);

        cboUnits = new QComboBox(grpGRBL);
        cboUnits->addItem(QString());
        cboUnits->addItem(QString());
        cboUnits->setObjectName(QString::fromUtf8("cboUnits"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cboUnits->sizePolicy().hasHeightForWidth());
        cboUnits->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(cboUnits, 0, 4, 1, 1);

        label_10 = new QLabel(grpGRBL);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_2->addWidget(label_10, 1, 0, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(6, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_6, 0, 2, 1, 1);

        label_14 = new QLabel(grpGRBL);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_2->addWidget(label_14, 1, 3, 1, 1);

        label_11 = new QLabel(grpGRBL);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_2->addWidget(label_11, 0, 0, 1, 1);

        queryStateTime = new QSpinBox(grpGRBL);
        queryStateTime->setObjectName(QString::fromUtf8("queryStateTime"));
        queryStateTime->setFont(font);
        queryStateTime->setAlignment(Qt::AlignCenter);
        queryStateTime->setButtonSymbols(QAbstractSpinBox::NoButtons);
        queryStateTime->setMaximum(9999);

        gridLayout_2->addWidget(queryStateTime, 0, 1, 1, 1);

        label_15 = new QLabel(grpGRBL);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_2->addWidget(label_15, 0, 3, 1, 1);


        verticalLayout_5->addLayout(gridLayout_2);


        gridLayout_20->addWidget(grpGRBL, 4, 0, 1, 1);

        grpParser = new GroupBox(scrollAreaWidgetContents_4);
        grpParser->setObjectName(QString::fromUtf8("grpParser"));
        verticalLayout_8 = new QVBoxLayout(grpParser);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_32 = new QLabel(grpParser);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        horizontalLayout_11->addWidget(label_32);

        horizontalSpacer_14 = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_14);


        verticalLayout_8->addLayout(horizontalLayout_11);

        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        radArcLengthMode = new QRadioButton(grpParser);
        radArcLengthMode->setObjectName(QString::fromUtf8("radArcLengthMode"));
        radArcLengthMode->setChecked(true);

        gridLayout_7->addWidget(radArcLengthMode, 0, 0, 1, 1);

        arcDegree = new QDoubleSpinBox(grpParser);
        arcDegree->setObjectName(QString::fromUtf8("arcDegree"));
        arcDegree->setEnabled(false);
        arcDegree->setAlignment(Qt::AlignCenter);
        arcDegree->setButtonSymbols(QAbstractSpinBox::NoButtons);
        arcDegree->setDecimals(2);
        arcDegree->setMaximum(180.000000000000000);

        gridLayout_7->addWidget(arcDegree, 0, 4, 1, 1);

        horizontalSpacer_15 = new QSpacerItem(6, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_15, 0, 2, 1, 1);

        arcLength = new QDoubleSpinBox(grpParser);
        arcLength->setObjectName(QString::fromUtf8("arcLength"));
        arcLength->setEnabled(true);
        arcLength->setAlignment(Qt::AlignCenter);
        arcLength->setButtonSymbols(QAbstractSpinBox::NoButtons);
        arcLength->setDecimals(2);
        arcLength->setMaximum(99.000000000000000);

        gridLayout_7->addWidget(arcLength, 0, 1, 1, 1);

        arcDegreeMode = new QRadioButton(grpParser);
        arcDegreeMode->setObjectName(QString::fromUtf8("arcDegreeMode"));

        gridLayout_7->addWidget(arcDegreeMode, 0, 3, 1, 1);


        verticalLayout_8->addLayout(gridLayout_7);


        gridLayout_20->addWidget(grpParser, 3, 0, 1, 1);

        groupBox_2 = new GroupBox(scrollAreaWidgetContents_4);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_9 = new QVBoxLayout(groupBox_2);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        label_17 = new QLabel(groupBox_2);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setStyleSheet(QString::fromUtf8("padding-top: 3;"));
        label_17->setIndent(0);

        verticalLayout_9->addWidget(label_17);

        touchCommand = new QLineEdit(groupBox_2);
        touchCommand->setObjectName(QString::fromUtf8("touchCommand"));
        touchCommand->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(touchCommand);

        label_33 = new QLabel(groupBox_2);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setStyleSheet(QString::fromUtf8("padding-top: 3;"));
        label_33->setIndent(0);

        verticalLayout_9->addWidget(label_33);

        safeCommand = new QLineEdit(groupBox_2);
        safeCommand->setObjectName(QString::fromUtf8("safeCommand"));
        safeCommand->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(safeCommand);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        moveOnRestore = new QCheckBox(groupBox_2);
        moveOnRestore->setObjectName(QString::fromUtf8("moveOnRestore"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(moveOnRestore->sizePolicy().hasHeightForWidth());
        moveOnRestore->setSizePolicy(sizePolicy2);

        horizontalLayout_8->addWidget(moveOnRestore);


        gridLayout_4->addLayout(horizontalLayout_8, 3, 0, 1, 1);

        cboRestoreMode = new QComboBox(groupBox_2);
        cboRestoreMode->addItem(QString());
        cboRestoreMode->addItem(QString());
        cboRestoreMode->setObjectName(QString::fromUtf8("cboRestoreMode"));
        cboRestoreMode->setEnabled(false);

        gridLayout_4->addWidget(cboRestoreMode, 3, 2, 1, 1);

        horizontalSpacer_16 = new QSpacerItem(9, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_16, 3, 1, 1, 1);

        gridLayout_4->setColumnStretch(1, 1);
        gridLayout_4->setColumnStretch(2, 2);

        verticalLayout_9->addLayout(gridLayout_4);

        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        spindleSpeedMin = new QSpinBox(groupBox_2);
        spindleSpeedMin->setObjectName(QString::fromUtf8("spindleSpeedMin"));
        spindleSpeedMin->setFont(font);
        spindleSpeedMin->setAlignment(Qt::AlignCenter);
        spindleSpeedMin->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spindleSpeedMin->setMaximum(99999);

        gridLayout_8->addWidget(spindleSpeedMin, 0, 1, 1, 1);

        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_8->addWidget(label_13, 0, 3, 1, 1);

        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_8->addWidget(label_12, 0, 0, 1, 1);

        spindleSpeedMax = new QSpinBox(groupBox_2);
        spindleSpeedMax->setObjectName(QString::fromUtf8("spindleSpeedMax"));
        spindleSpeedMax->setFont(font);
        spindleSpeedMax->setAlignment(Qt::AlignCenter);
        spindleSpeedMax->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spindleSpeedMax->setMaximum(99999);

        gridLayout_8->addWidget(spindleSpeedMax, 0, 4, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(6, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_9, 0, 2, 1, 1);

        gridLayout_8->setColumnStretch(1, 1);
        gridLayout_8->setColumnStretch(4, 1);

        verticalLayout_9->addLayout(gridLayout_8);

        restoreZeroAfterHoming = new QCheckBox(groupBox_2);
        restoreZeroAfterHoming->setObjectName(QString::fromUtf8("restoreZeroAfterHoming"));

        verticalLayout_9->addWidget(restoreZeroAfterHoming);


        gridLayout_20->addWidget(groupBox_2, 1, 0, 1, 1);

        groupBox_4 = new GroupBox(scrollAreaWidgetContents_4);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_11 = new QVBoxLayout(groupBox_4);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        label_38 = new QLabel(groupBox_4);
        label_38->setObjectName(QString::fromUtf8("label_38"));

        gridLayout_9->addWidget(label_38, 3, 0, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_8, 3, 1, 1, 1);

        heightMapProbingFeed = new QSpinBox(groupBox_4);
        heightMapProbingFeed->setObjectName(QString::fromUtf8("heightMapProbingFeed"));
        heightMapProbingFeed->setFont(font);
        heightMapProbingFeed->setAlignment(Qt::AlignCenter);
        heightMapProbingFeed->setButtonSymbols(QAbstractSpinBox::NoButtons);
        heightMapProbingFeed->setMaximum(99999);

        gridLayout_9->addWidget(heightMapProbingFeed, 3, 2, 1, 1);

        gridLayout_9->setColumnStretch(1, 1);
        gridLayout_9->setColumnStretch(2, 1);

        verticalLayout_11->addLayout(gridLayout_9);

        checkedHeightMap = new QCheckBox(groupBox_4);
        checkedHeightMap->setObjectName(QString::fromUtf8("checkedHeightMap"));

        verticalLayout_11->addWidget(checkedHeightMap);


        gridLayout_20->addWidget(groupBox_4, 2, 0, 1, 1);

        grpConnection = new GroupBox(scrollAreaWidgetContents_4);
        grpConnection->setObjectName(QString::fromUtf8("grpConnection"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(1);
        sizePolicy3.setHeightForWidth(grpConnection->sizePolicy().hasHeightForWidth());
        grpConnection->setSizePolicy(sizePolicy3);
        verticalLayout = new QVBoxLayout(grpConnection);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(grpConnection);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);

        port = new QComboBox(grpConnection);
        port->setObjectName(QString::fromUtf8("port"));
        port->setEditable(true);

        horizontalLayout_3->addWidget(port);

        cmdRefresh = new QToolButton(grpConnection);
        cmdRefresh->setObjectName(QString::fromUtf8("cmdRefresh"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(cmdRefresh->sizePolicy().hasHeightForWidth());
        cmdRefresh->setSizePolicy(sizePolicy4);
        cmdRefresh->setStyleSheet(QString::fromUtf8("padding: 0 1 0 2;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/restart.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdRefresh->setIcon(icon);

        horizontalLayout_3->addWidget(cmdRefresh);

        label_2 = new QLabel(grpConnection);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        baud = new QComboBox(grpConnection);
        baud->addItem(QString());
        baud->addItem(QString());
        baud->addItem(QString());
        baud->addItem(QString());
        baud->addItem(QString());
        baud->addItem(QString());
        baud->setObjectName(QString::fromUtf8("baud"));
        baud->setEditable(true);

        horizontalLayout_3->addWidget(baud);


        verticalLayout->addLayout(horizontalLayout_3);

        groupBox_3 = new QGroupBox(grpConnection);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setEnabled(true);
        verticalLayout_10 = new QVBoxLayout(groupBox_3);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_34 = new QLabel(groupBox_3);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        horizontalLayout_6->addWidget(label_34);

        horizontalSpacer_7 = new QSpacerItem(6, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_7);

        cboUnitsCommon = new QComboBox(groupBox_3);
        cboUnitsCommon->addItem(QString());
        cboUnitsCommon->addItem(QString());
        cboUnitsCommon->setObjectName(QString::fromUtf8("cboUnitsCommon"));
        sizePolicy1.setHeightForWidth(cboUnitsCommon->sizePolicy().hasHeightForWidth());
        cboUnitsCommon->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(cboUnitsCommon);

        horizontalLayout_6->setStretch(1, 1);
        horizontalLayout_6->setStretch(2, 2);

        verticalLayout_10->addLayout(horizontalLayout_6);


        verticalLayout->addWidget(groupBox_3);


        gridLayout_20->addWidget(grpConnection, 0, 0, 1, 1);

        scrollArea_3->setWidget(scrollAreaWidgetContents_4);

        gridLayout_19->addWidget(scrollArea_3, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        gridLayout_14 = new QGridLayout(tab_5);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        scrollArea = new QScrollArea(tab_5);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy5);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 464, 501));
        gridLayout_3 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        grpPanels = new GroupBox(scrollAreaWidgetContents);
        grpPanels->setObjectName(QString::fromUtf8("grpPanels"));
        QSizePolicy sizePolicy6(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(grpPanels->sizePolicy().hasHeightForWidth());
        grpPanels->setSizePolicy(sizePolicy6);
        gridLayout_16 = new QGridLayout(grpPanels);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        panelHeightmap = new QCheckBox(grpPanels);
        panelHeightmap->setObjectName(QString::fromUtf8("panelHeightmap"));

        gridLayout_16->addWidget(panelHeightmap, 0, 0, 1, 1);

        panelFeed = new QCheckBox(grpPanels);
        panelFeed->setObjectName(QString::fromUtf8("panelFeed"));

        gridLayout_16->addWidget(panelFeed, 0, 1, 1, 1);

        panelJog = new QCheckBox(grpPanels);
        panelJog->setObjectName(QString::fromUtf8("panelJog"));

        gridLayout_16->addWidget(panelJog, 1, 1, 1, 1);

        panelSpindle = new QCheckBox(grpPanels);
        panelSpindle->setObjectName(QString::fromUtf8("panelSpindle"));

        gridLayout_16->addWidget(panelSpindle, 1, 0, 1, 1);


        verticalLayout_16->addWidget(grpPanels);

        grpColors = new GroupBox(scrollAreaWidgetContents);
        grpColors->setObjectName(QString::fromUtf8("grpColors"));
        verticalLayout_7 = new QVBoxLayout(grpColors);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        label_20 = new QLabel(grpColors);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout_6->addWidget(label_20, 7, 2, 1, 1);

        label_21 = new QLabel(grpColors);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setWordWrap(true);

        gridLayout_6->addWidget(label_21, 5, 0, 1, 1);

        toolpathZMovementColor = new ColorPicker(grpColors);
        toolpathZMovementColor->setObjectName(QString::fromUtf8("toolpathZMovementColor"));

        gridLayout_6->addWidget(toolpathZMovementColor, 7, 3, 1, 1);

        label_24 = new QLabel(grpColors);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout_6->addWidget(label_24, 2, 0, 1, 1);

        toolpathHighlightColor = new ColorPicker(grpColors);
        toolpathHighlightColor->setObjectName(QString::fromUtf8("toolpathHighlightColor"));

        gridLayout_6->addWidget(toolpathHighlightColor, 7, 1, 1, 1);

        toolpathStartColor = new ColorPicker(grpColors);
        toolpathStartColor->setObjectName(QString::fromUtf8("toolpathStartColor"));

        gridLayout_6->addWidget(toolpathStartColor, 8, 1, 1, 1);

        label_25 = new QLabel(grpColors);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout_6->addWidget(label_25, 7, 0, 1, 1);

        label_26 = new QLabel(grpColors);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_6->addWidget(label_26, 8, 0, 1, 1);

        toolpathEndColor = new ColorPicker(grpColors);
        toolpathEndColor->setObjectName(QString::fromUtf8("toolpathEndColor"));

        gridLayout_6->addWidget(toolpathEndColor, 8, 3, 1, 1);

        label_27 = new QLabel(grpColors);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        gridLayout_6->addWidget(label_27, 8, 2, 1, 1);

        visualizerBackgroundColor = new ColorPicker(grpColors);
        visualizerBackgroundColor->setObjectName(QString::fromUtf8("visualizerBackgroundColor"));

        gridLayout_6->addWidget(visualizerBackgroundColor, 2, 1, 1, 1);

        visualizerTextColor = new ColorPicker(grpColors);
        visualizerTextColor->setObjectName(QString::fromUtf8("visualizerTextColor"));

        gridLayout_6->addWidget(visualizerTextColor, 2, 3, 1, 1);

        label_22 = new QLabel(grpColors);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        gridLayout_6->addWidget(label_22, 1, 0, 1, 1);

        label_28 = new QLabel(grpColors);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        gridLayout_6->addWidget(label_28, 2, 2, 1, 1);

        label_19 = new QLabel(grpColors);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout_6->addWidget(label_19, 3, 0, 1, 1);

        toolColor = new ColorPicker(grpColors);
        toolColor->setObjectName(QString::fromUtf8("toolColor"));

        gridLayout_6->addWidget(toolColor, 3, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 6, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_6->addItem(verticalSpacer, 4, 0, 1, 1);

        label_16 = new QLabel(grpColors);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_6->addWidget(label_16, 6, 2, 1, 1);

        toolpathDrawnColor = new ColorPicker(grpColors);
        toolpathDrawnColor->setObjectName(QString::fromUtf8("toolpathDrawnColor"));

        gridLayout_6->addWidget(toolpathDrawnColor, 6, 3, 1, 1);

        toolpathNormalColor = new ColorPicker(grpColors);
        toolpathNormalColor->setObjectName(QString::fromUtf8("toolpathNormalColor"));

        gridLayout_6->addWidget(toolpathNormalColor, 6, 1, 1, 1);

        label_23 = new QLabel(grpColors);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout_6->addWidget(label_23, 6, 0, 1, 1);


        verticalLayout_7->addLayout(gridLayout_6);


        verticalLayout_16->addWidget(grpColors);

        grpConsole = new QGroupBox(scrollAreaWidgetContents);
        grpConsole->setObjectName(QString::fromUtf8("grpConsole"));
        QSizePolicy sizePolicy7(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(grpConsole->sizePolicy().hasHeightForWidth());
        grpConsole->setSizePolicy(sizePolicy7);
        grpConsole->setMinimumSize(QSize(0, 0));
        verticalLayout_12 = new QVBoxLayout(grpConsole);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        verticalLayout_12->setContentsMargins(9, 9, 9, 9);
        showProgramCommands = new QCheckBox(grpConsole);
        showProgramCommands->setObjectName(QString::fromUtf8("showProgramCommands"));

        verticalLayout_12->addWidget(showProgramCommands);

        autoCompletion = new QCheckBox(grpConsole);
        autoCompletion->setObjectName(QString::fromUtf8("autoCompletion"));

        verticalLayout_12->addWidget(autoCompletion);

        showUICommands = new QCheckBox(grpConsole);
        showUICommands->setObjectName(QString::fromUtf8("showUICommands"));

        verticalLayout_12->addWidget(showUICommands);


        verticalLayout_16->addWidget(grpConsole);

        groupBox = new GroupBox(scrollAreaWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_10 = new QHBoxLayout(groupBox);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_29 = new QLabel(groupBox);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        horizontalLayout_10->addWidget(label_29);

        cboFontSize = new QComboBox(groupBox);
        cboFontSize->addItem(QString());
        cboFontSize->addItem(QString());
        cboFontSize->addItem(QString());
        cboFontSize->addItem(QString());
        cboFontSize->addItem(QString());
        cboFontSize->setObjectName(QString::fromUtf8("cboFontSize"));
        QSizePolicy sizePolicy8(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(cboFontSize->sizePolicy().hasHeightForWidth());
        cboFontSize->setSizePolicy(sizePolicy8);
        cboFontSize->setEditable(true);
        cboFontSize->setMaxCount(10);
        cboFontSize->setMinimumContentsLength(1);

        horizontalLayout_10->addWidget(cboFontSize);

        label_30 = new QLabel(groupBox);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        horizontalLayout_10->addWidget(label_30);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_2);

        label_31 = new QLabel(groupBox);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setStyleSheet(QString::fromUtf8("color: blue;"));

        horizontalLayout_10->addWidget(label_31);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_3);


        verticalLayout_16->addWidget(groupBox);


        gridLayout_3->addLayout(verticalLayout_16, 0, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout_14->addWidget(scrollArea, 0, 0, 1, 1);

        tabWidget->addTab(tab_5, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_15 = new QGridLayout(tab_3);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        scrollArea_2 = new QScrollArea(tab_3);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 478, 436));
        gridLayout_18 = new QGridLayout(scrollAreaWidgetContents_3);
        gridLayout_18->setObjectName(QString::fromUtf8("gridLayout_18"));
        grpVisualizer = new GroupBox(scrollAreaWidgetContents_3);
        grpVisualizer->setObjectName(QString::fromUtf8("grpVisualizer"));
        verticalLayout_2 = new QVBoxLayout(grpVisualizer);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        antialiasing = new QCheckBox(grpVisualizer);
        antialiasing->setObjectName(QString::fromUtf8("antialiasing"));

        horizontalLayout_5->addWidget(antialiasing);


        gridLayout_5->addLayout(horizontalLayout_5, 0, 3, 1, 1);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_8 = new QLabel(grpVisualizer);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setEnabled(false);

        horizontalLayout_13->addWidget(label_8);


        gridLayout_5->addLayout(horizontalLayout_13, 2, 3, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(6, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_5, 0, 2, 1, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        vsync = new QCheckBox(grpVisualizer);
        vsync->setObjectName(QString::fromUtf8("vsync"));

        horizontalLayout_9->addWidget(vsync);


        gridLayout_5->addLayout(horizontalLayout_9, 1, 3, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_7 = new QLabel(grpVisualizer);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_7->addWidget(label_7);

        cboFps = new QComboBox(grpVisualizer);
        cboFps->addItem(QString());
        cboFps->addItem(QString());
        cboFps->addItem(QString());
        cboFps->setObjectName(QString::fromUtf8("cboFps"));
        cboFps->setInputMethodHints(Qt::ImhDigitsOnly|Qt::ImhFormattedNumbersOnly);
        cboFps->setEditable(true);

        horizontalLayout_7->addWidget(cboFps);


        gridLayout_5->addLayout(horizontalLayout_7, 1, 0, 1, 1);

        simplify = new QCheckBox(grpVisualizer);
        simplify->setObjectName(QString::fromUtf8("simplify"));

        gridLayout_5->addWidget(simplify, 2, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_5 = new QLabel(grpVisualizer);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_4->addWidget(label_5);

        lineWidth = new QDoubleSpinBox(grpVisualizer);
        lineWidth->setObjectName(QString::fromUtf8("lineWidth"));
        lineWidth->setAlignment(Qt::AlignCenter);
        lineWidth->setButtonSymbols(QAbstractSpinBox::NoButtons);
        lineWidth->setDecimals(1);
        lineWidth->setMaximum(9.000000000000000);

        horizontalLayout_4->addWidget(lineWidth);


        gridLayout_5->addLayout(horizontalLayout_4, 0, 0, 1, 1);

        msaa = new QCheckBox(grpVisualizer);
        msaa->setObjectName(QString::fromUtf8("msaa"));
        msaa->setEnabled(false);

        gridLayout_5->addWidget(msaa, 0, 4, 1, 1);

        zBuffer = new QCheckBox(grpVisualizer);
        zBuffer->setObjectName(QString::fromUtf8("zBuffer"));

        gridLayout_5->addWidget(zBuffer, 1, 4, 1, 1);

        simplifyPrecision = new QDoubleSpinBox(grpVisualizer);
        simplifyPrecision->setObjectName(QString::fromUtf8("simplifyPrecision"));
        simplifyPrecision->setEnabled(false);
        simplifyPrecision->setAlignment(Qt::AlignCenter);
        simplifyPrecision->setButtonSymbols(QAbstractSpinBox::NoButtons);
        simplifyPrecision->setDecimals(2);
        simplifyPrecision->setMaximum(99.000000000000000);

        gridLayout_5->addWidget(simplifyPrecision, 2, 4, 1, 1);


        verticalLayout_2->addLayout(gridLayout_5);


        gridLayout_18->addWidget(grpVisualizer, 0, 0, 1, 1);

        grpTool = new GroupBox(scrollAreaWidgetContents_3);
        grpTool->setObjectName(QString::fromUtf8("grpTool"));
        gridLayout = new QGridLayout(grpTool);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        toolAngle = new QDoubleSpinBox(grpTool);
        toolAngle->setObjectName(QString::fromUtf8("toolAngle"));
        sizePolicy1.setHeightForWidth(toolAngle->sizePolicy().hasHeightForWidth());
        toolAngle->setSizePolicy(sizePolicy1);
        toolAngle->setAlignment(Qt::AlignCenter);
        toolAngle->setButtonSymbols(QAbstractSpinBox::NoButtons);
        toolAngle->setDecimals(3);
        toolAngle->setMaximum(180.000000000000000);

        gridLayout->addWidget(toolAngle, 0, 5, 1, 1);

        toolLength = new QDoubleSpinBox(grpTool);
        toolLength->setObjectName(QString::fromUtf8("toolLength"));
        sizePolicy1.setHeightForWidth(toolLength->sizePolicy().hasHeightForWidth());
        toolLength->setSizePolicy(sizePolicy1);
        toolLength->setAlignment(Qt::AlignCenter);
        toolLength->setButtonSymbols(QAbstractSpinBox::NoButtons);
        toolLength->setDecimals(3);
        toolLength->setMaximum(999.000000000000000);

        gridLayout->addWidget(toolLength, 1, 5, 1, 1);

        label_3 = new QLabel(grpTool);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        lblToolLength = new QLabel(grpTool);
        lblToolLength->setObjectName(QString::fromUtf8("lblToolLength"));

        gridLayout->addWidget(lblToolLength, 1, 4, 1, 1);

        toolDiameter = new QDoubleSpinBox(grpTool);
        toolDiameter->setObjectName(QString::fromUtf8("toolDiameter"));
        sizePolicy1.setHeightForWidth(toolDiameter->sizePolicy().hasHeightForWidth());
        toolDiameter->setSizePolicy(sizePolicy1);
        toolDiameter->setAlignment(Qt::AlignCenter);
        toolDiameter->setButtonSymbols(QAbstractSpinBox::NoButtons);
        toolDiameter->setDecimals(3);
        toolDiameter->setMaximum(99.000000000000000);

        gridLayout->addWidget(toolDiameter, 1, 1, 1, 1);

        label_4 = new QLabel(grpTool);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        lblToolAngle = new QLabel(grpTool);
        lblToolAngle->setObjectName(QString::fromUtf8("lblToolAngle"));
        lblToolAngle->setEnabled(false);

        gridLayout->addWidget(lblToolAngle, 0, 4, 1, 1);

        cboToolType = new QComboBox(grpTool);
        cboToolType->addItem(QString());
        cboToolType->addItem(QString());
        cboToolType->setObjectName(QString::fromUtf8("cboToolType"));
        sizePolicy1.setHeightForWidth(cboToolType->sizePolicy().hasHeightForWidth());
        cboToolType->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(cboToolType, 0, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(6, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 0, 2, 1, 1);


        gridLayout_18->addWidget(grpTool, 2, 0, 1, 1);

        scrollArea_2->setWidget(scrollAreaWidgetContents_3);

        gridLayout_15->addWidget(scrollArea_2, 0, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_10 = new QGridLayout(tab_2);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        scrollArea_4 = new QScrollArea(tab_2);
        scrollArea_4->setObjectName(QString::fromUtf8("scrollArea_4"));
        scrollArea_4->setWidgetResizable(true);
        scrollAreaWidgetContents_5 = new QWidget();
        scrollAreaWidgetContents_5->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_5"));
        scrollAreaWidgetContents_5->setGeometry(QRect(0, 0, 478, 436));
        gridLayout_21 = new QGridLayout(scrollAreaWidgetContents_5);
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        pcb2gcodeArgs = new QPlainTextEdit(scrollAreaWidgetContents_5);
        pcb2gcodeArgs->setObjectName(QString::fromUtf8("pcb2gcodeArgs"));

        gridLayout_21->addWidget(pcb2gcodeArgs, 1, 0, 1, 1);

        label_40 = new QLabel(scrollAreaWidgetContents_5);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        label_40->setFont(font1);

        gridLayout_21->addWidget(label_40, 0, 0, 1, 1);

        scrollArea_4->setWidget(scrollAreaWidgetContents_5);

        gridLayout_10->addWidget(scrollArea_4, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());

        gridLayout_13->addWidget(tabWidget, 0, 0, 1, 1);

        QWidget::setTabOrder(port, cmdRefresh);
        QWidget::setTabOrder(cmdRefresh, baud);
        QWidget::setTabOrder(baud, cboToolType);
        QWidget::setTabOrder(cboToolType, toolAngle);
        QWidget::setTabOrder(toolAngle, toolDiameter);
        QWidget::setTabOrder(toolDiameter, toolLength);
        QWidget::setTabOrder(toolLength, cmdDefaults);
        QWidget::setTabOrder(cmdDefaults, cmdOK);
        QWidget::setTabOrder(cmdOK, cmdCancel);

        retranslateUi(frmSettings);
        QObject::connect(moveOnRestore, SIGNAL(toggled(bool)), cboRestoreMode, SLOT(setEnabled(bool)));
        QObject::connect(vsync, SIGNAL(toggled(bool)), label_7, SLOT(setDisabled(bool)));
        QObject::connect(antialiasing, SIGNAL(toggled(bool)), msaa, SLOT(setEnabled(bool)));
        QObject::connect(arcDegreeMode, SIGNAL(toggled(bool)), arcDegree, SLOT(setEnabled(bool)));
        QObject::connect(vsync, SIGNAL(toggled(bool)), cboFps, SLOT(setDisabled(bool)));
        QObject::connect(simplify, SIGNAL(toggled(bool)), simplifyPrecision, SLOT(setEnabled(bool)));
        QObject::connect(simplify, SIGNAL(toggled(bool)), label_8, SLOT(setEnabled(bool)));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(frmSettings);
    } // setupUi

    void retranslateUi(QDialog *frmSettings)
    {
        frmSettings->setWindowTitle(QApplication::translate("frmSettings", "Settings", nullptr));
        cmdDefaults->setText(QApplication::translate("frmSettings", "Set to defaults", nullptr));
        cmdOK->setText(QApplication::translate("frmSettings", "OK", nullptr));
        cmdCancel->setText(QApplication::translate("frmSettings", "Cancel", nullptr));
        grpGRBL->setTitle(QApplication::translate("frmSettings", "Machine information", nullptr));
        cboUnits->setItemText(0, QApplication::translate("frmSettings", "Metric", nullptr));
        cboUnits->setItemText(1, QApplication::translate("frmSettings", "Imperial", nullptr));

        label_10->setText(QApplication::translate("frmSettings", "Rapid speed:", nullptr));
        label_14->setText(QApplication::translate("frmSettings", "Acceleration:", nullptr));
        label_11->setText(QApplication::translate("frmSettings", "Status query period:", nullptr));
        label_15->setText(QApplication::translate("frmSettings", "Units:", nullptr));
        grpParser->setTitle(QApplication::translate("frmSettings", "Parser", nullptr));
        label_32->setText(QApplication::translate("frmSettings", "Arc approximation segment size:", nullptr));
        radArcLengthMode->setText(QApplication::translate("frmSettings", "by length", nullptr));
        arcDegreeMode->setText(QApplication::translate("frmSettings", "by angle (deg.)", nullptr));
        groupBox_2->setTitle(QApplication::translate("frmSettings", "Control", nullptr));
        label_17->setText(QApplication::translate("frmSettings", "Probe commands:", nullptr));
        touchCommand->setPlaceholderText(QApplication::translate("frmSettings", "Command 1; command 2; ...", nullptr));
        label_33->setText(QApplication::translate("frmSettings", "Safe position commands:", nullptr));
        safeCommand->setPlaceholderText(QApplication::translate("frmSettings", "Command 1; command 2; ...", nullptr));
        moveOnRestore->setText(QApplication::translate("frmSettings", "\"Restore origin\" moves tool in:", nullptr));
        cboRestoreMode->setItemText(0, QApplication::translate("frmSettings", "Plane", nullptr));
        cboRestoreMode->setItemText(1, QApplication::translate("frmSettings", "Space", nullptr));

        label_13->setText(QApplication::translate("frmSettings", "max.:", nullptr));
        label_12->setText(QApplication::translate("frmSettings", "Spindle speed min.:", nullptr));
        restoreZeroAfterHoming->setText(QApplication::translate("frmSettings", "Restore zero XY after homing", nullptr));
        groupBox_4->setTitle(QApplication::translate("frmSettings", "Heightmap", nullptr));
        label_38->setText(QApplication::translate("frmSettings", "Heightmap probing feed:", nullptr));
        checkedHeightMap->setText(QApplication::translate("frmSettings", "Create multiple height map and use statistical analysis", nullptr));
        grpConnection->setTitle(QApplication::translate("frmSettings", "Connection", nullptr));
        label->setText(QApplication::translate("frmSettings", "Port:", nullptr));
        cmdRefresh->setText(QApplication::translate("frmSettings", "...", nullptr));
        label_2->setText(QApplication::translate("frmSettings", "Baud:", nullptr));
        baud->setItemText(0, QApplication::translate("frmSettings", "9600", nullptr));
        baud->setItemText(1, QApplication::translate("frmSettings", "14400", nullptr));
        baud->setItemText(2, QApplication::translate("frmSettings", "19200", nullptr));
        baud->setItemText(3, QApplication::translate("frmSettings", "38400", nullptr));
        baud->setItemText(4, QApplication::translate("frmSettings", "57600", nullptr));
        baud->setItemText(5, QApplication::translate("frmSettings", "115200", nullptr));

        baud->setCurrentText(QApplication::translate("frmSettings", "9600", nullptr));
        groupBox_3->setTitle(QApplication::translate("frmSettings", "Common", nullptr));
        label_34->setText(QApplication::translate("frmSettings", "Application measurement units:", nullptr));
        cboUnitsCommon->setItemText(0, QApplication::translate("frmSettings", "Metric", nullptr));
        cboUnitsCommon->setItemText(1, QApplication::translate("frmSettings", "Imperial", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("frmSettings", "Machine", nullptr));
        grpPanels->setTitle(QApplication::translate("frmSettings", "Panels", nullptr));
        panelHeightmap->setText(QApplication::translate("frmSettings", "Heightmap", nullptr));
        panelFeed->setText(QApplication::translate("frmSettings", "Feed", nullptr));
        panelJog->setText(QApplication::translate("frmSettings", "Jog", nullptr));
        panelSpindle->setText(QApplication::translate("frmSettings", "Spindle", nullptr));
        grpColors->setTitle(QApplication::translate("frmSettings", "Colors", nullptr));
        label_20->setText(QApplication::translate("frmSettings", "Z-movement:", nullptr));
        label_21->setText(QApplication::translate("frmSettings", "Toolpath", nullptr));
        label_24->setText(QApplication::translate("frmSettings", "Background:", nullptr));
        label_25->setText(QApplication::translate("frmSettings", "Highlight:", nullptr));
        label_26->setText(QApplication::translate("frmSettings", "Start point:", nullptr));
        label_27->setText(QApplication::translate("frmSettings", "End point:", nullptr));
        label_22->setText(QApplication::translate("frmSettings", "Visualizer", nullptr));
        label_28->setText(QApplication::translate("frmSettings", "Text:", nullptr));
        label_19->setText(QApplication::translate("frmSettings", "Tool:", nullptr));
        label_16->setText(QApplication::translate("frmSettings", "Drawn:", nullptr));
        label_23->setText(QApplication::translate("frmSettings", "Normal:", nullptr));
        grpConsole->setTitle(QApplication::translate("frmSettings", "Console", nullptr));
        showProgramCommands->setText(QApplication::translate("frmSettings", "Show G-code program commands", nullptr));
        autoCompletion->setText(QApplication::translate("frmSettings", "Command auto-completion", nullptr));
        showUICommands->setText(QApplication::translate("frmSettings", "Show UI commands", nullptr));
        groupBox->setTitle(QApplication::translate("frmSettings", "Font", nullptr));
        label_29->setText(QApplication::translate("frmSettings", "Size:", nullptr));
        cboFontSize->setItemText(0, QApplication::translate("frmSettings", "8", nullptr));
        cboFontSize->setItemText(1, QApplication::translate("frmSettings", "9", nullptr));
        cboFontSize->setItemText(2, QApplication::translate("frmSettings", "10", nullptr));
        cboFontSize->setItemText(3, QApplication::translate("frmSettings", "11", nullptr));
        cboFontSize->setItemText(4, QApplication::translate("frmSettings", "12", nullptr));

        cboFontSize->setCurrentText(QApplication::translate("frmSettings", "8", nullptr));
        label_30->setText(QApplication::translate("frmSettings", "pt", nullptr));
        label_31->setText(QApplication::translate("frmSettings", "(restart needed)", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("frmSettings", "UI", nullptr));
        grpVisualizer->setTitle(QApplication::translate("frmSettings", "Visualizer", nullptr));
        antialiasing->setText(QApplication::translate("frmSettings", "Smoothing", nullptr));
        label_8->setText(QApplication::translate("frmSettings", "Precision:", nullptr));
        vsync->setText(QApplication::translate("frmSettings", "V-sync", nullptr));
        label_7->setText(QApplication::translate("frmSettings", "FPS lock:", nullptr));
        cboFps->setItemText(0, QApplication::translate("frmSettings", "30", nullptr));
        cboFps->setItemText(1, QApplication::translate("frmSettings", "60", nullptr));
        cboFps->setItemText(2, QApplication::translate("frmSettings", "120", nullptr));

        simplify->setText(QApplication::translate("frmSettings", "Simplify geometry", nullptr));
        label_5->setText(QApplication::translate("frmSettings", "Line width:", nullptr));
        msaa->setText(QApplication::translate("frmSettings", "MSAA", nullptr));
        zBuffer->setText(QApplication::translate("frmSettings", "Z-buffer", nullptr));
        grpTool->setTitle(QApplication::translate("frmSettings", "Tool model", nullptr));
        label_3->setText(QApplication::translate("frmSettings", "Type:", nullptr));
        lblToolLength->setText(QApplication::translate("frmSettings", "Length:", nullptr));
        label_4->setText(QApplication::translate("frmSettings", "Diameter:", nullptr));
        lblToolAngle->setText(QApplication::translate("frmSettings", "Angle:", nullptr));
        cboToolType->setItemText(0, QApplication::translate("frmSettings", "Flat", nullptr));
        cboToolType->setItemText(1, QApplication::translate("frmSettings", "Conic", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("frmSettings", "3D", nullptr));
        pcb2gcodeArgs->setDocumentTitle(QApplication::translate("frmSettings", "pcb2gcode arguments", nullptr));
        label_40->setText(QApplication::translate("frmSettings", "pcb2gcode arguments", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("frmSettings", "PCB", nullptr));
    } // retranslateUi

};

namespace Ui {
    class frmSettings: public Ui_frmSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRMSETTINGS_H
