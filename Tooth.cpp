#include "Tooth.h"

Tooth::Tooth(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {
    this->type = 1;
    setStyleSheet("QLabel { background-color : white; }");
}

Tooth::~Tooth() {}

void Tooth::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}

void Tooth::setType(int type){
    this->type=type;
    updateType();
}

void Tooth::updateType(){
    if(type==1){
        setStyleSheet("QLabel { background-color : white; }");
    }else if(type==2){
        setStyleSheet("QLabel { background-color : black; }");
    }else if(type==3){
        setStyleSheet("QLabel { background-color : yellow; }");
    }else if(type==4){
        setStyleSheet("QLabel { background-color : blue; }");
    }else if(type==5){
        setStyleSheet("QLabel { background-color : green; }");
    }else if(type==6){
        setStyleSheet("QLabel { background-color : red; }");
    }
}
