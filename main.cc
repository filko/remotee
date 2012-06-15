#include "command_line.hh"

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "cache.hh"
#include "get_remote_ids.hh"
#include "remotes.hh"
#include "txt_printer.hh"

using namespace paludis;
using std::cerr;
using std::endl;

int main(int arc, char * arv[])
{
    try
    {
        CommandLine::get_instance()
            ->run(arc, arv, "remotee", "REMOTEE_OPTIONS", "REMOTEE_CMDLINE");

        if (! CommandLine::get_instance()->a_cache_path.specified())
            throw std::runtime_error("--cache-path unspecified, aborting.");

        Cache::get_instance()->init(CommandLine::get_instance()->a_cache_path.argument(),
                                    CommandLine::get_instance()->a_cache_expire.argument());

        Log::get_instance()->set_program_name(arv[0]);
        Log::get_instance()->set_log_level(CommandLine::get_instance()->a_log_level.option());

        if (CommandLine::get_instance()->begin_parameters() == CommandLine::get_instance()->end_parameters())
            throw std::runtime_error("No targets specified, aborting.");

        auto env(EnvironmentFactory::get_instance()->create(CommandLine::get_instance()->a_environment.argument()));

        Remotes remotes;
        for (auto q(CommandLine::get_instance()->begin_parameters()),
                 q_end(CommandLine::get_instance()->end_parameters());
             q != q_end; ++q)
        {
            PackageDepSpec spec(parse_user_package_dep_spec(*q, env.get(), {updso_allow_wildcards}));

            std::shared_ptr<const PackageIDSequence> ids(
                (*env)[selection::BestVersionOnly(
                        generator::Matches(spec, make_null_shared_ptr(), { }) |
                        filter::Matches(parse_user_package_dep_spec("*/*[<scm]", env.get(), {updso_allow_wildcards}),
                                          make_null_shared_ptr(), { }))]);

            for (auto i(ids->begin()), i_end(ids->end());
                 i != i_end; ++i)
            {
                for (auto m((*i)->begin_metadata()), m_end((*i)->end_metadata());
                     m != m_end; ++m)
                    if ("REMOTE_IDS" == (*m)->raw_name())
                    {
                        paludis::Context ct("When checking REMOTE_IDS for '" + stringify(**i) + "'");
                        auto r(get_remote_ids(**m, env.get()));
                        std::for_each(r.begin(), r.end(),
                                      [&](decltype(*r.begin()) s)
                                      {
                                          remotes.insert(s.type, s.name, *i);
                                      });
                    }
            }
        }

        if (CommandLine::get_instance()->a_dump_collected.specified())
        {
            remotes.dump();
            return EXIT_SUCCESS;
        }

        auto r(remotes.perform(CommandLine::get_instance()->a_only_better.specified()));
        TxtPrinter printer;
        printer.print(std::cout, r);
    }
    catch (const Exception & e)
    {
        cerr << endl;
        cerr << "Unhandled exception:" << endl
            << e.backtrace("\n  * ")
            << e.message() << " (" << e.what() << ")" << endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception & e)
    {
        cerr << endl;
        cerr << "Unhandled exception:" << endl
            << "  * " << e.what() << endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        cerr << endl;
        cerr << "Unhandled exception:" << endl
            << "  * Unknown exception type. Ouch..." << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
