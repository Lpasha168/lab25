#ifndef LOGGER_FACTORY_H
#define LOGGER_FACTORY_H

#include "logger.h"
#include <memory>
#include <QTextEdit>

// Типы логгеров, которые мы умеем создавать
enum class LoggerType {
    File,
    Console,
    Gui,
    Network
};

class LoggerFactory {
public:
    // Статический метод-фабрика. Возвращает умный указатель (unique_ptr)
    static std::unique_ptr<Logger> createLogger(LoggerType type,
                                                const QString& param = "",
                                                QTextEdit* textEdit = nullptr);
};

#endif // LOGGER_FACTORY_H
