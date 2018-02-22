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
            if (userObject.contains("username") && userObject["username"].isString() && userObject["username"] == this->username) {
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
        for (int userIndex = 0; userIndex < userArray.size(); userIndex++) {
            QJsonObject userObject = userArray[userIndex].toObject();
            if (userObject.contains("username") && userObject["username"].isString() && userObject["username"] == this->username) {
                userObject["firstName"] = this->firstName;
                userObject["lastName"] = this->lastName;
                return true;
            }
        }
    }
}
