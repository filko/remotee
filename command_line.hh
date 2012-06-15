#ifndef PRINT_DEPS_COMMAND_LINE
#define PRINT_DEPS_COMMAND_LINE

#include <paludis/args/args.hh>
#include <paludis/args/log_level_arg.hh>
#include <paludis/paludis.hh>

class CommandLine :
        public paludis::args::ArgsHandler,
        public paludis::Singleton<CommandLine>
{
    friend class paludis::Singleton<CommandLine>;

private:
    CommandLine();
    ~CommandLine();

public:
    virtual void run(const int, const char * const * const,
                     const std::string & client, const std::string & env_var,
                     const std::string & env_prefix);

    virtual std::string app_name() const;
    virtual std::string app_synopsis() const;
    virtual std::string app_description() const;

    paludis::args::ArgsGroup action_args;
    paludis::args::SwitchArg a_help;
    paludis::args::SwitchArg a_dump_collected;
    paludis::args::SwitchArg a_only_better;
    paludis::args::StringArg a_cache_path;
    paludis::args::IntegerArg a_cache_expire;

    paludis::args::ArgsGroup general_args;
    paludis::args::LogLevelArg a_log_level;
    paludis::args::StringArg a_environment;
};

void show_help_and_exit(const char * const argv[]);

#endif
