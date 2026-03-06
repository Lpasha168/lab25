#include "mainwindow.h"

#include "logger_factory.h"
#include <memory>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSettings>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    loadConfig();
}

MainWindow::~MainWindow()
{
    saveConfig();
    // Нет необходимости вручную удалять логгеры - shared_ptr сделает это автоматически
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
    m_createCompositeBtn = new QPushButton("Create Composite Group", this);

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addLogger);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::removeLogger);
    connect(genBtn, &QPushButton::clicked, this, &MainWindow::generateMessage);
    connect(m_createCompositeBtn, &QPushButton::clicked, this, &MainWindow::createCompositeGroup);

    layout->addWidget(m_loggerList);
    layout->addWidget(addBtn);
    layout->addWidget(removeBtn);
    layout->addWidget(m_createCompositeBtn);
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

    std::shared_ptr<Logger> logger;

    if (type == "File")
        logger = LoggerFactory::createLogger(LoggerType::File, "application.log");
    else if (type == "Console")
        logger = LoggerFactory::createLogger(LoggerType::Console);
    else if (type == "GUI")
        logger = LoggerFactory::createLogger(LoggerType::Gui, "", m_logDisplay);
    else if (type == "Network")
        logger = LoggerFactory::createLogger(LoggerType::Network);

    if (logger) {
        m_loggers.push_back(logger);
    }

    refreshLoggerList();
}

void MainWindow::removeLogger()
{
    int row = m_loggerList->currentRow();
    if (row < 0) return;

    // Просто удаляем из вектора - память освободится автоматически
    m_loggers.erase(m_loggers.begin() + row);
    refreshLoggerList();
}

void MainWindow::createCompositeGroup()
{
    if (m_loggers.empty()) {
        QMessageBox::information(this, "Info", "No loggers to group");
        return;
    }

    // Создаем композитный логгер из всех существующих логгеров
    auto composite = LoggerFactory::createCompositeLogger(m_loggers);

    // Добавляем композитный логгер как новый логгер
    m_loggers.push_back(composite);
    m_compositeGroups.push_back(composite);

    refreshLoggerList();

    QMessageBox::information(this, "Success",
                             "Composite group created with " + QString::number(composite->getLoggerCount()) + " loggers");
}

void MainWindow::refreshLoggerList()
{
    m_loggerList->clear();
    for (const auto& logger : m_loggers) {
        if (logger) {
            m_loggerList->addItem(logger->getName());
        }
    }
}

void MainWindow::generateMessage()
{
    QString text = m_messageInput->text();

    LogLevel level = LogLevel::Info;
    if (m_levelCombo->currentIndex() == 1)
        level = LogLevel::Warning;
    else if (m_levelCombo->currentIndex() == 2)
        level = LogLevel::Error;

    // Логируем сообщение через все логгеры
    for (const auto& logger : m_loggers) {
        if (logger && logger->isEnabled()) {
            logger->log(text, level);
        }
    }
}

void MainWindow::saveConfig()
{
    QSettings settings("MyCompany", "LogSystem");

    settings.beginWriteArray("loggers");
    for (size_t i = 0; i < m_loggers.size(); ++i) {
        settings.setArrayIndex(static_cast<int>(i));
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

        std::shared_ptr<Logger> logger;

        if (type == "File Logger")
            logger = LoggerFactory::createLogger(LoggerType::File, "application.log");
        else if (type == "Console Logger")
            logger = LoggerFactory::createLogger(LoggerType::Console);
        else if (type == "GUI Logger")
            logger = LoggerFactory::createLogger(LoggerType::Gui, "", m_logDisplay);
        else if (type == "Network Logger")
            logger = LoggerFactory::createLogger(LoggerType::Network);
        else if (type.startsWith("Composite Logger"))
            logger = LoggerFactory::createLogger(LoggerType::Composite);

        if (logger) {
            m_loggers.push_back(logger);
        }
    }
    settings.endArray();

    refreshLoggerList();
}
