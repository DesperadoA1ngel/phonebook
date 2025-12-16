#include "searchdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>

SearchDialog::SearchDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Расширенный поиск");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Текст поиска:", this));
    m_searchEdit = new QLineEdit(this);
    searchLayout->addWidget(m_searchEdit);
    mainLayout->addLayout(searchLayout);

    m_nameCheck = new QCheckBox("Искать в ФИО", this);
    m_emailCheck = new QCheckBox("Искать в e-mail", this);
    m_birthdayCheck = new QCheckBox("Искать в дате рождения", this);
    m_addedDateCheck = new QCheckBox("Искать в дате внесения", this);

    m_nameCheck->setChecked(true);
    m_emailCheck->setChecked(true);
    m_birthdayCheck->setChecked(true);
    m_addedDateCheck->setChecked(true);

    mainLayout->addWidget(m_nameCheck);
    mainLayout->addWidget(m_emailCheck);
    mainLayout->addWidget(m_birthdayCheck);
    mainLayout->addWidget(m_addedDateCheck);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        this);

    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Устанавливаем размер
    resize(400, 250);
}

QString SearchDialog::searchText() const
{
    return m_searchEdit->text();
}

bool SearchDialog::searchInName() const
{
    return m_nameCheck->isChecked();
}

bool SearchDialog::searchInEmail() const
{
    return m_emailCheck->isChecked();
}

bool SearchDialog::searchInBirthday() const
{
    return m_birthdayCheck->isChecked();
}

bool SearchDialog::searchInAddedDate() const
{
    return m_addedDateCheck->isChecked();
}