/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QScreen>
#include <QStyle>
#include "Utility/LogUtil.h"
#include "Utility/DumpUtil.h"
#include "Utility/ImPath.h"
#include "settingmanager.h"

CLogUtil* g_dllLog = nullptr;

QtMessageHandler originalHandler = nullptr;

void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (g_dllLog)
    {
        ELogLevel logLevel = ELogLevel::LOG_LEVEL_ERROR;
        if (type == QtMsgType::QtDebugMsg)
        {
            logLevel = ELogLevel::LOG_LEVEL_DEBUG;
        }
        else if (type == QtMsgType::QtInfoMsg || type == QtMsgType::QtWarningMsg)
        {
            logLevel = ELogLevel::LOG_LEVEL_INFO;
        }
        g_dllLog->Log(context.file? context.file: "", context.line, logLevel, msg.toStdWString().c_str());
    }

    if (originalHandler)
    {
        (*originalHandler)(type, context, msg);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("niwenplay");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    qint64 processId = app.applicationPid();
    g_dllLog = CLogUtil::GetLog((std::wstring(L"main_")+std::to_wstring(processId)).c_str());

    // 初始化崩溃转储机制
    CDumpUtil::SetDumpFilePath(CImPath::GetDumpPath().c_str());
    CDumpUtil::Enable(true);

    originalHandler = qInstallMessageHandler(logToFile);

    // 打印完整的命令行参数
    QStringList arguments = app.arguments();
    QString fullCommandLine = arguments.join(" ");
    qInfo() << "Full Command Line: " << fullCommandLine;

    // 解析命令行参数
    QCommandLineParser parser;
    QCommandLineOption titleOption("title", "window title", "title");
    parser.addOption(titleOption);
    QCommandLineOption urlOption("url", "url", "url");
    parser.addOption(urlOption);
    QCommandLineOption jsPathOption("jspath", "jspath", "jspath");
    parser.addOption(jsPathOption);
    QCommandLineOption widthOption("width", "width", "width");
    parser.addOption(widthOption);
    QCommandLineOption heightOption("height", "height", "height");
    parser.addOption(heightOption);
    parser.process(app);

    SettingManager::GetInstance()->m_title = parser.value(titleOption);
    if (SettingManager::GetInstance()->m_title.isEmpty())
    {
        qCritical("need command line param");
        return 1;
    }
    SettingManager::GetInstance()->m_url = parser.value(urlOption);
    SettingManager::GetInstance()->m_jsPath = parser.value(jsPathOption);
    SettingManager::GetInstance()->m_windowWidth = parser.value(widthOption).toInt();
    SettingManager::GetInstance()->m_windowHeight = parser.value(heightOption).toInt();

    MainWindow mainWindow;
    mainWindow.setWindowTitle(SettingManager::GetInstance()->m_title);
    int titleBarHeight = app.style()->pixelMetric(QStyle::PM_TitleBarHeight);
    mainWindow.resize(SettingManager::GetInstance()->m_windowWidth,
                      SettingManager::GetInstance()->m_windowHeight-titleBarHeight);

    // 居中显示
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - mainWindow.width()) / 2;
    int y = (screenGeometry.height() - mainWindow.height()) / 2;
    mainWindow.move(x, y);
    mainWindow.show();

    return app.exec();
}
