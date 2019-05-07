#include "clickablelabel.h"

/*! \file clickablelabel.cpp
    \brief Plik źródłowy zawierający klasę ClickableLabel, odpowiadający za obsługę klikania na pola.
*/

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
}
