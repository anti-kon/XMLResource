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
#include "XMLResource.h"

class Tree {
public:
    class TreeNode {
        XMLResource::Header tag;
        XMLResource::Value value;
        std::vector<std::shared_ptr<TreeNode>> children;

    public:
        explicit TreeNode(XMLResource::Header tag, XMLResource::Value value) :
                tag(std::move(tag)), value(std::move(value)) {}

        void recursivePrintTree(std::ostream &output, int indent, int tabLength) const;
        void for_each_child (const std::function<void (const std::weak_ptr<TreeNode>&)>& functor);

        std::weak_ptr<TreeNode> addChild(const XMLResource::Header &childTagName,
                                         const XMLResource::Value &childValue);
        std::weak_ptr<TreeNode> getChild(int index) { return children[index]; }
        unsigned int getChildrenAmount() { return children.size(); }

        [[nodiscard]] XMLResource::Value getValue() const { return value; }
        [[nodiscard]] XMLResource::Header getTag() const { return tag; }
    };
private:
    class IteratorList {
        std::list<std::weak_ptr<TreeNode>> treeNodesList;

    public:
        IteratorList() {treeNodesList = {};}

        void refreshIteratorList (Tree * tree);
        std::list<std::weak_ptr<TreeNode>>::iterator updateIteratorList (
                const std::list<std::weak_ptr<TreeNode>>::iterator& parentIter,
                const std::weak_ptr<TreeNode>& newTreeNode);
        void eraseFromIteratorList (const std::list<std::weak_ptr<TreeNode>>::iterator& iter);

        std::list<std::weak_ptr<TreeNode>>::iterator begin () { return treeNodesList.begin(); }
        std::list<std::weak_ptr<TreeNode>>::iterator end () { return treeNodesList.end(); }
    };

    std::shared_ptr<TreeNode> head;
    IteratorList iteratorList;

public:
    Tree() : head(nullptr) {iteratorList = IteratorList();}

    void load(const std::string &path);

    void save(const std::string &path);
    void print();

    std::list<std::weak_ptr<TreeNode>>::iterator find(const XMLResource::Header & findHeader);
    std::list<std::weak_ptr<TreeNode>>::iterator find(const XMLResource::Value & findValue);

    std::list<std::weak_ptr<TreeNode>>::iterator add(
            const std::list<std::weak_ptr<TreeNode>>::iterator& parentPosition,
            const XMLResource::Header& header, const XMLResource::Value& value);

    void for_each (const std::function<void (const std::weak_ptr<TreeNode>&)>& functor);
};

#endif //XMLRESOURCE_TREE_H