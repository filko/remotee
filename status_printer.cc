#include "status_printer.hh"

#include <iostream>
#include <paludis/util/pimp-impl.hh>
#include <sstream>

#include "cache.hh"

namespace paludis
{

template <>
struct Imp<StatusPrinter>
{
    const std::size_t total_;
    std::size_t count_;
    std::string engine_;
    std::size_t engine_size_;
    std::size_t engine_count_;
    std::size_t uncached_;
    std::string::size_type line_lenght_;

    Imp(std::size_t total)
        : total_(total),
          count_(0),
          engine_size_(0),
          engine_count_(0),
          uncached_(0),
          line_lenght_(0)
    {
        Cache::get_instance()->attach_uncached_observer([&]() { this->uncached(); });
    }

    ~Imp()
    {
        Cache::get_instance()->release_uncached_observer();
        std::cerr << std::endl;
    }

    void uncached()
    {
        ++uncached_;
        update();
    }

    void done(std::size_t n)
    {
        count_ += n;
        engine_count_ += n;
        update();
    }

    void update()
    {
        std::stringstream ss;
        ss << "Total: " << count_ << '/' << total_ << ", " << engine_ << ": " << engine_count_ << '/' << engine_size_;
        if (0 != uncached_)
            ss << ", uncached: " << uncached_;

        std::string s(ss.str());
        if (s.length() > line_lenght_)
            line_lenght_ = s.length();
        else
        {
            std::size_t t(s.length());
            s.append(line_lenght_ - s.length(), ' ');
            line_lenght_ = t;
        }

        std::cerr << '\r' << s << std::flush;
    }
};

}

StatusPrinter::StatusPrinter(std::size_t total)
    : imp_(total)
{
}

StatusPrinter::~StatusPrinter() = default;

void StatusPrinter::current_engine(const std::string & engine, std::size_t size)
{
    imp_->engine_ = engine;
    imp_->engine_size_ = size;
    imp_->engine_count_ = 0;
}

void StatusPrinter::done(std::size_t n)
{
    imp_->done(n);
}
