#ifndef USER_H
#define USER_H

#include <QJsonObject>
#include <QJsonArray>

/**
 *\file User.h
 *@brief The User class, represents spongebob
 *
 */

class User
{
public:
    User();

    bool read(const QJsonObject &json);//!<Member function reads users info
    bool write(QJsonObject &json) const;//!<Member function writes users info

    void clear();//!<Member function clear users info

    QString username;
    QString password;

    QString firstName;
    QString lastName;
    QString email;
    QString gender;

    int DoBday;
    int DoBmonth;
    int DoByear;

    bool isGuest;

private:
    void writeHelper(QJsonObject &json) const;
};

#endif // USER_H
