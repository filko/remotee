#include "command_line.hh"
#include <paludis/paludis.hh>
#include <cstdlib>
#include <iostream>

using namespace paludis;

using std::cout;
using std::endl;

CommandLine::CommandLine() :
    action_args(main_options_section(), "Actions",
            "Selects which basic action to perform. At most one action should "
            "be specified."),
    a_help(&action_args,           "help",           'h', "Display program help", false),
    a_dump_collected(&action_args, "dump-collected", 'D', "Dump collected remotes.  Disables processing them.", false),
    a_only_better(&action_args,    "only-better",    'B', "Only report ids that have better remote versions.", false),
    a_cache_path(&action_args,     "cache-path",     'C', "Cache path", false),
    a_cache_expire(&action_args,   "cache-expire",   'c', "How long cache will be valid, in seconds.  Default: 23h"),

    general_args(main_options_section(), "General options",
            "Options which are relevant for most or all actions."),
    a_log_level(&general_args, "log-level",  'L'),
    a_environment(&general_args, "environment", 'E', "Environment specification (class:suffix, both parts optional)")
{
    a_cache_expire.set_argument(23 * 60 * 60);
}

std::string
CommandLine::app_name() const
{
    return "remotee";
}

std::string
CommandLine::app_synopsis() const
{
    return "Remote IDs client for Paludis";
}

std::string
CommandLine::app_description() const
{
    return "";
}

CommandLine::~CommandLine()
{
}

void
show_help_and_exit(const char * const argv[])
{
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << endl;
    cout << *CommandLine::get_instance();

    std::exit(EXIT_SUCCESS);
}

void
CommandLine::run(const int argc, const char * const * const argv, const std::string & client,
        const std::string & env_var, const std::string & env_prefix)
{
    args::ArgsHandler::run(argc, argv, client, env_var, env_prefix);

    if (CommandLine::get_instance()->a_log_level.specified())
        Log::get_instance()->set_log_level(CommandLine::get_instance()->a_log_level.option());

    if (CommandLine::get_instance()->a_help.specified())
        show_help_and_exit(argv);
}

namespace paludis
{
    template class Singleton<CommandLine>;
}
