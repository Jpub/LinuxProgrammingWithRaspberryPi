#include <QApplication>
#include <QLabel>
	
int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QLabel *hello = new QLabel("<font color=blue>Hello <i>World!</i></font>", 0);
    hello->show();

    return app.exec();
}

