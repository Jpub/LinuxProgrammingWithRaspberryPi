#include <QApplication>
#include <QTextBrowser>
#include <QUrl>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QUrl url("index.html");

    QTextBrowser *tb = new QTextBrowser();
    tb->setSource(url);
    tb->show();

    return app.exec();
}

