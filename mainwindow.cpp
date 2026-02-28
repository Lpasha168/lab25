#include "mainwindow.h"
#include "concrete_loggers.h"
#include <memory>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSettings>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    loadConfig();
}

MainWindow::~MainWindow()
{
    saveConfig();
}

void MainWindow::setupUi()
{
    resize(600, 500);

    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);

    m_logDisplay = new QTextEdit(this);
    m_logDisplay->setReadOnly(true);

    m_loggerList = new QListWidget(this);

    m_messageInput = new QLineEdit("Test message", this);

    m_levelCombo = new QComboBox(this);
    m_levelCombo->addItems({"Info", "Warning", "Error"});

    QPushButton* addBtn = new QPushButton("Add Logger", this);
    QPushButton* removeBtn = new QPushButton("Remove Logger", this);
    QPushButton* genBtn = new QPushButton("Generate Log", this);

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addLogger);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::removeLogger);
    connect(genBtn, &QPushButton::clicked, this, &MainWindow::generateMessage);

    layout->addWidget(m_loggerList);
    layout->addWidget(addBtn);
    layout->addWidget(removeBtn);
    layout->addWidget(m_logDisplay);
    layout->addWidget(m_messageInput);
    layout->addWidget(m_levelCombo);
    layout->addWidget(genBtn);

    setCentralWidget(central);
}

void MainWindow::addLogger()
{
    QStringList types = {"File", "Console", "GUI", "Network"};
    bool ok;
    QString type = QInputDialog::getItem(this, "Add Logger",
                                         "Select type:", types, 0, false, &ok);
    if (!ok) return;

    if (type == "File")
        m_loggers.push_back(std::make_unique<FileLogger>("application.log"));
    else if (type == "Console")
        m_loggers.push_back(std::make_unique<ConsoleLogger>());
    else if (type == "GUI")
        m_loggers.push_back(std::make_unique<GuiLogger>(m_logDisplay));
    else if (type == "Network")
        m_loggers.push_back(std::make_unique<NetworkLogger>());

    refreshLoggerList();
}

void MainWindow::removeLogger()
{
    int row = m_loggerList->currentRow();
    if (row < 0) return;

    m_loggers.erase(m_loggers.begin() + row);
    refreshLoggerList();
}

void MainWindow::refreshLoggerList()
{
    m_loggerList->clear();
    for (const auto& logger : m_loggers)
        m_loggerList->addItem(logger->getName());
}

void MainWindow::generateMessage()
{
    QString text = m_messageInput->text();

    LogLevel level = LogLevel::Info;
    if (m_levelCombo->currentIndex() == 1)
        level = LogLevel::Warning;
    else if (m_levelCombo->currentIndex() == 2)
        level = LogLevel::Error;

    for (const auto& logger : m_loggers)
        logger->log(text, level);
}

void MainWindow::saveConfig()
{
    QSettings settings("MyCompany", "LogSystem");

    settings.beginWriteArray("loggers");
    for (int i = 0; i < m_loggers.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("type", m_loggers[i]->getName());
    }
    settings.endArray();
}

void MainWindow::loadConfig()
{
    QSettings settings("MyCompany", "LogSystem");

    int size = settings.beginReadArray("loggers");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString type = settings.value("type").toString();

        if (type == "File Logger")
            m_loggers.push_back(std::make_unique<FileLogger>("application.log"));
        else if (type == "Console Logger")
            m_loggers.push_back(std::make_unique<ConsoleLogger>());
        else if (type == "GUI Logger")
            m_loggers.push_back(std::make_unique<GuiLogger>(m_logDisplay));
        else if (type == "Network Logger")
            m_loggers.push_back(std::make_unique<NetworkLogger>());
    }
    settings.endArray();

    refreshLoggerList();
}
