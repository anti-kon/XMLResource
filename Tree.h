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
public:
    class TreeNode {
        std::string tag;
        std::string value;
        std::vector<std::shared_ptr<TreeNode>> children;

    public:
        explicit TreeNode(std::string tag, std::string value) :
                tag(std::move(tag)), value(std::move(value)) {}

        void recursivePrintTree(std::ostream &output, int indent, int tabLength);
        void for_each_child (const std::function<void (const std::weak_ptr<TreeNode>&)>& functor);

        std::weak_ptr<TreeNode> addChild(const std::string &childTagName, const std::string &childValue);
        std::weak_ptr<TreeNode> getChild(int index) { return children[index]; }
        unsigned int getChildrenAmount() { return children.size(); }

        std::string getValue() const { return value; }
        std::string getTag() const { return tag; }
    };
private:
    class IteratorList {
        std::list<std::weak_ptr<TreeNode>> treeNodesList;

    public:
        IteratorList() {treeNodesList = {};}

        void refreshIteratorList (Tree * tree);
        void updateIteratorList (const std::list<std::weak_ptr<TreeNode>>::iterator& parentIter,
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

    void for_each (const std::function<void (const std::weak_ptr<TreeNode>&)>& functor);
};

#endif //XMLRESOURCE_TREE_H