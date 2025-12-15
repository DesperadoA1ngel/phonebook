#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "phonebook.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QLabel>
#include <QMenuBar>
#include <QStatusBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onAddContact();
    void onEditContact();
    void onDeleteContact();
    void onSearch();
    void onReset();
    void onSortColumn(int column);
    void onTableDoubleClick(int row, int column);
    void onAbout();
    void refreshTable();

private:
    void setupUI();
    void setupMenu();
    void setupTable();
    void clearForm();
    void loadContactToForm(const Contact& contact);
    Contact getContactFromForm() const;
    void showError(const QString& message);
    void showInfo(const QString& message);

    PhoneBook* m_phoneBook;
    QTableWidget* m_table;

    // Ёлементы формы
    QLineEdit* m_nameEdit;
    QLineEdit* m_emailEdit;
    QDateEdit* m_birthdayEdit;
    QDateEdit* m_addedDateEdit;

    QPushButton* m_addButton;
    QPushButton* m_editButton;
    QPushButton* m_deleteButton;
    QPushButton* m_searchButton;
    QPushButton* m_resetButton;

    QLineEdit* m_searchEdit;

    int m_currentEditId;
};

#endif // MAINWINDOW_H