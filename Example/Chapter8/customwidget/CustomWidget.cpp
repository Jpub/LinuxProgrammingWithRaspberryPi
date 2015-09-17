#include <QApplication>
#include <QPushButton>

#include "CustomWidget.h"
  
CustomWidget::CustomWidget(QWidget* parent) : QWidget(parent)
{
    QPushButton *button = new QPushButton("Quit", this); 
    button->resize(120, 35); 

    this->resize(120, 35); 
    move(300, 300);

    connect(button, SIGNAL(clicked( )), SLOT(processClick( )));
}

void CustomWidget:: processClick( )
{
    emit widgetClicked( );
}

