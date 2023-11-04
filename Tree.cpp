#include "Tree.h"

void Tree::load (std::ifstream & inputFile) {
    std::string buffer;
    std::stack<std::weak_ptr<TreeNode>> treeNodes;

    head.reset();
    std::getline(inputFile, buffer);
    while (!inputFile.eof()) {
        auto newTagNameStart = buffer.find('<');
        while (newTagNameStart != std::string::npos) {
            if (buffer.at(newTagNameStart + 1) != '/') {
                auto newTagNameEnd = buffer.find_first_of(" \t", newTagNameStart);
                std::string newTagName = buffer.substr(newTagNameStart + 1,
                                                       (newTagNameEnd - newTagNameStart) - 1);
                auto valueStartPosition = buffer.find('\"', newTagNameEnd);
                auto valueEndPosition = buffer.find('\"', valueStartPosition + 1);
                std::string newTagValue = buffer.substr(valueStartPosition + 1,
                                                        (valueEndPosition - valueStartPosition) - 1);
                if (treeNodes.empty()) {
                    if (head != nullptr)
                        throw std::runtime_error("Incorrect tree entry in file");
                    head = std::make_shared<TreeNode>(Header(newTagName),
                                                      Value(newTagValue),
                                                      std::shared_ptr<TreeNode> (nullptr));
                    treeNodes.push(head);
                    newTagValue.clear();
                    newTagName.clear();
                } else
                    if (std::shared_ptr<TreeNode> stackTop = treeNodes.top().lock())
                        treeNodes.push(stackTop->addChild(Header(newTagName),
                                                          Value(newTagValue),
                                                          stackTop));
                    else
                        treeNodes.pop();
                newTagNameStart = buffer.find('<', valueEndPosition);
            } else {
                treeNodes.pop();
                newTagNameStart = buffer.find('<', newTagNameStart + 1);
            }
        }
        buffer.clear();
        std::getline(inputFile, buffer);
    }

    if (!treeNodes.empty())
        throw std::runtime_error("Incorrect tree entry in file");

    iteratorList.refreshIteratorList(this);
}

void Tree::save (std::ofstream& outputFile) {
    if (head == nullptr)
        throw std::runtime_error("Incorrect tree entry in file");
    head->recursivePrintTree(outputFile, 0, 4);
}

void Tree::print () const {
    std::stack<std::pair <int, std::weak_ptr<TreeNode>>> nodes;
    nodes.emplace(0, head);
    while (!nodes.empty()) {
        auto currentTopNode = nodes.top();
        if (std::shared_ptr<TreeNode> currentNode = currentTopNode.second.lock()){
            std::string nodeOutput = "(" + currentNode->getTag().getHeader() + ") " +
                    currentNode->getValue().getValue();
            std::cout << std::string(currentTopNode.first, ' ') << nodeOutput << std::endl;
            nodes.pop();
            for (int childIndex = currentNode->getChildrenAmount() - 1; childIndex >= 0; childIndex--)
                nodes.emplace(currentTopNode.first + nodeOutput.length(),
                              currentNode->getChild(childIndex));
        } else nodes.pop();
    }
}

std::weak_ptr<Tree::TreeNode> Tree::TreeNode::addChild(const Header &childTagName,
                                                       const Value &childValue,
                                                       const std::weak_ptr<TreeNode>& childParent) {
    children.emplace_back(std::make_shared<TreeNode>(childTagName, childValue, childParent));
    return children.back();
}

void Tree::TreeNode::recursivePrintTree(std::ostream &output, int indent, int tabLength) const {
    std::string nodeOutput = "<" + tag.getHeader() + " value = \"" + value.getValue() + "\">";
    output << std::string(indent, ' ') << nodeOutput << std::endl;

    for (const auto &child: children)
        child->recursivePrintTree(output, indent + tabLength, tabLength);

    output << std::string(indent, ' ') << "</" + tag.getHeader() + ">" << std::endl;
}

void Tree::TreeNode::for_each_child (const std::function<void (const std::weak_ptr<TreeNode>&)>& functor) {
    for (std::weak_ptr<TreeNode> child : children)
        functor(child);
}

