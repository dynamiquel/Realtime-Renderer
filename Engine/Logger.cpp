#include "Logger.h"
#include <comdef.h>
#include "StringHelper.h"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

void Logger::Initialise()
{
    // Creates a Logger from the spdlog library: https://github.com/gabime/spdlog.
    // Filename includes the time the log was created (now).
    std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt("FileLogger", CreateFileNameForNewLog());
    
    logger->set_level(spdlog::level::debug);
    logger->info("Start of Log Session!\n");
}

void Logger::Log(std::string message)
{
    Log(LogSource::None, LogType::Log, message);
}

void Logger::Log(LogType logType, std::string message)
{
    Log(LogSource::None, logType, message);
}

void Logger::Log(LogSource logSource, LogType logType, std::string message)
{
    AffixLogSource(logSource, &message);

    std::shared_ptr<spdlog::logger> logger = spdlog::get("FileLogger");
    
    switch (logType)
    {
        default:
        case LogType::Log:
            logger->info(message);
            break;
        case LogType::Warning:
            logger->warn(message);
            break;
        case LogType::Error:
            logger->error(message);
            break;
    }

    // Writes the new messages from the logger to the log file.
    logger->flush();
}


void Logger::Log(HRESULT hr, std::string message)
{
    _com_error error(hr);
    std::wstring error_message = L"Error: " + StringHelper::StringToWide(message) + L"\n" + error.ErrorMessage();
    MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}

void Logger::Log(HRESULT hr, std::wstring message)
{
    _com_error error(hr);
    std::wstring error_message = L"Error: " + message + L"\n" + error.ErrorMessage();
    MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}

void Logger::Flush()
{
    auto logger = spdlog::get("FileLogger");
    logger->flush();
}

std::string Logger::CreateFileNameForNewLog()
{
    // Gets the current time.
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    const auto nowString = StringHelper::ToString(now);
    
    // Creates the file name for the log file.
    const auto logFileLocation = "logs/" + nowString + ".log";

    return logFileLocation;
}

void Logger::AffixLogSource(LogSource logSource, std::string* message)
{
    switch (logSource)
    {
        default:
        case LogSource::None:
            *message = "[None] " + *message;
            break;
        case LogSource::Camera:
            *message = "[Camera] " + *message;
            break;
        case LogSource::Input:
            *message = "[Input] " + *message;
            break;
        case LogSource::Lighting:
            *message = "[Lighting] " + *message;
            break;
        case LogSource::Scene:
            *message = "[Scene] " + *message;
            break;
        case LogSource::Rendering:
            *message = "[Rendering] " + *message;
            break;
        case LogSource::Application:
            *message = "[Application] " + *message;
            break;
        case LogSource::Models:
            *message = "[Models] " + *message;
            break;
    }
}

void Logger::AffixLogSource(LogSource logSource, std::wstring* message)
{
    switch (logSource)
    {
    default:
    case LogSource::None:
        *message = L"[None] " + *message;
        break;
    case LogSource::Camera:
        *message = L"[Camera] " + *message;
        break;
    case LogSource::Input:
        *message = L"[Input] " + *message;
        break;
    case LogSource::Lighting:
        *message = L"[Lighting] " + *message;
        break;
    case LogSource::Scene:
        *message = L"[Scene] " + *message;
        break;
    case LogSource::Rendering:
        *message = L"[Rendering] " + *message;
        break;
    case LogSource::Application:
        *message = L"[Application] " + *message;
        break;
    case LogSource::Models:
        *message = L"[Models] " + *message;
        break;
    }
}

