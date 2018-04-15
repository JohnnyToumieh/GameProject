#include "Tooth.h"
/**
 *\file Tooth.cpp
 *@brief The Tooth class, represents the teeth objects to be clicked
 *
 *
 */

/**
 * @brief Tooth::Tooth constructor of Tooth class
 *
 * A constructor that set the Tooth along with its attributes
 * it create a tooth intially white
 *
 * @param type of the tool to create
 */
Tooth::Tooth(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {
    this->type = 1;
    setStyleSheet("QLabel { background-color : white; }");
}

/**
 * @brief Tooth::~Tooth destructor of Item class
 */
Tooth::~Tooth() {}

/**
 * @brief Tooth::mousePressEvent
 *
 * function that responed to clicking tool
 * @param QKeyEvent *event
 */
void Tooth::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}

/**
 * @brief Tooth::setType
 *
 * function that sets the member type and call updateType function
 * @param int type
 */
void Tooth::setType(int type){
    this->type=type;
    updateType();
}

/**
 * @brief Tooth::updateType
 *
 * function that based on type change the tooth color
 */
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
