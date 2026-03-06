#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QMetaType>
#include <memory>

enum class LogLevel {
    Info,
    Warning,
    Error
};

Q_DECLARE_METATYPE(LogLevel)

class Logger {
public:
    virtual ~Logger() = default;
    virtual void log(const QString& message, LogLevel level) = 0;
    virtual QString getName() const = 0;
    virtual bool isEnabled() const = 0;
    virtual void setEnabled(bool enabled) = 0;
};

// Композитный логгер для группового логирования
class CompositeLogger : public Logger {
public:
    void addLogger(std::shared_ptr<Logger> logger);
    void removeLogger(const QString& loggerName);
    void log(const QString& message, LogLevel level) override;
    QString getName() const override;
    bool isEnabled() const override;
    void setEnabled(bool enabled) override;
    size_t getLoggerCount() const;

private:
    std::vector<std::shared_ptr<Logger>> m_loggers;
    bool m_enabled = true;
};

#endif 

