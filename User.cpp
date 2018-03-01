#include "User.h"

User::User()
{
    this->isGuest = false;
}

bool User::read(const QJsonObject &json)
{
    if (json.contains("users") && json["users"].isArray()) {
        QJsonArray userArray = json["users"].toArray();
        for (int userIndex = 0; userIndex < userArray.size(); userIndex++) {
            QJsonObject userObject = userArray[userIndex].toObject();
            if (userObject.contains("username") && userObject["username"].isString() && userObject["username"] == this->username
                    && userObject.contains("password") && userObject["password"].isString() && userObject["password"] == this->password) {
                if (userObject.contains("firstName") && userObject["firstName"].isString()) {
                    this->firstName = userObject["firstName"].toString();
                }
                if (userObject.contains("lastName") && userObject["lastName"].isString()) {
                    this->lastName = userObject["lastName"].toString();
                }
                if (userObject.contains("email") && userObject["email"].isString()) {
                    this->email = userObject["email"].toString();
                }
                if (userObject.contains("age") && userObject["age"].isString()) {
                    this->age = userObject["age"].toString();
                }
                if (userObject.contains("gender") && userObject["gender"].isString()) {
                    this->gender = userObject["gender"].toString();
                }
                if (userObject.contains("DoBday") && userObject["DoBday"].isString()) {
                    this->DoBday = userObject["DoBday"].toString();
                }
                if (userObject.contains("DoBmonth") && userObject["DoBmonth"].isString()) {
                    this->DoBmonth = userObject["DoBmonth"].toString();
                }
                if (userObject.contains("DoByear") && userObject["DoByear"].isString()) {
                    this->DoByear = userObject["DoByear"].toString();
                }
                return true;
            }
        }
    }
    return false;
}

bool User::write(QJsonObject &json) const
{
    if (json.contains("users") && json["users"].isArray()) {
        QJsonArray userArray = json["users"].toArray();

        // If user has already been created, update fields
        for (int userIndex = 0; userIndex < userArray.size(); userIndex++) {
            QJsonObject userObject = userArray[userIndex].toObject();
            if (userObject.contains("username") && userObject["username"].isString() && userObject["username"] == this->username) {
                User::writeHelper(userObject);
                userArray[userIndex] = userObject;
                json["users"] = userArray;
                return true;
            }
        }

        // If user has never been created before, create him
        QJsonObject userObject;
        User::writeHelper(userObject);
        userArray.append(userObject);
        json["users"] = userArray;
        return true;
    } else {
        // If no users have ever been created before, create new list and create the user
        QJsonArray userArray;
        QJsonObject userObject;
        User::writeHelper(userObject);
        userArray.append(userObject);
        json["users"] = userArray;
        return true;
    }
    return false;
}

void User::clear() {
    this->username = QString();
    this->password = QString();

    this->firstName = QString();
    this->lastName = QString();
    this->email = QString();
    this->age = QString();
    this->gender = QString();
    this->DoBday = QString();
    this->DoBmonth = QString();
    this->DoByear = QString();
}

void User::writeHelper(QJsonObject &userObject) const
{
    userObject["username"] = this->username;
    userObject["password"] = this->password;

    userObject["firstName"] = this->firstName;
    userObject["lastName"] = this->lastName;
    userObject["email"] = this->email;
    userObject["age"] = this->age;
    userObject["gender"] = this->gender;
    userObject["DoBday"] = this->DoBday;
    userObject["DoBmonth"] = this->DoBmonth;
    userObject["DoByear"] = this->DoByear;
}
