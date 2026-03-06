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
    Network,
    Composite
};

class LoggerFactory {
public:
    // Статический метод-фабрика. Возвращает умный указатель (shared_ptr)
    static std::shared_ptr<Logger> createLogger(LoggerType type,const QString& param = "",QTextEdit* textEdit = nullptr);
    // Создает композитный логгер с предустановленными логгерами
    static std::shared_ptr<CompositeLogger> createCompositeLogger(
        const std::vector<std::shared_ptr<Logger>>& loggers = {});
};

#endif // LOGGER_FACTORY_H
