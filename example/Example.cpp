/*
	MIT License

	Copyright(c) 2023 George Fotopoulos

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files(the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions :

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include <AVLTree/AVLTree.hpp>

#include <iostream>
#include <string>

int main() {
    AVLTree<int, std::string> tree;

    tree.Insert(2, "two");
    tree.Insert(4, "four");
    tree.Insert(90, "ninety");
    tree.Insert(3, "three");
    tree.Insert(0, "zero");
    tree.Insert(14, "fourteen");
    tree.Insert(45, "forty-five");

    std::cout << "Pre Order Traversal" << std::endl;
    tree.PreOrderTraversal([](auto key, auto &value) {
        std::cout << key << " -> " << value << std::endl;
    });
    std::cout << std::endl;

    std::cout << "In Order Traversal" << std::endl;
    tree.InOrderTraversal([](auto key, auto &value) {
        std::cout << key << " -> " << value << std::endl;
    });
    std::cout << std::endl;

    std::cout << "Post Order Traversal" << std::endl;
    tree.PostOrderTraversal([](auto key, auto &value) {
        std::cout << key << " -> " << value << std::endl;
    });
    std::cout << std::endl;

    std::cout << "Breadth First Traversal" << std::endl;
    tree.BreadthFirstTraversal([](auto key, auto &value) {
        std::cout << key << " -> " << value << std::endl;
    });
    std::cout << std::endl;

    std::cout << "Remove Node with Key: 3" << std::endl;
    tree.Remove(3);

    std::cout << "Search Node with Key: 3" << std::endl;
    auto result = tree.Search(3);
    if (result) {
        std::cout << "Found" << std::endl;
    } else {
        std::cout << "Not Found" << std::endl;
    }

    auto min = tree.Minimum();
    std::cout << "Minimum: ";
    if (min) {
        std::cout << min->key << " -> " << min->value << std::endl;
    } else {
        std::cout << "Not Found" << std::endl;
    }

    auto max = tree.Maximum();
    std::cout << "Maximum: ";
    if (max) {
        std::cout << max->key << " -> " << max->value << std::endl;
    } else {
        std::cout << "Not Found" << std::endl;
    }

    std::cout << "Height: " << tree.Height() << std::endl;
    std::cout << "Size: " << tree.Size() << std::endl;

    tree.Clear();

    return 0;
}
