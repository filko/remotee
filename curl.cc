#include "curl.hh"

#include <stdexcept>

namespace
{

std::size_t writer(void *buffer, size_t size, size_t nmemb, void *userp)
{
    std::string & out(*static_cast<std::string*>(userp));
    out.append(static_cast<const char*>(buffer), size * nmemb);
    return size * nmemb;
}

struct Init
{
    Init()
    {
        curl_global_init(CURL_GLOBAL_ALL);
    }

    ~Init()
    {
        curl_global_cleanup();
    }
} init_;

}

Curl::Curl()
    : curl_(curl_easy_init()),
      url_(),
      errornum_(CURLE_OK)
{
    if (!curl_)
        throw std::runtime_error("allocating curl failed!");

    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &buffer_);
//    curl_easy_setopt(curl_, CURLOPT_VERBOSE, (long)1);
//    curl_easy_setopt(curl_, CURLOPT_USERAGENT, "");
}

Curl::~Curl()
{
    curl_easy_cleanup(curl_);
}

void Curl::url(const std::string & url)
{
    url_ = url;
    buffer_.clear();
    curl_easy_setopt(curl_, CURLOPT_URL, url_.c_str());    
}

void Curl::perform()
{
    errornum_ = curl_easy_perform(curl_);
}

std::string Curl::effective_url() const
{
    const char * buf(0);
    curl_easy_getinfo(curl_, CURLINFO_EFFECTIVE_URL, &buf);
    return buf;
}

std::string Curl::status_as_string() const
{
    return curl_easy_strerror(errornum_);
}

long Curl::http_code() const
{
    long ret;
    curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &ret);
    return ret;
}

std::string Curl::content_type() const
{
    char * content(0);
    curl_easy_getinfo(curl_, CURLINFO_CONTENT_TYPE, &content);
    if (content)
        return content;

    return "";
}

void Curl::timeout(long to)
{
    curl_easy_setopt(curl_, CURLOPT_TIMEOUT, to);
}
