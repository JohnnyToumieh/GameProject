#include "Message.h"
/**
 *\file Message.cpp
 *@brief contains Message class definition which represents the page where user chooses game
 *
 * Message class offers a generic message object that given any message as a string
 * it will display ot for the user as a pop window.
 *
 */

/**
 * @brief Message::Message constructor of Message class
 */
Message::Message()
{

}

/**
 * @brief Message::Message constructor of Message class
 *
 * A constructor that set up the message and display it in w new window (widget)
 * @param QString m, the message to display
 */
Message::Message(QString m)
{
    msg = new QLabel(m);
    VerticalL = new QVBoxLayout();
    VerticalL->addWidget(msg);
    setLayout(VerticalL);
    setFixedSize(350,100);
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size(),
            qApp->desktop()->availableGeometry()
        ));


}
