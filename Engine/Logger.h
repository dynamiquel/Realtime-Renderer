#pragma once
#include <memory>
#include <string>
#include <Windows.h>

#include "spdlog/logger.h"

enum class LogType : UINT8
{
    Log,
    Warning,
    Error
};

enum class LogSource : UINT8
{
    None,
    Camera,
    Input,
    Lighting,
    Scene,
    Rendering,
    Application,
    Models,
    Shaders,
    Assets
};

/**
 * \brief Responsible for logging engine-level messages.
 */
class Logger
{
public:
    static void Initialise();
    
    static void Log(std::string message);
    static void Log(LogType logType, std::string message);
    
    /**
     * \brief Logs a message to the logfile.
     * \param logSource Source of log (Example: Input or Rendering).
     * \param logType Type of log (Example: Warning or Error).
     * \param message Message to log.
     */
    static void Log(LogSource logSource, LogType logType, std::string message);

    /**
     * \brief Message pop-up log.
     * \param message Message to log.
     */
    static void Log(HRESULT hr, std::string message);
    static void Log(HRESULT hr, std::wstring message);

    /**
     * \brief Manually flush the log to the logfile.
     */
    static void Flush();

private:
    // Generates a filename for the new log file.
    static std::string CreateFileNameForNewLog();
    
    // Includes the LogSource into the message.
    static void AffixLogSource(LogSource logSource, std::string* message);
    static void AffixLogSource(LogSource logSource, std::wstring* message);
};