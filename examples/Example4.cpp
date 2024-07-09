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

#include "AVLTree/AVLTree.hpp"

#include <iostream>
#include <string>

int main() {
    avl_tree_t<int, std::string> tree;

    tree.insert(2, "two");
    tree.insert(4, "four");
    tree.insert(90, "ninety");
    tree.insert(3, "three");
    tree.insert(0, "zero");
    tree.insert(14, "fourteen");
    tree.insert(45, "forty-five");

    std::cout << "Height: " << tree.height() << std::endl;
    std::cout << "Size: " << tree.size() << std::endl;

    tree.clear();

    std::cout << "Height: " << tree.height() << std::endl;
    std::cout << "Size: " << tree.size() << std::endl;

    return 0;
}
