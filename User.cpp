#include "User.h"

User::User()
{

}

// So what do i really wanna do here?
// Store the user here without the password? Only check the password to confirm him?

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

void User::writeHelper(QJsonObject &userObject) const
{
    userObject["username"] = this->username;
    userObject["password"] = this->password;

    userObject["firstName"] = this->firstName;
    userObject["lastName"] = this->lastName;
}
