#ifndef USER_H
#define USER_H

#include <QJsonObject>
#include <QJsonArray>

class User
{
public:
    User();

    bool read(const QJsonObject &json);
    bool write(QJsonObject &json) const;

    QString username;
    QString password;

    QString firstName;
    QString lastName;
    QString email;
    QString picture;
    //int age; how to store/retrieve an int from json?
    //qint16 gender; what type is it?
    QString DoBday;
    QString DoBmonth;
    QString DoByear;

private:
    void writeHelper(QJsonObject &json) const;
};

#endif // USER_H
