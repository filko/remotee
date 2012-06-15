#ifndef REMOTEE_CURL_HH
#define REMOTEE_CURL_HH 1

#include <list>
#include <string>

#include <curl/curl.h>

// typedef std::pair<CURLoption, std::string> FetchOption;

// typedef std::list<FetchOption> FetchOptions;

class Curl
{
private:
    CURL * curl_;

    std::string url_;
    std::string buffer_;
    CURLcode errornum_;

public:
    Curl(const Curl &) = delete;
    Curl & operator=(const Curl &) = delete;

    void init();

    explicit Curl();
    ~Curl();

    void perform();

    const std::string & content() const {  return buffer_; }

    void url(const std::string & url);
    const std::string & url() const     {  return url_;  }
    std::string effective_url() const;
    CURLcode status() const             {  return errornum_; }
    bool ok() const                     {  return errornum_ == CURLE_OK; }
    std::string status_as_string() const;
    long http_code() const;
    std::string content_type() const;
    void timeout(long to);
};

#endif
