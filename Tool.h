#ifndef TOOL_H
#define TOOL_H

#include <QLabel>
#include <QWidget>

class Tool : public QLabel {
    Q_OBJECT

public:
    explicit Tool(int type,QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~Tool();
    void setType(int type);

private:
    int type; // 1,2,3
signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);

};

#endif // TOOTH_H
