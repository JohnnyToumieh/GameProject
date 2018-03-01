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

    void clear();

    QString username;
    QString password;

    QString firstName;
    QString lastName;
    QString email;
    QString age;
    QString gender;
    QString DoBday;
    QString DoBmonth;
    QString DoByear;

    bool isGuest;

private:
    void writeHelper(QJsonObject &json) const;
};

#endif // USER_H
