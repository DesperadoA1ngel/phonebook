#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QDate>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDateEdit>
#include <QTableWidget>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_phoneBook(new PhoneBook(this))
    , m_currentEditId(-1)
{
    setupUI();
    setupMenu();

    // Загружаем тестовые данные
    m_phoneBook->loadDefaultContacts();
    refreshTable();

    // Подключаем сигналы
    connect(m_phoneBook, &PhoneBook::dataChanged, this, &MainWindow::refreshTable);

    setWindowTitle("Phone Book");
    resize(1000, 600);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Создаем таблицу
    m_table = new QTableWidget(this);
    setupTable();
    mainLayout->addWidget(m_table);

    // Создаем форму для ввода данных
    QGroupBox* formGroup = new QGroupBox("Contact Data", this);
    QFormLayout* formLayout = new QFormLayout(formGroup);

    m_nameEdit = new QLineEdit(this);
    m_emailEdit = new QLineEdit(this);
    m_birthdayEdit = new QDateEdit(this);
    m_birthdayEdit->setCalendarPopup(true);
    m_birthdayEdit->setDate(QDate::currentDate());

    m_addedDateEdit = new QDateEdit(this);
    m_addedDateEdit->setCalendarPopup(true);
    m_addedDateEdit->setDate(QDate::currentDate());
    m_addedDateEdit->setEnabled(false);

    formLayout->addRow("Full Name:", m_nameEdit);
    formLayout->addRow("E-mail:", m_emailEdit);
    formLayout->addRow("Birthday:", m_birthdayEdit);
    formLayout->addRow("Added Date:", m_addedDateEdit);

    mainLayout->addWidget(formGroup);

    // Панель кнопок управления
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    m_addButton = new QPushButton("Add", this);
    m_editButton = new QPushButton("Edit", this);
    m_deleteButton = new QPushButton("Delete", this);
    m_searchButton = new QPushButton("Search", this);
    m_resetButton = new QPushButton("Reset", this);

    // Устанавливаем размеры кнопок
    m_addButton->setFixedWidth(80);
    m_editButton->setFixedWidth(80);
    m_deleteButton->setFixedWidth(80);
    m_searchButton->setFixedWidth(80);
    m_resetButton->setFixedWidth(80);

    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_editButton);
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_searchButton);
    buttonLayout->addWidget(m_resetButton);

    mainLayout->addLayout(buttonLayout);

    // Поле поиска
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Search:", this));
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Enter search text...");
    searchLayout->addWidget(m_searchEdit);
    mainLayout->addLayout(searchLayout);

    setCentralWidget(centralWidget);

    // Подключаем кнопки
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::onAddContact);
    connect(m_editButton, &QPushButton::clicked, this, &MainWindow::onEditContact);
    connect(m_deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteContact);
    connect(m_searchButton, &QPushButton::clicked, this, &MainWindow::onSearch);
    connect(m_resetButton, &QPushButton::clicked, this, &MainWindow::onReset);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearch);

    // Двойной клик по таблице
    connect(m_table, &QTableWidget::cellDoubleClicked, this, &MainWindow::onTableDoubleClick);
}

void MainWindow::setupMenu()
{
    // Меню File
    QMenu* fileMenu = menuBar()->addMenu("File");

    QAction* resetAction = new QAction("Reset Data", this);
    QAction* exitAction = new QAction("Exit", this);

    fileMenu->addAction(resetAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    // Меню Help
    QMenu* helpMenu = menuBar()->addMenu("Help");
    QAction* aboutAction = new QAction("About", this);
    helpMenu->addAction(aboutAction);

    // Подключаем меню
    connect(resetAction, &QAction::triggered, this, &MainWindow::onReset);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
}

void MainWindow::setupTable()
{
    m_table->setColumnCount(5);
    QStringList headers;
    headers << "ID" << "Full Name" << "E-mail" << "Birthday" << "Added Date";
    m_table->setHorizontalHeaderLabels(headers);

    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Настройка заголовков таблицы
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    m_table->verticalHeader()->setVisible(false);

    // Подключаем сортировку по клику на заголовок
    connect(m_table->horizontalHeader(), &QHeaderView::sectionClicked,
        this, &MainWindow::onSortColumn);
}

void MainWindow::refreshTable()
{
    // Временно отключаем сортировку
    m_table->setSortingEnabled(false);
    m_table->clearContents();

    QList<Contact> contacts = m_phoneBook->getAllContacts();
    m_table->setRowCount(contacts.size());

    for (int i = 0; i < contacts.size(); ++i) {
        const Contact& contact = contacts[i];

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(contact.id()));
        QTableWidgetItem* nameItem = new QTableWidgetItem(contact.name());
        QTableWidgetItem* emailItem = new QTableWidgetItem(contact.email());
        QTableWidgetItem* birthdayItem = new QTableWidgetItem(contact.birthday().toString("dd.MM.yyyy"));
        QTableWidgetItem* addedDateItem = new QTableWidgetItem(contact.addedDate().toString("dd.MM.yyyy"));

        // Сохраняем ID для быстрого доступа
        idItem->setData(Qt::UserRole, contact.id());

        m_table->setItem(i, 0, idItem);
        m_table->setItem(i, 1, nameItem);
        m_table->setItem(i, 2, emailItem);
        m_table->setItem(i, 3, birthdayItem);
        m_table->setItem(i, 4, addedDateItem);
    }

    // Включаем сортировку обратно
    m_table->setSortingEnabled(true);
    m_table->resizeColumnsToContents();

    // Обновляем статус бар
    statusBar()->showMessage(QString("Total contacts: %1").arg(contacts.size()));
}

