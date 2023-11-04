#ifndef XMLRESOURCE_XMLRESOURCE_H
#define XMLRESOURCE_XMLRESOURCE_H


#include <string>
#include <utility>

class XMLResource {
public:
    class Header {
        std::string header;
    public:
        explicit Header(std::string header) : header(std::move(header)) {}

        bool operator == (const Header& other) const {
            return (other.header == header);
        };

        [[nodiscard]] std::string getHeader() const { return header; };
    };

    class Value {
        std::string value;
    public:
        explicit Value(std::string value) : value(std::move(value)) {}

        bool operator == (const Value& other) const {
            return (other.value == value);
        };

        [[nodiscard]] std::string getValue() const { return value; };
    };
};


#endif //XMLRESOURCE_XMLRESOURCE_H
