/********************************************************************************
** Form generated from reading UI file 'rendersettingdialogform.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENDERSETTINGDIALOGFORM_H
#define UI_RENDERSETTINGDIALOGFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QSplitter *splitter_2;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *znLabel;
    QDoubleSpinBox *zfSpinBox;
    QDoubleSpinBox *gammaSpinBox;
    QDoubleSpinBox *znSpinBox;
    QLabel *depthLabel;
    QDoubleSpinBox *shSpinBox;
    QLabel *swLabel;
    QSpinBox *depthSpinBox;
    QLabel *shLabel;
    QLabel *gammaLabel;
    QLabel *zfLabel;
    QDoubleSpinBox *swSpinBox;
    QLabel *heightLabel;
    QSpinBox *heightSpinBox;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *roughtRadioButton;
    QRadioButton *normalRadioButton;
    QRadioButton *fineRadioButton;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout;
    QPushButton *colorButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(383, 180);
        splitter_2 = new QSplitter(Dialog);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setGeometry(QRect(20, 20, 342, 144));
        splitter_2->setOrientation(Qt::Vertical);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        znLabel = new QLabel(layoutWidget);
        znLabel->setObjectName(QStringLiteral("znLabel"));
        znLabel->setMinimumSize(QSize(31, 0));

        gridLayout->addWidget(znLabel, 1, 0, 1, 1);

        zfSpinBox = new QDoubleSpinBox(layoutWidget);
        zfSpinBox->setObjectName(QStringLiteral("zfSpinBox"));
        zfSpinBox->setEnabled(false);

        gridLayout->addWidget(zfSpinBox, 1, 3, 1, 1);

        gammaSpinBox = new QDoubleSpinBox(layoutWidget);
        gammaSpinBox->setObjectName(QStringLiteral("gammaSpinBox"));
        gammaSpinBox->setEnabled(false);

        gridLayout->addWidget(gammaSpinBox, 3, 1, 1, 1);

        znSpinBox = new QDoubleSpinBox(layoutWidget);
        znSpinBox->setObjectName(QStringLiteral("znSpinBox"));
        znSpinBox->setEnabled(false);

        gridLayout->addWidget(znSpinBox, 1, 1, 1, 1);

        depthLabel = new QLabel(layoutWidget);
        depthLabel->setObjectName(QStringLiteral("depthLabel"));

        gridLayout->addWidget(depthLabel, 3, 2, 1, 1);

        shSpinBox = new QDoubleSpinBox(layoutWidget);
        shSpinBox->setObjectName(QStringLiteral("shSpinBox"));
        shSpinBox->setEnabled(false);

        gridLayout->addWidget(shSpinBox, 2, 3, 1, 1);

        swLabel = new QLabel(layoutWidget);
        swLabel->setObjectName(QStringLiteral("swLabel"));

        gridLayout->addWidget(swLabel, 2, 0, 1, 1);

        depthSpinBox = new QSpinBox(layoutWidget);
        depthSpinBox->setObjectName(QStringLiteral("depthSpinBox"));
        depthSpinBox->setEnabled(false);

        gridLayout->addWidget(depthSpinBox, 3, 3, 1, 1);

        shLabel = new QLabel(layoutWidget);
        shLabel->setObjectName(QStringLiteral("shLabel"));

        gridLayout->addWidget(shLabel, 2, 2, 1, 1);

        gammaLabel = new QLabel(layoutWidget);
        gammaLabel->setObjectName(QStringLiteral("gammaLabel"));

        gridLayout->addWidget(gammaLabel, 3, 0, 1, 1);

        zfLabel = new QLabel(layoutWidget);
        zfLabel->setObjectName(QStringLiteral("zfLabel"));

        gridLayout->addWidget(zfLabel, 1, 2, 1, 1);

        swSpinBox = new QDoubleSpinBox(layoutWidget);
        swSpinBox->setObjectName(QStringLiteral("swSpinBox"));
        swSpinBox->setEnabled(false);

        gridLayout->addWidget(swSpinBox, 2, 1, 1, 1);

        heightLabel = new QLabel(layoutWidget);
        heightLabel->setObjectName(QStringLiteral("heightLabel"));

        gridLayout->addWidget(heightLabel, 4, 0, 1, 1);

        heightSpinBox = new QSpinBox(layoutWidget);
        heightSpinBox->setObjectName(QStringLiteral("heightSpinBox"));
        heightSpinBox->setMinimum(50);
        heightSpinBox->setMaximum(10000);

        gridLayout->addWidget(heightSpinBox, 4, 1, 1, 1);

        splitter->addWidget(layoutWidget);
        groupBox = new QGroupBox(splitter);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setEnabled(false);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        roughtRadioButton = new QRadioButton(groupBox);
        roughtRadioButton->setObjectName(QStringLiteral("roughtRadioButton"));

        verticalLayout_2->addWidget(roughtRadioButton);

        normalRadioButton = new QRadioButton(groupBox);
        normalRadioButton->setObjectName(QStringLiteral("normalRadioButton"));

        verticalLayout_2->addWidget(normalRadioButton);

        fineRadioButton = new QRadioButton(groupBox);
        fineRadioButton->setObjectName(QStringLiteral("fineRadioButton"));

        verticalLayout_2->addWidget(fineRadioButton);

        splitter->addWidget(groupBox);
        splitter_2->addWidget(splitter);
        layoutWidget1 = new QWidget(splitter_2);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        horizontalLayout = new QHBoxLayout(layoutWidget1);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        colorButton = new QPushButton(layoutWidget1);
        colorButton->setObjectName(QStringLiteral("colorButton"));

        horizontalLayout->addWidget(colorButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        okButton = new QPushButton(layoutWidget1);
        okButton->setObjectName(QStringLiteral("okButton"));

        horizontalLayout->addWidget(okButton);

        cancelButton = new QPushButton(layoutWidget1);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        horizontalLayout->addWidget(cancelButton);

        splitter_2->addWidget(layoutWidget1);

        retranslateUi(Dialog);
        QObject::connect(okButton, SIGNAL(clicked()), Dialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0));
        znLabel->setText(QApplication::translate("Dialog", "ZN", 0));
        depthLabel->setText(QApplication::translate("Dialog", "Depth", 0));
        swLabel->setText(QApplication::translate("Dialog", "SW", 0));
        shLabel->setText(QApplication::translate("Dialog", "SH", 0));
        gammaLabel->setText(QApplication::translate("Dialog", "Gamma", 0));
        zfLabel->setText(QApplication::translate("Dialog", "ZF", 0));
        heightLabel->setText(QApplication::translate("Dialog", "Height", 0));
        groupBox->setTitle(QApplication::translate("Dialog", "Quality mode", 0));
        roughtRadioButton->setText(QApplication::translate("Dialog", "Rought", 0));
        normalRadioButton->setText(QApplication::translate("Dialog", "Normal", 0));
        fineRadioButton->setText(QApplication::translate("Dialog", "Fine", 0));
        colorButton->setText(QApplication::translate("Dialog", "Color", 0));
        okButton->setText(QApplication::translate("Dialog", "OK", 0));
        cancelButton->setText(QApplication::translate("Dialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENDERSETTINGDIALOGFORM_H
