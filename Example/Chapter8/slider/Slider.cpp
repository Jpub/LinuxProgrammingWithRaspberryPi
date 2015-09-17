#include <QApplication>
#include <QSlider>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QSlider *sl = new QSlider(Qt::Horizontal, 0);
    sl->setTickPosition(QSlider::TicksAbove);
    sl->setRange(0, 100);
    sl->setValue(50);
    sl->show();

    return app.exec();
}

