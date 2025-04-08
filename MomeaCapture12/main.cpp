#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <Windows.h>
#include <minwindef.h>
#include <QSharedPointer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Teste_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // QSharedPointer<DataBuffer> buffer(new DataBuffer());
    // MainWindow w(buffer.data());

    MainWindow w;


    //QObject::connect(&w, &MainWindow::stopThread, &a, &QCoreApplication::quit);

    w.show();
    return a.exec();
}
