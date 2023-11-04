#include "XMLResource.h"

int main() {
    std::unique_ptr<XMLResource> example = XMLResource::create("..\\tree1.xml");
    example->print();
    example->load("..\\tree.xml");
    example->print();
    example->add(example->find(Value ("12")),
                 Header("NEWTAG"), Value("122"));
    example->erase(example->find(Value ("13")));
    example->print();
    if (std::shared_ptr<Tree::TreeNode> testIterator = example->find(Header("CHILD"))++->lock())
        std::cout << testIterator->getValue().getValue() << std::endl;
    example->save("..\\output_tree.xml");
}