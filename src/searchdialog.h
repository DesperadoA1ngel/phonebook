#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget* parent = nullptr);

    QString searchText() const;
    bool searchInName() const;
    bool searchInEmail() const;
    bool searchInBirthday() const;
    bool searchInAddedDate() const;

private:
    QLineEdit* m_searchEdit;
    QCheckBox* m_nameCheck;
    QCheckBox* m_emailCheck;
    QCheckBox* m_birthdayCheck;
    QCheckBox* m_addedDateCheck;
    QPushButton* m_searchButton;
    QPushButton* m_cancelButton;
};

#endif // SEARCHDIALOG_H