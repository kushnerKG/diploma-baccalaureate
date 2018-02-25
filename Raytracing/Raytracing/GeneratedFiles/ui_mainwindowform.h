/********************************************************************************
** Form generated from reading UI file 'mainwindowform.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOWFORM_H
#define UI_MAINWINDOWFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "CanvasWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *openRenderFileAction;
    QAction *saveRenderSettingAction;
    QAction *saveImageAction;
    QAction *renderSettingDiolagAction;
    QAction *initStateAction;
    QAction *renderAction;
    QAction *chooseViewAction;
    QAction *openHelpInfoAction;
    QAction *quitAction;
    QAction *openSceneFileAction;
    QAction *showCoordSystemAction;
    QAction *spectrumRenderAction;
    QAction *polarizatiomRenderAction;
    QAction *dispersionRenderSurfaceAction;
    QAction *openCommandsFileAction;
    QAction *polarizationTransferAction;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea;
    CanvasWidget *canvasWidget;
    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *menu;
    QMenu *menu_2;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(804, 600);
        openRenderFileAction = new QAction(MainWindow);
        openRenderFileAction->setObjectName(QStringLiteral("openRenderFileAction"));
        saveRenderSettingAction = new QAction(MainWindow);
        saveRenderSettingAction->setObjectName(QStringLiteral("saveRenderSettingAction"));
        saveImageAction = new QAction(MainWindow);
        saveImageAction->setObjectName(QStringLiteral("saveImageAction"));
        renderSettingDiolagAction = new QAction(MainWindow);
        renderSettingDiolagAction->setObjectName(QStringLiteral("renderSettingDiolagAction"));
        initStateAction = new QAction(MainWindow);
        initStateAction->setObjectName(QStringLiteral("initStateAction"));
        initStateAction->setEnabled(false);
        renderAction = new QAction(MainWindow);
        renderAction->setObjectName(QStringLiteral("renderAction"));
        renderAction->setCheckable(true);
        chooseViewAction = new QAction(MainWindow);
        chooseViewAction->setObjectName(QStringLiteral("chooseViewAction"));
        chooseViewAction->setEnabled(false);
        openHelpInfoAction = new QAction(MainWindow);
        openHelpInfoAction->setObjectName(QStringLiteral("openHelpInfoAction"));
        quitAction = new QAction(MainWindow);
        quitAction->setObjectName(QStringLiteral("quitAction"));
        openSceneFileAction = new QAction(MainWindow);
        openSceneFileAction->setObjectName(QStringLiteral("openSceneFileAction"));
        openSceneFileAction->setIconVisibleInMenu(true);
        showCoordSystemAction = new QAction(MainWindow);
        showCoordSystemAction->setObjectName(QStringLiteral("showCoordSystemAction"));
        showCoordSystemAction->setCheckable(true);
        showCoordSystemAction->setChecked(true);
        spectrumRenderAction = new QAction(MainWindow);
        spectrumRenderAction->setObjectName(QStringLiteral("spectrumRenderAction"));
        spectrumRenderAction->setCheckable(true);
        polarizatiomRenderAction = new QAction(MainWindow);
        polarizatiomRenderAction->setObjectName(QStringLiteral("polarizatiomRenderAction"));
        polarizatiomRenderAction->setCheckable(true);
        dispersionRenderSurfaceAction = new QAction(MainWindow);
        dispersionRenderSurfaceAction->setObjectName(QStringLiteral("dispersionRenderSurfaceAction"));
        dispersionRenderSurfaceAction->setCheckable(true);
        openCommandsFileAction = new QAction(MainWindow);
        openCommandsFileAction->setObjectName(QStringLiteral("openCommandsFileAction"));
        polarizationTransferAction = new QAction(MainWindow);
        polarizationTransferAction->setObjectName(QStringLiteral("polarizationTransferAction"));
        polarizationTransferAction->setEnabled(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        centralwidget->setAutoFillBackground(false);
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setWidgetResizable(true);
        scrollArea->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        canvasWidget = new CanvasWidget();
        canvasWidget->setObjectName(QStringLiteral("canvasWidget"));
        canvasWidget->setEnabled(true);
        canvasWidget->setGeometry(QRect(0, 0, 784, 513));
        scrollArea->setWidget(canvasWidget);

        horizontalLayout->addWidget(scrollArea);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 804, 21));
        fileMenu = new QMenu(menubar);
        fileMenu->setObjectName(QStringLiteral("fileMenu"));
        editMenu = new QMenu(menubar);
        editMenu->setObjectName(QStringLiteral("editMenu"));
        helpMenu = new QMenu(menubar);
        helpMenu->setObjectName(QStringLiteral("helpMenu"));
        menu = new QMenu(menubar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(98, 108, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(226, 228, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(162, 168, 255, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(49, 54, 127, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(65, 72, 170, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush6(QColor(255, 255, 255, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush7(QColor(176, 181, 255, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush7);
        QBrush brush8(QColor(255, 255, 220, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        statusbar->setPalette(palette);
        statusbar->setAutoFillBackground(true);
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setEnabled(true);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(fileMenu->menuAction());
        menubar->addAction(editMenu->menuAction());
        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(helpMenu->menuAction());
        fileMenu->addAction(openSceneFileAction);
        fileMenu->addAction(openRenderFileAction);
        fileMenu->addAction(openCommandsFileAction);
        fileMenu->addAction(saveRenderSettingAction);
        fileMenu->addAction(saveImageAction);
        fileMenu->addSeparator();
        fileMenu->addAction(quitAction);
        editMenu->addAction(renderSettingDiolagAction);
        editMenu->addAction(showCoordSystemAction);
        editMenu->addAction(initStateAction);
        editMenu->addAction(chooseViewAction);
        helpMenu->addAction(openHelpInfoAction);
        menu->addAction(renderAction);
        menu->addAction(spectrumRenderAction);
        menu->addSeparator();
        menu->addAction(polarizatiomRenderAction);
        menu->addAction(dispersionRenderSurfaceAction);
        menu_2->addAction(polarizationTransferAction);
        toolBar->addAction(openSceneFileAction);
        toolBar->addAction(openCommandsFileAction);
        toolBar->addAction(renderSettingDiolagAction);
        toolBar->addAction(saveImageAction);
        toolBar->addAction(polarizationTransferAction);
        toolBar->addAction(showCoordSystemAction);
        toolBar->addAction(renderAction);
        toolBar->addAction(spectrumRenderAction);
        toolBar->addAction(polarizatiomRenderAction);
        toolBar->addAction(dispersionRenderSurfaceAction);
        toolBar->addAction(quitAction);
        toolBar->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        openRenderFileAction->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214 \320\275\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \320\276\321\202\321\200\320\270\321\201\320\276\320\262\320\272\320\270", 0));
#ifndef QT_NO_STATUSTIP
        openRenderFileAction->setStatusTip(QApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214 \320\275\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \320\276\321\202\321\200\320\270\321\201\320\276\320\262\320\272\320\270, \321\205\321\200\320\260\320\275\321\217\321\211\320\270\320\265\321\201\321\217 \320\275\320\260 \320\264\320\270\321\201\320\272\320\265", 0));
#endif // QT_NO_STATUSTIP
        saveRenderSettingAction->setText(QApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\275\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \320\276\321\202\321\200\320\270\321\201\320\276\320\262\320\272\320\270", 0));
#ifndef QT_NO_STATUSTIP
        saveRenderSettingAction->setStatusTip(QApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\275\320\260\321\201\321\200\320\276\320\271\320\272\320\270 \320\276\321\202\321\200\320\270\321\201\320\276\320\262\320\272\320\270 \320\275\320\260 \320\264\320\270\321\201\320\272", 0));
#endif // QT_NO_STATUSTIP
        saveImageAction->setText(QApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\265", 0));
#ifndef QT_NO_STATUSTIP
        saveImageAction->setStatusTip(QApplication::translate("MainWindow", "\321\201\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\275\320\260 \320\264\320\270\321\201\320\272 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\265 \320\270\320\267 \321\200\320\260\320\261\320\276\321\207\320\265\320\271 \320\276\320\261\320\273\320\260\321\201\321\202\320\270", 0));
#endif // QT_NO_STATUSTIP
        renderSettingDiolagAction->setText(QApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \320\276\321\202\321\200\320\270\321\201\320\276\320\262\320\272\320\270", 0));
#ifndef QT_NO_STATUSTIP
        renderSettingDiolagAction->setStatusTip(QApplication::translate("MainWindow", "\320\276\321\202\320\272\321\200\321\213\321\202\321\214 \320\264\320\270\320\260\320\273\320\276\320\263 \320\275\320\260\321\201\321\202\321\200\320\276\320\265\320\272 \320\276\321\202\321\200\320\270\321\201\320\276\320\262\320\272\320\270 \320\276\321\202\321\200\320\270\321\201\320\276\320\262\320\272\320\270(Rendering)", 0));
#endif // QT_NO_STATUSTIP
        initStateAction->setText(QApplication::translate("MainWindow", "\320\235\320\260\321\207\320\260\320\273\321\214\320\275\320\276\320\265 \320\277\320\276\320\273\320\276\320\266\320\265\320\275\320\270\320\265", 0));
#ifndef QT_NO_STATUSTIP
        initStateAction->setStatusTip(QApplication::translate("MainWindow", "\320\222\321\213\321\201\321\202\320\260\320\262\320\270\321\202\321\214 \321\201\321\206\320\265\320\275\321\203 \320\262 \320\275\320\260\321\207\320\260\320\273\321\214\320\275\320\276\320\265 \320\277\320\276\320\273\320\276\320\266\320\265\320\275\320\270\320\265", 0));
#endif // QT_NO_STATUSTIP
        renderAction->setText(QApplication::translate("MainWindow", "RGB", 0));
#ifndef QT_NO_STATUSTIP
        renderAction->setStatusTip(QApplication::translate("MainWindow", "RGB \321\200\320\265\320\275\320\264\320\265\321\200\320\270\320\275\320\263 \321\201 \321\202\321\200\320\260\320\264\320\270\321\206\320\270\320\276\320\275\320\275\321\213\320\274\320\270 \320\270\321\201\321\202\320\276\321\207\320\275\320\270\320\272\320\260\320\274\320\270", 0));
#endif // QT_NO_STATUSTIP
        chooseViewAction->setText(QApplication::translate("MainWindow", "\320\240\320\265\320\266\320\270\320\274 \320\262\321\213\320\261\320\276\321\200\320\260 \321\200\320\260\320\272\321\203\321\200\321\201\320\260", 0));
#ifndef QT_NO_STATUSTIP
        chooseViewAction->setStatusTip(QApplication::translate("MainWindow", "\320\222\321\213\320\261\320\276\321\200 \321\200\320\260\320\272\321\203\321\200\321\201\320\260(\320\262\321\200\320\260\321\211\320\265\320\275\320\270\320\265 \321\201\320\260\320\274\320\276\320\271 \321\201\321\206\320\265\320\275\321\213)", 0));
#endif // QT_NO_STATUSTIP
        openHelpInfoAction->setText(QApplication::translate("MainWindow", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", 0));
#ifndef QT_NO_STATUSTIP
        openHelpInfoAction->setStatusTip(QApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\216 \320\276 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", 0));
#endif // QT_NO_STATUSTIP
        quitAction->setText(QApplication::translate("MainWindow", "\320\222\321\213\320\271\321\202\320\270", 0));
#ifndef QT_NO_STATUSTIP
        quitAction->setStatusTip(QApplication::translate("MainWindow", "\320\222\321\213\320\271\321\202\320\270 \320\270\320\267 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\321\213", 0));
#endif // QT_NO_STATUSTIP
        openSceneFileAction->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214 \321\204\320\260\320\271\320\273 \321\201\321\206\320\265\320\275\321\213", 0));
#ifndef QT_NO_STATUSTIP
        openSceneFileAction->setStatusTip(QApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214 \321\204\320\260\320\271\320\273 \321\201\321\206\320\265\320\275\321\213", 0));
#endif // QT_NO_STATUSTIP
        showCoordSystemAction->setText(QApplication::translate("MainWindow", "\320\237\320\276\320\272\320\260\320\267\320\260\321\202\321\214 \321\201\320\270\321\201\321\202\320\265\320\274\321\203 \320\272\320\276\320\276\321\200\320\264\320\270\320\275\320\260\321\202", 0));
#ifndef QT_NO_STATUSTIP
        showCoordSystemAction->setStatusTip(QApplication::translate("MainWindow", "\320\237\320\276\320\272\320\260\320\267\320\260\321\202\321\214 \320\274\320\270\321\200\320\276\320\262\321\203\321\216 \321\201\320\270\321\201\321\202\320\265\320\274\321\203 \320\272\320\276\320\276\321\200\320\264\320\270\320\275\320\260\321\202", 0));
#endif // QT_NO_STATUSTIP
        spectrumRenderAction->setText(QApplication::translate("MainWindow", "\320\241\320\277\320\265\320\272\321\202\321\200\320\260\320\273\321\214\320\275\321\213\320\271", 0));
#ifndef QT_NO_STATUSTIP
        spectrumRenderAction->setStatusTip(QApplication::translate("MainWindow", "\320\241\320\277\320\265\320\272\321\202\321\200\320\260\320\273\321\214\320\275\321\213\320\271 \321\200\320\265\320\275\320\264\320\265\321\200\320\270\320\275\320\263 \321\201 \321\202\321\200\320\260\320\264\320\270\321\206\320\270\320\276\320\275\320\275\321\213\320\274\320\270\320\274 \320\270\321\201\321\202\320\276\321\207\320\275\320\270\320\272\320\260\320\274\320\270", 0));
#endif // QT_NO_STATUSTIP
        polarizatiomRenderAction->setText(QApplication::translate("MainWindow", "C \320\277\320\276\320\273\321\217\321\200\320\270\320\267\320\260\321\206\320\270\320\265\320\271", 0));
#ifndef QT_NO_STATUSTIP
        polarizatiomRenderAction->setStatusTip(QApplication::translate("MainWindow", "\320\240\320\265\320\275\320\264\320\265\321\200\320\270\320\275\320\263 \321\201 \320\277\320\276\320\273\321\217\321\200\320\270\320\267\320\260\321\206\320\270\320\265\320\271 \321\201 \320\277\320\273\320\276\321\201\320\272\320\270\320\274 \320\270\321\201\321\202\320\276\321\207\320\275\320\270\320\272\320\276\320\274", 0));
#endif // QT_NO_STATUSTIP
        dispersionRenderSurfaceAction->setText(QApplication::translate("MainWindow", "\320\224\320\270\321\201\320\277\320\265\321\200\321\201\320\270\321\217", 0));
#ifndef QT_NO_STATUSTIP
        dispersionRenderSurfaceAction->setStatusTip(QApplication::translate("MainWindow", "\320\241\320\277\320\265\320\272\321\202\321\200\320\260\320\273\321\214\320\275\321\213\320\271 \321\200\320\265\320\275\320\264\320\265\321\200\320\270\320\275\320\263 \321\201 \320\277\320\273\320\276\321\201\320\272\320\270\320\274 \320\270\321\201\321\202\320\276\321\207\320\275\320\270\320\272\320\276\320\274(\320\264\320\270\321\201\320\277\320\265\321\200\321\201\320\270\321\217)", 0));
#endif // QT_NO_STATUSTIP
        openCommandsFileAction->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214 \320\272\320\276\320\274\320\260\320\275\320\264\320\275\321\213\320\271 \321\204\320\260\320\271\320\273", 0));
#ifndef QT_NO_STATUSTIP
        openCommandsFileAction->setStatusTip(QApplication::translate("MainWindow", "\320\232\320\276\320\274\320\260\320\275\320\264\320\275\321\213\320\271 \321\200\320\265\320\266\320\270\320\274", 0));
#endif // QT_NO_STATUSTIP
        polarizationTransferAction->setText(QApplication::translate("MainWindow", "\320\237\320\265\321\200\320\265\320\275\320\276\321\201 \320\277\320\276\320\273\321\217\321\200\320\270\320\267\320\260\321\206\320\270\320\270", 0));
        fileMenu->setTitle(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", 0));
        editMenu->setTitle(QApplication::translate("MainWindow", "\320\237\321\200\320\260\320\262\320\272\320\260", 0));
        helpMenu->setTitle(QApplication::translate("MainWindow", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260", 0));
        menu->setTitle(QApplication::translate("MainWindow", "\320\236\321\202\321\200\320\270\321\201\320\276\320\262\320\272\320\260", 0));
        menu_2->setTitle(QApplication::translate("MainWindow", "\320\224\320\270\320\267\320\260\320\271\320\275", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOWFORM_H
