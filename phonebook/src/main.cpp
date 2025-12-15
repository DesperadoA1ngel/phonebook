#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Устанавливаем стиль приложения
    app.setStyle(QStyleFactory::create("Fusion"));

    // Устанавливаем информацию о приложении
    app.setApplicationName("PhoneBook");
    app.setOrganizationName("MyCompany");
    app.setApplicationVersion("1.0.0");

    MainWindow window;
    window.show();

    return app.exec();
}