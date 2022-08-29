#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();

    User(QString name, QString password, bool hashed = true);

    std::pair<QString, QString> getUserData();

    inline bool operator==(const User &u) {
        return name == u.name && password == u.password;
    }

private:
    QString name;
    QString password;
};

#endif // USER_H
