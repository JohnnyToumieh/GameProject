#ifndef TOOTH_H
#define TOOTH_H

#include <QLabel>
#include <QWidget>

/**
 *\file Tooth.h
 *@brief The Tooth.h class, represents the teeth objects to be clicked
 *
 *
 */

class Tooth : public QLabel {
    Q_OBJECT

public:
    explicit Tooth(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~Tooth();
    void setType(int type);//!<Member function sets the type of the teeth

private:
    int type;//!<Int member that represent the type of the tooth
signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);
    void updateType();

};

#endif // TOOTH_H
