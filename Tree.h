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
#include "XMLAddition.h"

class Tree {
public:
    class TreeNode {
        Header tag;
        Value value;
        std::weak_ptr<TreeNode> parent;
        std::vector<std::shared_ptr<TreeNode>> children;

    public:
        explicit TreeNode(Header tag, Value value, std::weak_ptr<TreeNode> parent) :
                tag(std::move(tag)), value(std::move(value)), parent(std::move(parent)) {}

        void recursivePrintTree(std::ostream &output, int indent, int tabLength) const;
        void for_each_child (const std::function<void (const std::weak_ptr<TreeNode>&)>& functor);

        std::weak_ptr<TreeNode> addChild(const Header &childTagName,
                                         const Value &childValue,
                                         const std::weak_ptr<TreeNode>& childParent);
        std::weak_ptr<TreeNode> addChild(const std::shared_ptr<TreeNode>& newNode);
        [[nodiscard]] std::weak_ptr<TreeNode> getChild(int index) const { return children[index]; }
        void removeChild(const std::weak_ptr<TreeNode>& removeNode);

        [[nodiscard]] unsigned int getChildrenAmount() const { return children.size(); }
        void clearChildren() { children.clear(); }

        [[nodiscard]] Value getValue() const { return value; }
        [[nodiscard]] Header getTag() const { return tag; }
        [[nodiscard]] std::weak_ptr<TreeNode> getParent() const { return parent; };
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

    void load(std::ifstream & inputFile);

    void save(std::ofstream& outputFile);
    void print() const;

    std::list<std::weak_ptr<TreeNode>>::iterator find(const Header & findHeader);
    std::list<std::weak_ptr<TreeNode>>::iterator find(const Value & findValue);

    std::list<std::weak_ptr<TreeNode>>::iterator add(
            const std::list<std::weak_ptr<TreeNode>>::iterator& parentPosition,
            const Header& header, const Value& value);
    void erase(const std::list<std::weak_ptr<TreeNode>>::iterator& parentPosition);

    void for_each (const std::function<void (const std::weak_ptr<TreeNode>&)>& functor);

    std::list<std::weak_ptr<Tree::TreeNode>>::iterator begin () { return iteratorList.begin(); };
    std::list<std::weak_ptr<Tree::TreeNode>>::iterator end () { return iteratorList.end(); };
};

#endif //XMLRESOURCE_TREE_H