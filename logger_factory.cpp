#include "logger_factory.h"
#include "concrete_loggers.h"

std::unique_ptr<Logger> LoggerFactory::createLogger(LoggerType type, const QString& param, QTextEdit* textEdit) {
    switch (type) {
    case LoggerType::File:
        // std::make_unique - правильный и безопасный способ создания умного указателя
        return std::make_unique<FileLogger>(param);
    case LoggerType::Console:
        return std::make_unique<ConsoleLogger>();
    case LoggerType::Gui:
        return std::make_unique<GuiLogger>(textEdit);
    case LoggerType::Network:
        return std::make_unique<NetworkLogger>();
    default:
        return nullptr;
    }
}
