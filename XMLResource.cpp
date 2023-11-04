#include "XMLResource.h"

void XMLResource::load(const std::string &path) {
    Tree saveTree = tree;

    std::ifstream inputFile;
    inputFile.open(path);

    if (!inputFile.is_open() || path.substr(path.length() - 4, 4) != ".xml")
        return;

    try {
        tree.load(inputFile);
    } catch (...) {
        tree = saveTree;
    }

    inputFile.close();
}

void XMLResource::save(const std::string &path) {
    std::ofstream outputFile;
    outputFile.open(path);

    tree.save(outputFile);

    outputFile.close();
}

std::list<std::weak_ptr<Tree::TreeNode>>::iterator XMLResource::find(const Header &findHeader) {
    return tree.find(findHeader);
}

std::list<std::weak_ptr<Tree::TreeNode>>::iterator XMLResource::find(const Value &findValue) {
    return tree.find(findValue);
}

std::list<std::weak_ptr<Tree::TreeNode>>::iterator
XMLResource::add(const std::list<std::weak_ptr<Tree::TreeNode>>::iterator &parentPosition,
                 const Header &header,
                 const Value &value) {
    try {
        return tree.add(parentPosition, header, value);
    } catch (...) {
        return tree.end();
    }
}

bool XMLResource::erase(const std::list<std::weak_ptr<Tree::TreeNode>>::iterator &erasePosition) {
    try {
        tree.erase(erasePosition);
        return true;
    } catch (...) {
        return false;
    }
}

std::unique_ptr<XMLResource> XMLResource::create(const std::string &path) {
    return std::unique_ptr<XMLResource>(new XMLResource(path));
}

void XMLResource::print() const {
    tree.print();
}
