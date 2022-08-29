#include "user.h"
#include "bcrypt.h"

User::User() {}

User::User(QString name, QString _password, bool hashed) : name(name) {
    if (!hashed) {
        std::string hash = bcrypt::generateHash(_password.toStdString());
        password = QString::fromStdString(hash);
    } else {
        password = _password;
    }
}

std::pair<QString, QString> User::getUserData() {
    return {name, password};
}
