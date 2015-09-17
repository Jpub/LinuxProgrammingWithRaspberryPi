#include <QApplication>
#include <QComboBox>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QComboBox *cb = new QComboBox();
    cb->addItem("KDE");
    cb->addItem("GNOME");
    cb->addItem("FVWM");
    cb->addItem("CDE");
    cb->setEditable(true); 
    cb->show();

    return app.exec();
}

