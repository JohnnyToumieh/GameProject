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

private:
    void writeHelper(QJsonObject &json) const;
};

#endif // USER_H
