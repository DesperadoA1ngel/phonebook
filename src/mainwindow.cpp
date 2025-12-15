#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QIcon>
#include <QDate>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_phoneBook(new PhoneBook(this))
    , m_currentEditId(-1)
{
    setupUI();
    setupMenu();
    setupToolbar();

    // Загружаем тестовые данные
    m_phoneBook->loadDefaultContacts();
    refreshTable();

    // Подключаем сигналы
    connect(m_phoneBook, &PhoneBook::dataChanged, this, &MainWindow::refreshTable);

    setWindowTitle("Телефонная книга");
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
    QGroupBox* formGroup = new QGroupBox("Данные контакта", this);
    QFormLayout* formLayout = new QFormLayout(formGroup);

    m_nameEdit = new QLineEdit(this);
    m_emailEdit = new QLineEdit(this);
    m_birthdayEdit = new QDateEdit(this);
    m_birthdayEdit->setCalendarPopup(true);
    m_birthdayEdit->setDate(QDate::currentDate());

    m_addedDateEdit = new QDateEdit(this);
    m_addedDateEdit->setCalendarPopup(true);
    m_addedDateEdit->setDate(QDate::currentDate());
    m_addedDateEdit->setEnabled(false); // Дата внесения не редактируется

    formLayout->addRow("ФИО:", m_nameEdit);
    formLayout->addRow("E-mail:", m_emailEdit);
    formLayout->addRow("День рождения:", m_birthdayEdit);
    formLayout->addRow("Дата внесения:", m_addedDateEdit);

    mainLayout->addWidget(formGroup);

    // Панель кнопок
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    m_addButton = new QPushButton("Добавить", this);
    m_editButton = new QPushButton("Изменить", this);
    m_deleteButton = new QPushButton("Удалить", this);
    m_searchButton = new QPushButton("Найти", this);
    m_resetButton = new QPushButton("Сброс", this);

    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_editButton);
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_searchButton);
    buttonLayout->addWidget(m_resetButton);

    mainLayout->addLayout(buttonLayout);

    // Поле поиска
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Поиск:", this));
    m_searchEdit = new QLineEdit(this);
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
    QMenu* fileMenu = menuBar()->addMenu("Файл");

    QAction* resetAction = new QAction("Сбросить данные", this);
    QAction* exitAction = new QAction("Выход", this);

    fileMenu->addAction(resetAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    QMenu* helpMenu = menuBar()->addMenu("Помощь");
    QAction* aboutAction = new QAction("О программе", this);
    helpMenu->addAction(aboutAction);

    connect(resetAction, &QAction::triggered, this, &MainWindow::onReset);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
}

void MainWindow::setupToolbar()
{
    QToolBar* toolbar = addToolBar("Основные");

    toolbar->addAction(QIcon(":/icons/add.png"), "Добавить", this, &MainWindow::onAddContact);
    toolbar->addAction(QIcon(":/icons/edit.png"), "Изменить", this, &MainWindow::onEditContact);
    toolbar->addAction(QIcon(":/icons/delete.png"), "Удалить", this, &MainWindow::onDeleteContact);
    toolbar->addSeparator();
    toolbar->addAction(QIcon(":/icons/search.png"), "Поиск", this, &MainWindow::onSearch);
    toolbar->addAction(QIcon(":/icons/reset.png"), "Сброс", this, &MainWindow::onReset);
}

void MainWindow::setupTable()
{
    m_table->setColumnCount(5);
    QStringList headers;
    headers << "ID" << "ФИО" << "E-mail" << "День рождения" << "Дата внесения";
    m_table->setHorizontalHeaderLabels(headers);

    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);

    // Подключаем сортировку по клику на заголовок
    connect(m_table->horizontalHeader(), &QHeaderView::sectionClicked,
        this, &MainWindow::onSortColumn);
}

void MainWindow::refreshTable()
{
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

        // Устанавливаем данные
        idItem->setData(Qt::UserRole, contact.id());

        m_table->setItem(i, 0, idItem);
        m_table->setItem(i, 1, nameItem);
        m_table->setItem(i, 2, emailItem);
        m_table->setItem(i, 3, birthdayItem);
        m_table->setItem(i, 4, addedDateItem);
    }

    m_table->setSortingEnabled(true);
    m_table->resizeColumnsToContents();

    // Обновляем статус бар
    statusBar()->showMessage(QString("Всего контактов: %1").arg(contacts.size()));
}

void MainWindow::onAddContact()
{
    Contact contact = getContactFromForm();
    if (contact.name().isEmpty()) {
        showError("ФИО не может быть пустым!");
        return;
    }

    m_phoneBook->addContact(contact);
    clearForm();
    showInfo("Контакт добавлен успешно!");
}

void MainWindow::onEditContact()
{
    if (m_currentEditId == -1) {
        showError("Выберите контакт для редактирования!");
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
    showInfo("Контакт обновлен успешно!");
}

void MainWindow::onDeleteContact()
{
    int row = m_table->currentRow();
    if (row == -1) {
        showError("Выберите контакт для удаления!");
        return;
    }

    QTableWidgetItem* item = m_table->item(row, 0);
    int id = item->data(Qt::UserRole).toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение",
        "Вы уверены, что хотите удалить этот контакт?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_phoneBook->removeContact(id);
        clearForm();
        showInfo("Контакт удален успешно!");
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
    statusBar()->showMessage(QString("Найдено контактов: %1").arg(contacts.size()));
}

void MainWindow::onReset()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение",
        "Вернуть телефонную книгу в исходное состояние?\nВсе текущие изменения будут потеряны.",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_phoneBook->loadDefaultContacts();
        m_searchEdit->clear();
        clearForm();
        showInfo("Данные сброшены к исходному состоянию!");
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
    return Contact(0, // ID будет присвоен при добавлении
        m_nameEdit->text(),
        m_emailEdit->text(),
        m_birthdayEdit->date(),
        m_addedDateEdit->date());
}

void MainWindow::showError(const QString& message)
{
    QMessageBox::critical(this, "Ошибка", message);
}

void MainWindow::showInfo(const QString& message)
{
    QMessageBox::information(this, "Информация", message);
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "О программе",
        "Телефонная книга\n\n"
        "Версия 1.0\n"
        "Разработана с использованием Qt 5.12.5\n\n"
        "Функции:\n"
        "- Добавление, редактирование и удаление контактов\n"
        "- Поиск по всем полям\n"
        "- Сортировка по любому столбцу\n"
        "- Возврат к исходным данным");
}