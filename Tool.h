#ifndef TOOL_H
#define TOOL_H

#include <QLabel>
#include <QWidget>

/**
 *\file Tool.h
 *@brief The Tool.h class, represents the tools used by the dentist
 *
 * This clas holds three tools that are used in game 2 level 3 where the dentist (player) will have to
 * use each specific tool for the specific problem within the tooth.
 *
 */

class Tool : public QLabel {
    Q_OBJECT

public:
    explicit Tool(int type,QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~Tool();
    void setType(int type);//!<Member function sets the type of the tool
    QString imageName;
private:
    int type; //!<Int member that represent the type of the tools
signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);

};

#endif // TOOTH_H
