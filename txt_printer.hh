#ifndef REMOTEE_TXT_PRINTER_HH
#define REMOTEE_TXT_PRINTER_HH 1

#include "remotes.hh"

class TxtPrinter
{
public:
    TxtPrinter() = default;

    void print(std::ostream &, const IDRemoteMap & i_r_m);
};

#endif
