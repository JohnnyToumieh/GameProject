#include "Tooth.h"

Tooth::Tooth(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

}

Tooth::~Tooth() {}

void Tooth::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}
