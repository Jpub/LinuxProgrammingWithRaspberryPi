#include <QApplication>
#include <QSpinBox>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QSpinBox *sp = new QSpinBox();
    sp->setRange(0, 10);
    sp->setValue(5);
    sp->setWrapping(true);    
    sp->setSpecialValueText("Value");
    sp->show();

    return app.exec();
}

