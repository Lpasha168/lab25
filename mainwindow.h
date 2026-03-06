#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <vector>
#include <memory>
#include "logger.h"

class QTextEdit;
class QLineEdit;
class QComboBox;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addLogger();
    void removeLogger();
    void generateMessage();
    void createCompositeGroup();
    void saveConfig();
    void loadConfig();

private:
    void setupUi();
    void refreshLoggerList();

    // Используем shared_ptr для логгеров, чтобы они могли быть в нескольких местах
    std::vector<std::shared_ptr<Logger>> m_loggers;

    // Композитные логгеры также хранятся как shared_ptr
    std::vector<std::shared_ptr<CompositeLogger>> m_compositeGroups;

    QTextEdit* m_logDisplay;
    QLineEdit* m_messageInput;
    QComboBox* m_levelCombo;
    QListWidget* m_loggerList;
    QPushButton* m_createCompositeBtn;
};

#endif 


