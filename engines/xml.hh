#ifndef REMOTEE_ENGINES_XML_HH
#define REMOTEE_ENGINES_XML_HH 1

#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <paludis/util/exception.hh>

class XmlException
    : public paludis::Exception
{
public:
    XmlException(const std::string & m)
        : paludis::Exception(m)
    {
    }
};

struct XmldocDeleter
{
    constexpr XmldocDeleter() = default;

    void operator()(xmlDoc * doc)
    {
        xmlFreeDoc(doc);
    }
};

typedef std::unique_ptr<xmlDoc, XmldocDeleter> UniqueXml;

inline
UniqueXml load_html_or_throw(const std::string & input)
{
    UniqueXml ptr(htmlReadMemory(input.c_str(), input.size(), "", "",
                                 HTML_PARSE_RECOVER | HTML_PARSE_NOERROR |
                                 HTML_PARSE_NOWARNING | HTML_PARSE_NOBLANKS |
                                 HTML_PARSE_NONET ));

    if (! ptr)
        throw XmlException("Parsing xml failed.");

    return ptr;
}

struct XmlxpathcontextDeleter
{
    constexpr XmlxpathcontextDeleter() = default;

    void operator()(xmlXPathContext * ctx)
    {
        xmlXPathFreeContext(ctx);
    }
};

typedef std::unique_ptr<xmlXPathContext, XmlxpathcontextDeleter> UniqueXmlContext;

inline
UniqueXmlContext xml_get_context_or_throw(const UniqueXml & doc)
{
    UniqueXmlContext ctx(xmlXPathNewContext(doc.get()));

    if (! ctx)
        throw XmlException("Cannot create xpath context");

    return ctx;
}

struct XmlxpathobjDeleter
{
    constexpr XmlxpathobjDeleter() = default;

    void operator()(xmlXPathObject * obj)
    {
        xmlXPathFreeObject(obj);
    }
};

typedef std::unique_ptr<xmlXPathObject, XmlxpathobjDeleter> UniqueXmlObject;

inline
UniqueXmlObject xml_eval_expression_or_throw(const UniqueXmlContext & ctx, const char * path)
{
    UniqueXmlObject obj(xmlXPathEvalExpression((const xmlChar*)path, ctx.get()));

    if (! obj)
        throw XmlException(std::string("Cannot evaluate expression '") + path + "'");

    return obj;
}

struct XmlstringDeleter
{
    constexpr XmlstringDeleter() = default;

    void operator()(xmlChar * string)
    {
        xmlFree(string);
    }
};

typedef std::unique_ptr<xmlChar, XmlstringDeleter> UniqueXmlString;

#endif
