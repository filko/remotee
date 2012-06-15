#ifndef REMOTEE_STATUS_PRINTER_HH
#define REMOTEE_STATUS_PRINTER_HH 1

#include <paludis/util/pimp.hh>
#include <string>

class StatusPrinter
{
    paludis::Pimp<StatusPrinter> imp_;

public:
    StatusPrinter(std::size_t total);
    ~StatusPrinter();

    void current_engine(const std::string & engine, std::size_t size);
    void done(std::size_t n = 1);
};

#endif
