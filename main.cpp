#include "Tree.h"

int main() {
    Tree tree;
    tree.load("..\\tree.xml");
    tree.print();
    tree.save("..\\output_tree.xml");
}