std::weak_ptr<Tree::TreeNode> Tree::TreeNode::addChild(const std::shared_ptr<TreeNode>& newNode) {
    children.push_back(newNode);
    return children.back();
}

void Tree::TreeNode::removeChild(const std::weak_ptr<TreeNode>& removeNode) {
    children.erase(std::find(children.begin(), children.end(), removeNode.lock()));
}

void Tree::IteratorList::refreshIteratorList (Tree * tree) {
    treeNodesList.clear();

    tree->for_each([&] (const std::weak_ptr<TreeNode>& node)
                               { treeNodesList.push_back(node); });
}

std::list<std::weak_ptr<Tree::TreeNode>>::iterator Tree::IteratorList::updateIteratorList (
        const std::list<std::weak_ptr<TreeNode>>::iterator& parentIter,
        const std::weak_ptr<TreeNode>& newTreeNode) {
    return treeNodesList.insert(std::find_if(treeNodesList.begin(), treeNodesList.end(),
                                             [&](const std::weak_ptr<TreeNode>& ptr)
                                             {return ptr.lock() == parentIter->lock();}),
                                newTreeNode);
}

void Tree::IteratorList::eraseFromIteratorList (const std::list<std::weak_ptr<TreeNode>>::iterator& iter) {
    treeNodesList.erase(std::find_if(treeNodesList.begin(), treeNodesList.end(),
                                     [&](const std::weak_ptr<TreeNode>& ptr)
                                     {return ptr.lock() == iter->lock();}));
}

void Tree::for_each (const std::function<void (const std::weak_ptr<TreeNode>&)>& functor) {
    functor(head);

    std::function<void (const std::weak_ptr<TreeNode>& node)> bruteForce;
    bruteForce = [&](const std::weak_ptr<TreeNode>& node) {
        if (std::shared_ptr<TreeNode> childNode = node.lock()){
            functor(childNode);
            childNode->for_each_child(bruteForce);
        }
    };

    head->for_each_child([&] (const std::weak_ptr<TreeNode>& node) {
        if (std::shared_ptr<TreeNode> childNode = node.lock()) {
            functor(childNode);
            childNode->for_each_child(bruteForce);
        }
    });
}

std::list<std::weak_ptr<Tree::TreeNode>>::iterator Tree::find(const Header & findHeader) {
    return std::find_if(iteratorList.begin(), iteratorList.end(),
                        [&](const std::weak_ptr<TreeNode>& ptr)
                                { if(std::shared_ptr currentNode = ptr.lock())
                                    return currentNode->getTag() == findHeader;
                                  else
                                    return false;});
}

std::list<std::weak_ptr<Tree::TreeNode>>::iterator Tree::find(const Value & findValue) {
    return std::find_if(iteratorList.begin(), iteratorList.end(),
                        [&](const std::weak_ptr<TreeNode>& ptr)
                            { if(std::shared_ptr currentNode = ptr.lock())
                                return currentNode->getValue() == findValue;
                              else
                                return false;});
}

std::list<std::weak_ptr<Tree::TreeNode>>::iterator Tree::add(
        const std::list<std::weak_ptr<TreeNode>>::iterator& parentPosition,
        const Header& header, const Value& value) {
    if(std::shared_ptr parentNode = parentPosition->lock()) {
        auto newNode = parentNode->addChild(header,
                                            value, parentNode);
        return iteratorList.updateIteratorList(parentPosition, newNode);
    } else {
        throw std::runtime_error("Not found parent position");
    }
}

void Tree::erase(const std::list<std::weak_ptr<TreeNode>>::iterator& erasePosition) {
    if(std::shared_ptr eraseNode = erasePosition->lock()) {
        eraseNode->for_each_child([&] (const std::weak_ptr<TreeNode>& node) {
            if (std::shared_ptr<TreeNode> parentNode = eraseNode->getParent().lock()) {
                if (std::shared_ptr<TreeNode> childNode = node.lock())
                    parentNode->addChild(childNode);
            } else throw std::runtime_error("Impossible to erase");
        });
        eraseNode->clearChildren();
        iteratorList.eraseFromIteratorList(erasePosition);
        eraseNode->getParent().lock()->removeChild(eraseNode);
    } else throw std::runtime_error("Not found erase position");
}