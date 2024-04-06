# AVLTree

[![Build](https://github.com/xorz57/AVLTree/actions/workflows/Build.yml/badge.svg)](https://github.com/xorz57/AVLTree/actions/workflows/Build.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=xorz57_AVLTree&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=xorz57_AVLTree)

## Example

```cpp
#include "AVLTree/AVLTree.hpp"

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

## How to Build

#### Linux & macOS

```bash
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh

git clone https://github.com/xorz57/AVLTree.git
cd AVLTree
cmake -B build -DCMAKE_BUILD_TYPE=Release -S . -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
ctest --build-config Release
```

#### Windows

```powershell
git clone https://github.com/microsoft/vcpkg.git C:/vcpkg
C:/vcpkg/bootstrap-vcpkg.bat
C:/vcpkg/vcpkg.exe integrate install

git clone https://github.com/xorz57/AVLTree.git
cd AVLTree
cmake -B build -DCMAKE_BUILD_TYPE=Release -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
ctest --build-config Release
```
