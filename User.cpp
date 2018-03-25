#include "User.h"
/**
 *\file User.cpp
 *@brief contains User class definition which represents the user that will be playing the games
 *
 */

/**
 * @brief User::User constructor of User class
 *
 */
User::User()
{
    this->isGuest = false;
}

/**
 * @brief User::read member function: read the user's info from JSON file
 *
 * function that take a QJsonObject reference access it to read user's basic info.
 * @param QJsonObject &json reference to the JSON file holding users info
 * @return bool true if the read is successfull
 */
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
                if (userObject.contains("gender") && userObject["gender"].isString()) {
                    this->gender = userObject["gender"].toString();
                }
                if (userObject.contains("DoBday") && userObject["DoBday"].isString()) {
                    this->DoBday = userObject["DoBday"].toInt();
                }
                if (userObject.contains("DoBmonth") && userObject["DoBmonth"].isString()) {
                    this->DoBmonth = userObject["DoBmonth"].toInt();
                }
                if (userObject.contains("DoByear") && userObject["DoByear"].isString()) {
                    this->DoByear = userObject["DoByear"].toInt();
                }
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief User::write member function: write the user's info to a JSON file
 *
 * function that take a QJsonObject reference access it to write user's basic info.
 * @param QJsonObject &json reference to the JSON file holding users info
 * @return bool true if the write is successfull
 */
bool User::write(QJsonObject &json) const
{
    QJsonArray userArray;
    if (json.contains("users") && json["users"].isArray()) {
        userArray = json["users"].toArray();
    }

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
}

/**
 * @brief User::clear member function: clear the info of the object user
 */
void User::clear() {
    this->username = QString();
    this->password = QString();

    this->firstName = QString();
    this->lastName = QString();
    this->email = QString();
    this->gender = QString();
    this->DoBday = -1;
    this->DoBmonth = -1;
    this->DoByear = -1;
}

/**
 * @brief User::writeHelper member function: helper function to write userObject in JSON file.
 */
void User::writeHelper(QJsonObject &userObject) const
{
    userObject["username"] = this->username;
    userObject["password"] = this->password;

    userObject["firstName"] = this->firstName;
    userObject["lastName"] = this->lastName;
    userObject["email"] = this->email;
    userObject["gender"] = this->gender;
    userObject["DoBday"] = this->DoBday;
    userObject["DoBmonth"] = this->DoBmonth;
    userObject["DoByear"] = this->DoByear;
}
