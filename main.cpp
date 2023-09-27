#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

static void fixupFont();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "RoboMasterUIDesigner_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    fixupFont();
    MainWindow w;
    w.show();
    return a.exec();
}

static void fixupFont()
{
    foreach (auto i, QLocale::system().uiLanguages()) {
        // Workaround for Qt5 font under Chinese systems
        if (i.startsWith("zh")) {
            auto font = qApp->font();
            qApp->setFont(QFont("Microsoft YaHei UI", font.pointSize(), font.weight()));
            return;
        }
    }
}
