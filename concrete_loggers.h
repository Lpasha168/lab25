#ifndef CONCRETE_LOGGERS_H
#define CONCRETE_LOGGERS_H

#include "logger.h"
#include <QFile>
#include <QTextEdit>
#include <QDebug>
#include <QDateTime>

// Преобразование уровня в текст
inline QString levelToString(LogLevel level)
{
    switch (level) {
    case LogLevel::Info: return "[INFO]";
    case LogLevel::Warning: return "[WARNING]";
    case LogLevel::Error: return "[ERROR]";
    }
    return "[UNKNOWN]";
}

// ---------- FileLogger ----------
class FileLogger : public Logger {
public:
    explicit FileLogger(const QString& filePath);

    void log(const QString& message, LogLevel level) override;
    QString getName() const override;
    bool isEnabled() const override;
    void setEnabled(bool enabled) override;

private:
    QString m_filePath;
    bool m_enabled = true;
};

// ---------- ConsoleLogger ----------
class ConsoleLogger : public Logger {
public:
    void log(const QString& message, LogLevel level) override;
    QString getName() const override;
    bool isEnabled() const override;
    void setEnabled(bool enabled) override;

private:
    bool m_enabled = true;
};

// ---------- GuiLogger ----------
class GuiLogger : public Logger {
public:
    explicit GuiLogger(QTextEdit* textEdit);

    void log(const QString& message, LogLevel level) override;
    QString getName() const override;
    bool isEnabled() const override;
    void setEnabled(bool enabled) override;

private:
    QTextEdit* m_textEdit;
    bool m_enabled = true;
};

// ---------- NetworkLogger ----------
class NetworkLogger : public Logger {
public:
    void log(const QString& message, LogLevel level) override;
    QString getName() const override;
    bool isEnabled() const override;
    void setEnabled(bool enabled) override;

private:
    bool m_enabled = true;
};

#endif
