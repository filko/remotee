#include "txt_printer.hh"

#include <iomanip>
#include <paludis/version_spec.hh>
#include <paludis/metadata_key.hh>
#include <paludis/name.hh>
#include <paludis/slot.hh>
#include <paludis/util/stringify.hh>
#include <vector>

using std::endl;

namespace
{

struct Row
{
    std::vector<std::string> content;
    bool begin;

    Row()
        : begin(true)
    {
    }

    struct tab_t { };
    static const tab_t tab;

    template <typename T>
    Row & operator<<(const T & t)
    {
        if (begin)
        {
            content.push_back(paludis::stringify(t));
            begin = false;
        }
        else
            content.back().append(paludis::stringify(t));

        return *this;
    }

    Row & operator<<(const tab_t)
    {
        if (begin)
            content.push_back("");
        else
            begin = true;

        return *this;
    }
};

struct ColumnedText
{
    std::vector<Row> content;

    Row & next()
    {
        content.push_back(Row());
        return content.back();
    }

    void print(std::ostream & os)
    {
        std::vector<std::size_t> sizes;
        for (const auto & r : content)
        {
            if (sizes.size() < r.content.size())
                sizes.resize(r.content.size());

            std::size_t count(0);
            for (auto c(r.content.begin()), c_end(r.content.end());
                 c != c_end; ++c, ++count)
            {
                if (sizes[count] < c->size())
                    sizes[count] = c->size();
            }
        }

        for (const auto & r : content)
        {
            std::size_t count(0);
            for (auto c(r.content.begin()), c_end(r.content.end());
                 c != c_end; ++c, ++count)
            {
                os << std::left << std::setw(sizes[count]) << *c << ' ';
            }
            os << endl;
        }
    }
};

}

void TxtPrinter::print(std::ostream & os, const IDRemoteMap & i_r_m)
{
    ColumnedText columns;
    for (auto i(i_r_m.begin()), i_end(i_r_m.end());
         i != i_end; ++i)
    {
        Row & row(columns.next());
        row << i->first->name() << ':' << i->first->slot_key()->parse_value().raw_value() << Row::tab;
        row << "::" << i->first->repository_name() << Row::tab << i->first->version() << Row::tab;
 
        for (auto r(i->second.begin()), r_end(i->second.end());
             r != r_end; ++r)
        {
            row << r->first << Row::tab << r->second;
            if (r->second > i->first->version())
                row << '*';

            row << Row::tab;
        }
    }

    columns.print(os);
}
