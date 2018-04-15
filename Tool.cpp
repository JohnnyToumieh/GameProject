#include "Tool.h"
/**
 *\file Tool.cpp
 *@brief The Tool.cpp class, represents the tools used by the dentist
 *
 * This clas holds three tools that are used in game 2 level 3 where the dentist (player) will have to
 * use each specific tool for the specific problem within the tooth.
 *
 */

/**
 * @brief Tool::Tool constructor of Tool class
 *
 * A constructor that set the Tool along with its attributes
 * it create one of the tools based on passed type integere
 *
 * @param type of the tool to create
 */
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

/**
 * @brief Tool::~Tool destructor of Item class
 */
Tool::~Tool() {}

/**
 * @brief Tool::mousePressEvent
 *
 * function that responed to clicking tool
 * @param QKeyEvent *event
 */
void Tool::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}
