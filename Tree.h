#ifndef XMLRESOURCE_TREE_H
#define XMLRESOURCE_TREE_H


#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <stack>
#include <string>
#include <sstream>
#include <functional>
#include <queue>

class Tree {
private:
    class TreeNode {
        std::string tag;
        std::string value;
        std::vector<std::shared_ptr<TreeNode>> children;

    public:
        explicit TreeNode(std::string tag, std::string value) :
                tag(std::move(tag)), value(std::move(value)) {}

        void recursivePrintTree(std::ostream &output, int indent, int tabLength);

        std::weak_ptr<TreeNode> addChild(const std::string &childTagName, const std::string &childValue);
        std::weak_ptr<TreeNode> getChild(int index) { return children[index]; }
        unsigned int getChildrenAmount() { return children.size(); }

        std::string getValue() const { return value; }
        std::string getTag() const { return tag; }
    };

    std::shared_ptr<TreeNode> head;

public:
    Tree() : head(nullptr) {}

    void load(const std::string &path);
    void save(const std::string &path);
    void print();
};

#endif //XMLRESOURCE_TREE_H