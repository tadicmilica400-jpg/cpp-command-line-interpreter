#include "CommandBuilder.h"
#include "Command.h"
#include "EchoCommand.h"
#include "TimeCommand.h"
#include "DateCommand.h"
#include "TouchCommand.h"
#include "WCCommand.h"
#include "PromptCommand.h"
#include "TruncateCommand.h"
#include "RmCommand.h"
#include "TrCommand.h"
#include "HeadCommand.h"
#include "PipeCommand.h"
#include "BatchCommand.h"

#include <vector>
#include <string>
#include<iostream>
#include <sstream>

// Pravi odgovarajucu komandu na osnovu ParsedCommand strukture koja joj prosledi sve potrebne informacije
std::unique_ptr<Command> CommandBuilder::create(const ParsedCommand& pcmd) {
    // Proveri da li je komanda prazna
    if (pcmd.name.empty())
        throw std::runtime_error("No command specified");

    const std::string& cmdName = pcmd.name;

    std::unique_ptr<Command> cmd = nullptr;

    // Pravi odgovarajucu komandu na osnovu imena
    if (cmdName == "echo") {
        cmd = std::make_unique<EchoCommand>(pcmd.args);
    }

    else if (cmdName == "time") {
        cmd = std::make_unique<TimeCommand>();
    }

    else if (cmdName == "date") {
        cmd = std::make_unique<DateCommand>();
    }

    else if (cmdName == "touch") {
        cmd = std::make_unique<TouchCommand>(pcmd.args[0]);
    }

    else if (cmdName == "wc") {
        std::string opt = pcmd.options.empty() ? "" : pcmd.options[0];
        WCCommand::Mode mode = WCCommand::Mode::WORDS;
        if (opt == "-c") {
            mode = WCCommand::Mode::CHARS;
        }
        cmd = std::make_unique<WCCommand>(mode, pcmd);
    }

    else if (cmdName == "prompt") {
        std::string newprompt;
        newprompt = pcmd.args.empty() ? "" : pcmd.args[0];
        newprompt.push_back(' ');
        Error::checkArgumentsCount("prompt", pcmd.args, 1);
        cmd = std::make_unique<PromptCommand>(newprompt);
    }

    else if (cmdName == "truncate") {
        cmd = std::make_unique<TruncateCommand>(pcmd.args[0]);
    }

    else if (cmdName == "rm") {
        cmd = std::make_unique<RmCommand>(pcmd.args[0]);
    }

    else if (cmdName == "tr") {
        cmd = std::make_unique<TrCommand>(pcmd);
    }

    else if (cmdName == "head") {
        if (pcmd.options.empty()) {
            throw std::runtime_error("head: missing option -n<count>");
        }

        const std::string& opt = pcmd.options[0];

        if (opt.rfind("-n", 0) != 0) { // mora da počne sa -n
            throw std::runtime_error("head: invalid option, expected -n<count>");
        }

        std::string numStr = opt.substr(2); // uzmi deo posle -n

        if (numStr.empty() || numStr.size() > 5) {
            throw std::runtime_error("head: invalid line count");
        }

        for (char c : numStr) {
            if (!isdigit(c)) {
                throw std::runtime_error("head: invalid number in -n option");
            }
        }

        int n = std::stoi(numStr);
        cmd = std::make_unique<HeadCommand>(n); // prosledi i ParsedCommand
    }

    else if (cmdName == "batch") {
        cmd = std::make_unique<BatchCommand>(std::move(pcmd));
    }

    else if (cmdName == "pipe") {
        std::vector<std::unique_ptr<Command>> subcmds;
        for (auto& sub : pcmd.subcommands) {
            subcmds.push_back(CommandBuilder::create(std::move(sub)));
        }

        // uzmi inputFile od prve, outputFile/append od poslednje
        std::string inputFile = pcmd.subcommands.front().inputFile;
        std::string outputFile = pcmd.subcommands.back().outputFile;
        bool append = pcmd.subcommands.back().append;

        cmd = std::make_unique<PipeCommand>(
            std::move(subcmds), inputFile, outputFile, append
        );
    }

    else {
        return nullptr; // Nepoznata komanda    
    }
    cmd->setInputFile(pcmd.inputFile);
    cmd->setOutputFile(pcmd.outputFile);
    return cmd;

}