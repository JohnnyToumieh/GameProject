#ifndef TOOTH_H
#define TOOTH_H

#include <QLabel>
#include <QWidget>

class Tooth : public QLabel {
    Q_OBJECT

public:
    explicit Tooth(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~Tooth();
    void setType(int type);

private:
    int type; // white,yellow,blue and black
signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);
    void updateType();

};

#endif // TOOTH_H