void MainWindow::onAddContact()
{
    Contact contact = getContactFromForm();
    if (contact.name().isEmpty()) {
        showError("Full Name cannot be empty!");
        return;
    }

    m_phoneBook->addContact(contact);
    clearForm();
    showInfo("Contact added successfully!");
}

void MainWindow::onEditContact()
{
    if (m_currentEditId == -1) {
        showError("Please select a contact to edit!");
        return;
    }

    Contact contact = getContactFromForm();
    contact = Contact(m_currentEditId,
        contact.name(),
        contact.email(),
        contact.birthday(),
        contact.addedDate());

    m_phoneBook->updateContact(contact);
    clearForm();
    showInfo("Contact updated successfully!");
}

void MainWindow::onDeleteContact()
{
    int row = m_table->currentRow();
    if (row == -1) {
        showError("Please select a contact to delete!");
        return;
    }

    QTableWidgetItem* item = m_table->item(row, 0);
    int id = item->data(Qt::UserRole).toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
        "Are you sure you want to delete this contact?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_phoneBook->removeContact(id);
        clearForm();
        showInfo("Contact deleted successfully!");
    }
}

void MainWindow::onSearch()
{
    QString query = m_searchEdit->text();
    if (query.isEmpty()) {
        refreshTable();
        return;
    }

    m_table->setSortingEnabled(false);
    m_table->clearContents();

    QList<Contact> contacts = m_phoneBook->search(query);
    m_table->setRowCount(contacts.size());

    for (int i = 0; i < contacts.size(); ++i) {
        const Contact& contact = contacts[i];

        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(contact.id()));
        QTableWidgetItem* nameItem = new QTableWidgetItem(contact.name());
        QTableWidgetItem* emailItem = new QTableWidgetItem(contact.email());
        QTableWidgetItem* birthdayItem = new QTableWidgetItem(contact.birthday().toString("dd.MM.yyyy"));
        QTableWidgetItem* addedDateItem = new QTableWidgetItem(contact.addedDate().toString("dd.MM.yyyy"));

        idItem->setData(Qt::UserRole, contact.id());

        m_table->setItem(i, 0, idItem);
        m_table->setItem(i, 1, nameItem);
        m_table->setItem(i, 2, emailItem);
        m_table->setItem(i, 3, birthdayItem);
        m_table->setItem(i, 4, addedDateItem);
    }

    m_table->setSortingEnabled(true);
    statusBar()->showMessage(QString("Found contacts: %1").arg(contacts.size()));
}

void MainWindow::onReset()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
        "Reset phone book to initial state?\nAll current changes will be lost.",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_phoneBook->loadDefaultContacts();
        m_searchEdit->clear();
        clearForm();
        showInfo("Data has been reset to initial state!");
    }
}

void MainWindow::onSortColumn(int column)
{
    PhoneBook::SortColumn sortColumn;
    switch (column) {
    case 0: sortColumn = PhoneBook::SortById; break;
    case 1: sortColumn = PhoneBook::SortByName; break;
    case 2: sortColumn = PhoneBook::SortByEmail; break;
    case 3: sortColumn = PhoneBook::SortByBirthday; break;
    case 4: sortColumn = PhoneBook::SortByAddedDate; break;
    default: sortColumn = PhoneBook::SortById;
    }

    m_phoneBook->sortByColumn(sortColumn,
        m_table->horizontalHeader()->sortIndicatorOrder());
}

void MainWindow::onTableDoubleClick(int row, int column)
{
    Q_UNUSED(column);

    QTableWidgetItem* item = m_table->item(row, 0);
    int id = item->data(Qt::UserRole).toInt();

    Contact contact = m_phoneBook->getContact(id);
    if (contact.id() != 0) {
        loadContactToForm(contact);
        m_currentEditId = contact.id();
    }
}

void MainWindow::clearForm()
{
    m_nameEdit->clear();
    m_emailEdit->clear();
    m_birthdayEdit->setDate(QDate::currentDate());
    m_addedDateEdit->setDate(QDate::currentDate());
    m_currentEditId = -1;

    // Снимаем выделение с таблицы
    m_table->clearSelection();
}

void MainWindow::loadContactToForm(const Contact& contact)
{
    m_nameEdit->setText(contact.name());
    m_emailEdit->setText(contact.email());
    m_birthdayEdit->setDate(contact.birthday());
    m_addedDateEdit->setDate(contact.addedDate());
}

Contact MainWindow::getContactFromForm() const
{
    return Contact(0, // ID will be assigned when adding
        m_nameEdit->text(),
        m_emailEdit->text(),
        m_birthdayEdit->date(),
        m_addedDateEdit->date());
}

void MainWindow::showError(const QString& message)
{
    QMessageBox::critical(this, "Error", message);
}

void MainWindow::showInfo(const QString& message)
{
    QMessageBox::information(this, "Information", message);
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "About Phone Book",
        "Phone Book Application\n\n"
        "Version 1.0\n"
        "Developed using Qt 5.15.2\n\n"
        "Features:\n"
        "- Add, edit and delete contacts\n"
        "- Search across all fields\n"
        "- Sort by any column\n"
        "- Reset to initial data\n"
        "- Cross-platform (Windows/Linux)");
}