#include "logger_factory.h"
#include "concrete_loggers.h"
#include <memory>

std::shared_ptr<Logger> LoggerFactory::createLogger(LoggerType type, const QString& param, QTextEdit* textEdit)
{
    switch (type) {
    case LoggerType::File:
        return std::make_shared<FileLogger>(param);
    case LoggerType::Console:
        return std::make_shared<ConsoleLogger>();
    case LoggerType::Gui:
        return std::make_shared<GuiLogger>(textEdit);
    case LoggerType::Network:
        return std::make_shared<NetworkLogger>();
    case LoggerType::Composite:
        return std::make_shared<CompositeLogger>();
    default:
        return nullptr;
    }
}

std::shared_ptr<CompositeLogger> LoggerFactory::createCompositeLogger(const std::vector<std::shared_ptr<Logger>>& loggers)
{
    auto composite = std::make_shared<CompositeLogger>();
    for (const auto& logger : loggers) {
        composite->addLogger(logger);
    }
    return composite;
}
// LOGGER_FACTORY_H

