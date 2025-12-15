#include "contact.h"
#include <QStringList>

Contact::Contact()
    : m_id(0)
{
}

Contact::Contact(int id, const QString& name, const QString& email,
    const QDate& birthday, const QDate& addedDate)
    : m_id(id)
    , m_name(name)
    , m_email(email)
    , m_birthday(birthday)
    , m_addedDate(addedDate)
{
}

int Contact::id() const { return m_id; }
QString Contact::name() const { return m_name; }
QString Contact::email() const { return m_email; }
QDate Contact::birthday() const { return m_birthday; }
QDate Contact::addedDate() const { return m_addedDate; }

void Contact::setName(const QString& name) { m_name = name; }
void Contact::setEmail(const QString& email) { m_email = email; }
void Contact::setBirthday(const QDate& birthday) { m_birthday = birthday; }

QString Contact::toString() const
{
    return QString("ID: %1, Name: %2, Email: %3, Birthday: %4, Added: %5")
        .arg(m_id)
        .arg(m_name)
        .arg(m_email)
        .arg(m_birthday.toString("dd.MM.yyyy"))
        .arg(m_addedDate.toString("dd.MM.yyyy"));
}