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
    void saveConfig();
    void loadConfig();

private:
    void setupUi();
    void refreshLoggerList();

    std::vector<std::unique_ptr<Logger>> m_loggers;

    QTextEdit* m_logDisplay;
    QLineEdit* m_messageInput;
    QComboBox* m_levelCombo;
    QListWidget* m_loggerList;
};

#endif
