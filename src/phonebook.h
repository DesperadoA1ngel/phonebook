#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include "contact.h"
#include <QList>
#include <QObject>

class PhoneBook : public QObject
{
    Q_OBJECT

public:
    explicit PhoneBook(QObject* parent = nullptr);

    bool addContact(const Contact& contact);
    bool removeContact(int id);
    bool updateContact(const Contact& contact);
    Contact getContact(int id) const;
    QList<Contact> getAllContacts() const;

    QList<Contact> search(const QString& query) const;

    // Сортировка
    enum SortColumn {
        SortById,
        SortByName,
        SortByEmail,
        SortByBirthday,
        SortByAddedDate
    };

    void sortByColumn(SortColumn column, Qt::SortOrder order = Qt::AscendingOrder);

    // Управление данными
    void loadDefaultContacts();
    void clear();
    int count() const;

    // Статические методы для генерации тестовых данных
    static QList<Contact> generateDefaultContacts();

signals:
    void dataChanged();

private:
    QList<Contact> m_contacts;
    int m_nextId;
};

#endif 