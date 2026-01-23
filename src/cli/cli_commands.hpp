#ifndef CLI_COMMANDS_HPP
#define CLI_COMMANDS_HPP

#include <string>
#include <vector>

namespace RawrXD {
namespace CLI {

/**
 * @brief CLI command result
 */
struct CommandResult {
    bool success;
    std::string message;
    int exitCode;
    
    static CommandResult ok(const std::string& msg = "") {
        return CommandResult{true, msg, 0};
    }
    
    static CommandResult error(const std::string& msg, int code = 1) {
        return CommandResult{false, msg, code};
    }
};

/**
 * @brief CLI command handler
 */
class CommandHandler {
public:
    static CommandResult help();
    static CommandResult version();
    static CommandResult loadModel(const std::string& path);
    static CommandResult applyPatch(const std::string& patchFile);
    static CommandResult listDevices();
    static CommandResult info();
};

/**
 * @brief Argument parser
 */
class ArgumentParser {
public:
    ArgumentParser(int argc, char** argv);
    
    bool hasFlag(const std::string& flag) const;
    std::string getValue(const std::string& key, const std::string& defaultValue = "") const;
    std::vector<std::string> getPositionalArgs() const;
    
    std::string getCommand() const;

private:
    std::vector<std::string> m_args;
    std::string m_command;
};

} // namespace CLI
} // namespace RawrXD

#endif // CLI_COMMANDS_HPP
