#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QDate>

class Contact
{
public:
    Contact();
    Contact(int id, const QString& name, const QString& email,
        const QDate& birthday, const QDate& addedDate);

    // Геттеры
    int id() const;
    QString name() const;
    QString email() const;
    QDate birthday() const;
    QDate addedDate() const;

    // Сеттеры
    void setName(const QString& name);
    void setEmail(const QString& email);
    void setBirthday(const QDate& birthday);

    QString toString() const;

private:
    int m_id;
    QString m_name;
    QString m_email;
    QDate m_birthday;
    QDate m_addedDate;
};

#endif 