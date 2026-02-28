#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QMetaType>

// Уровни логирования
enum class LogLevel {
    Info,
    Warning,
    Error
};

Q_DECLARE_METATYPE(LogLevel)

// Базовый интерфейс
class Logger {
public:
    virtual ~Logger() = default;

    virtual void log(const QString& message, LogLevel level) = 0;
    virtual QString getName() const = 0;
    virtual bool isEnabled() const = 0;
    virtual void setEnabled(bool enabled) = 0;
};

#endif
