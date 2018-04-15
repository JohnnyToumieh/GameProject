#include "Tool.h"

Tool::Tool(int type, QWidget* parent, Qt::WindowFlags f): QLabel(parent) {
    this->type=type;

   if(this->type==1){
       imageName = ":tool1";
       setStyleSheet("QLabel { background-color : white;}");
   }else if(this->type==2){
       imageName = ":tool2";
       setStyleSheet("QLabel { background-color : white;}");
   }else if(this->type==3){
       imageName = ":tool3";
       setStyleSheet("QLabel { background-color : white;}");
   }
    setPixmap(QPixmap(imageName).scaled(70,70));
}

Tool::~Tool() {}

void Tool::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}
