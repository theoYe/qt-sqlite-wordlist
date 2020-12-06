/****************************************************************************
**
**
**
**
**
****************************************************************************/

#include "adddialog.h"

#include <QtWidgets>

//! [0]
AddDialog::AddDialog(QWidget *parent)
    : QDialog(parent)
{
    spellingLabel = new QLabel("spelling");
    meaningLabel = new QLabel("meaning");
    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");

    spellingText = new QLineEdit;
    meaningText = new QTextEdit;

    QGridLayout *gLayout = new QGridLayout;
    gLayout->setColumnStretch(1, 2);
    gLayout->addWidget(spellingLabel, 0, 0);
    gLayout->addWidget(spellingText, 0, 1);

    gLayout->addWidget(meaningLabel, 1, 0, Qt::AlignLeft|Qt::AlignTop);
    gLayout->addWidget(meaningText, 1, 1, Qt::AlignLeft);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    gLayout->addLayout(buttonLayout, 2, 1, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    setLayout(mainLayout);

    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

    setWindowTitle(tr("Add a Word"));
}
//! [0]
