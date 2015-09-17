#include <QApplication>

#include "CustomWidget.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    CustomWidget *widget = new CustomWidget(0);
    QObject::connect(widget, SIGNAL(widgetClicked( )), &app, SLOT(quit( )));
    widget->show();

    return app.exec();
}

