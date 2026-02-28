#include "concrete_loggers.h"
#include <QTextStream>

// -------- FileLogger --------
FileLogger::FileLogger(const QString& filePath)
    : m_filePath(filePath)
{
}

void FileLogger::log(const QString& message, LogLevel level)
{
    if (!m_enabled) return;

    QFile file(m_filePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        QString time = QDateTime::currentDateTime()
                           .toString("yyyy-MM-dd hh:mm:ss");

        out << time << " "
            << levelToString(level) << " "
            << message << "\n";
    }
}

QString FileLogger::getName() const { return "File Logger"; }
bool FileLogger::isEnabled() const { return m_enabled; }
void FileLogger::setEnabled(bool enabled) { m_enabled = enabled; }

// -------- ConsoleLogger --------
void ConsoleLogger::log(const QString& message, LogLevel level)
{
    if (!m_enabled) return;

    QString time = QDateTime::currentDateTime()
                       .toString("hh:mm:ss");

    qDebug() << time
             << levelToString(level)
             << message;
}

QString ConsoleLogger::getName() const { return "Console Logger"; }
bool ConsoleLogger::isEnabled() const { return m_enabled; }
void ConsoleLogger::setEnabled(bool enabled) { m_enabled = enabled; }

// -------- GuiLogger --------
GuiLogger::GuiLogger(QTextEdit* textEdit)
    : m_textEdit(textEdit)
{
}

void GuiLogger::log(const QString& message, LogLevel level)
{
    if (!m_enabled || !m_textEdit) return;

    QString time = QDateTime::currentDateTime()
                       .toString("hh:mm:ss");

    QString text = time + " "
                   + levelToString(level)
                   + " " + message;

    if (level == LogLevel::Error)
        m_textEdit->append("<font color='red'>" + text + "</font>");
    else if (level == LogLevel::Warning)
        m_textEdit->append("<font color='orange'>" + text + "</font>");
    else
        m_textEdit->append(text);
}

QString GuiLogger::getName() const { return "GUI Logger"; }
bool GuiLogger::isEnabled() const { return m_enabled; }
void GuiLogger::setEnabled(bool enabled) { m_enabled = enabled; }

// -------- NetworkLogger --------
void NetworkLogger::log(const QString& message, LogLevel level)
{
    if (!m_enabled) return;

    qDebug() << "[NETWORK]"
             << levelToString(level)
             << message;
}

QString NetworkLogger::getName() const { return "Network Logger"; }
bool NetworkLogger::isEnabled() const { return m_enabled; }
void NetworkLogger::setEnabled(bool enabled) { m_enabled = enabled; }
