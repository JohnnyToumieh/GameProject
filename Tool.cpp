#include "Tool.h"

Tool::Tool(int type,QWidget* parent, Qt::WindowFlags f) : QLabel(parent) {
    this->type=type;

   if(this->type==1){
       setStyleSheet("QLabel { background-color : black; }");
   }else if(this->type==2){
       setStyleSheet("QLabel { background-color : yellow; }");
   }else if(this->type==3){
       setStyleSheet("QLabel { background-color : blue; }");
   }

}

Tool::~Tool() {}

void Tool::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}
