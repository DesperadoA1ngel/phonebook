#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char* argv[])
{
    // Устанавливаем высокое DPI 
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QApplication app(argc, argv);

    // Устанавливаем стиль приложения
    app.setStyle(QStyleFactory::create("Fusion"));


    app.setApplicationName("PhoneBook");
    app.setOrganizationName("MyCompany");
    app.setApplicationVersion("1.0.0");

    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    MainWindow window;
    window.show();

    return app.exec();
}