# AVLTree

AVLTree written in C++14

## Example

```cpp
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

    tree.PreOrderTraversal([](auto key, auto &value) {
        std::cout << key << " -> " << value << std::endl;
    });
    std::cout << std::endl;

    tree.InOrderTraversal([](auto key, auto &value) {
        std::cout << key << " -> " << value << std::endl;
    });
    std::cout << std::endl;

    tree.PostOrderTraversal([](auto key, auto &value) {
        std::cout << key << " -> " << value << std::endl;
    });
    std::cout << std::endl;

    return 0;
}
```

```console
4 -> four
2 -> two
0 -> zero
3 -> three
45 -> forty-five
14 -> fourteen
90 -> ninety

0 -> zero
2 -> two
3 -> three
4 -> four
14 -> fourteen
45 -> forty-five
90 -> ninety

0 -> zero
3 -> three
2 -> two
14 -> fourteen
90 -> ninety
45 -> forty-five
4 -> four
```

## How to Build using [CMake](https://cmake.org/)

```bash
cmake -B build
cmake --build build --config Release
ctest -C Release
```
