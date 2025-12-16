#include "phonebook.h"
#include <algorithm>
#include <QDebug>

PhoneBook::PhoneBook(QObject* parent)
    : QObject(parent)
    , m_nextId(1)
{
}

bool PhoneBook::addContact(const Contact& contact)
{
    Contact newContact = contact;
    newContact = Contact(m_nextId++,
        contact.name(),
        contact.email(),
        contact.birthday(),
        contact.addedDate());

    m_contacts.append(newContact);
    emit dataChanged();
    return true;
}

bool PhoneBook::removeContact(int id)
{
    auto it = std::remove_if(m_contacts.begin(), m_contacts.end(),
        [id](const Contact& contact) { return contact.id() == id; });

    if (it != m_contacts.end()) {
        m_contacts.erase(it, m_contacts.end());
        emit dataChanged();
        return true;
    }
    return false;
}

bool PhoneBook::updateContact(const Contact& contact)
{
    for (auto& c : m_contacts) {
        if (c.id() == contact.id()) {
            c = contact;
            emit dataChanged();
            return true;
        }
    }
    return false;
}

Contact PhoneBook::getContact(int id) const
{
    for (const auto& contact : m_contacts) {
        if (contact.id() == id) {
            return contact;
        }
    }
    return Contact();
}

QList<Contact> PhoneBook::getAllContacts() const
{
    return m_contacts;
}

QList<Contact> PhoneBook::search(const QString& query) const
{
    if (query.isEmpty()) {
        return m_contacts;
    }

    QList<Contact> results;
    QString searchTerm = query.toLower();

    for (const auto& contact : m_contacts) {
        if (contact.name().toLower().contains(searchTerm) ||
            contact.email().toLower().contains(searchTerm) ||
            contact.birthday().toString("dd.MM.yyyy").contains(searchTerm) ||
            contact.addedDate().toString("dd.MM.yyyy").contains(searchTerm) ||
            QString::number(contact.id()).contains(searchTerm)) {
            results.append(contact);
        }
    }

    return results;
}

void PhoneBook::sortByColumn(SortColumn column, Qt::SortOrder order)
{
    std::stable_sort(m_contacts.begin(), m_contacts.end(),
        [column](const Contact& a, const Contact& b) {
            switch (column) {
            case SortById:
                return a.id() < b.id();

            case SortByName:
                return a.name().toLower() < b.name().toLower();

            case SortByEmail:
                return a.email().toLower() < b.email().toLower();

            case SortByBirthday:
                return a.birthday() < b.birthday();

            case SortByAddedDate:
                return a.addedDate() < b.addedDate();

            default:
                return a.id() < b.id();
            }
        });

    if (order == Qt::DescendingOrder) {
        std::reverse(m_contacts.begin(), m_contacts.end());
    }

    emit dataChanged();
}

void PhoneBook::loadDefaultContacts()
{
    clear();
    m_contacts = generateDefaultContacts();

    // Находим максимальный ID
    int maxId = 0;
    for (const auto& contact : m_contacts) {
        if (contact.id() > maxId) {
            maxId = contact.id();
        }
    }
    m_nextId = maxId + 1;

    // Сортируем по ID по умолчанию
    sortByColumn(SortById, Qt::AscendingOrder);
}

void PhoneBook::clear()
{
    m_contacts.clear();
    m_nextId = 1;
    emit dataChanged();
}

int PhoneBook::count() const
{
    return m_contacts.size();
}

QList<Contact> PhoneBook::generateDefaultContacts()
{
    QList<Contact> contacts;

    contacts << Contact(1, "Ivanov Ivan Ivanovich", "ivanov@mail.com",
        QDate(1985, 5, 15), QDate(2023, 1, 10));
    contacts << Contact(2, "Petrov Petr Petrovich", "petrov@mail.com",
        QDate(1990, 8, 22), QDate(2023, 1, 12));
    contacts << Contact(3, "Sidorova Anna Vladimirovna", "sidorova@mail.com",
        QDate(1988, 3, 30), QDate(2023, 1, 15));
    contacts << Contact(4, "Kuznetsov Alexey Sergeevich", "kuznetsov@mail.com",
        QDate(1995, 11, 5), QDate(2023, 2, 1));
    contacts << Contact(5, "Smirnova Elena Dmitrievna", "smirnova@mail.com",
        QDate(1992, 7, 18), QDate(2023, 2, 3));
    contacts << Contact(6, "Vasilyev Dmitry Alexeevich", "vasilyev@mail.com",
        QDate(1983, 9, 25), QDate(2023, 2, 10));
    contacts << Contact(7, "Pavlova Olga Igorevna", "pavlova@mail.com",
        QDate(1998, 12, 12), QDate(2023, 2, 15));
    contacts << Contact(8, "Nikolaev Mikhail Petrovich", "nikolaev@mail.com",
        QDate(1979, 4, 7), QDate(2023, 2, 20));
    contacts << Contact(9, "Fedorova Maria Sergeevna", "fedorova@mail.com",
        QDate(1991, 6, 9), QDate(2023, 3, 1));
    contacts << Contact(10, "Dmitriev Andrey Vladimirovich", "dmitriev@mail.com",
        QDate(1987, 2, 28), QDate(2023, 3, 5));

    return contacts;
}