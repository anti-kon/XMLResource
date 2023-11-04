#ifndef XMLRESOURCE_XMLRESOURCE_H
#define XMLRESOURCE_XMLRESOURCE_H


#include <string>
#include <utility>
#include "Tree.h"

class XMLResource {
private:
    Tree tree;

    explicit XMLResource(const std::string& path) {
        std::ifstream inputFile;
        try{
            tree = Tree();

            inputFile.open(path);

            if (!inputFile.is_open() || path.substr(path.length() - 4, 4) != ".xml")
                return;

            tree.load(inputFile);
        } catch (...) {
            tree = Tree();
        }
        inputFile.close();
    }
public:
    void load(const std::string &path);

    void print() const;
    void save(const std::string &path);

    std::list<std::weak_ptr<Tree::TreeNode>>::iterator find(const Header & findHeader);
    std::list<std::weak_ptr<Tree::TreeNode>>::iterator find(const Value & findValue);

    std::list<std::weak_ptr<Tree::TreeNode>>::iterator add(
            const std::list<std::weak_ptr<Tree::TreeNode>>::iterator& parentPosition,
            const Header& header, const Value& value);
    bool erase(const std::list<std::weak_ptr<Tree::TreeNode>>::iterator& erasePosition);

    static std::unique_ptr<XMLResource> create (const std::string& path);
};

#endif //XMLRESOURCE_XMLRESOURCE_H
