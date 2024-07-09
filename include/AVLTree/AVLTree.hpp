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

#pragma once

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <memory>
#include <queue>
#include <utility>

template<typename key_t, typename value_t>
struct avl_tree_node_t {
    avl_tree_node_t(const key_t &key, const value_t &value) : key(key), value(value) {}

    std::shared_ptr<avl_tree_node_t> lchild;
    std::shared_ptr<avl_tree_node_t> rchild;

    key_t key;
    value_t value;

    unsigned height = 1;
};

template<typename key_t, typename value_t>
using handler_t = std::function<void(const key_t &, value_t &)>;

template<typename key_t, typename value_t>
class avl_tree_t {
private:
    void PreOrderTraversalHelper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root, handler_t<key_t, value_t> handler) {
        if (!root) return;
        handler(root->key, root->value);
        PreOrderTraversalHelper(root->lchild, handler);
        PreOrderTraversalHelper(root->rchild, handler);
    }

    void InOrderTraversalHelper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root, handler_t<key_t, value_t> handler) {
        if (!root) return;
        InOrderTraversalHelper(root->lchild, handler);
        handler(root->key, root->value);
        InOrderTraversalHelper(root->rchild, handler);
    }

    void PostOrderTraversalHelper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root, handler_t<key_t, value_t> handler) {
        if (!root) return;
        PostOrderTraversalHelper(root->lchild, handler);
        PostOrderTraversalHelper(root->rchild, handler);
        handler(root->key, root->value);
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> InsertHelper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root, const key_t &key, const value_t &value) {
        if (!root) return std::make_shared<avl_tree_node_t<key_t, value_t>>(key, value);

        if (key < root->key) {
            root->lchild = InsertHelper(root->lchild, key, value);
        } else if (key > root->key) {
            root->rchild = InsertHelper(root->rchild, key, value);
        } else {
            return root;
        }

        root->height = 1 + std::max(HeightHelper(root->lchild), HeightHelper(root->rchild));

        int balance = Balance(root);

        if (balance > 1 && key < root->lchild->key) {
            return RotateRight(root);
        }

        if (balance < -1 && key > root->rchild->key) {
            return RotateLeft(root);
        }

        if (balance > 1 && key > root->lchild->key) {
            root->lchild = RotateLeft(root->lchild);
            return RotateRight(root);
        }

        if (balance < -1 && key < root->rchild->key) {
            root->rchild = RotateRight(root->rchild);
            return RotateLeft(root);
        }

        return root;
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> RemoveHelper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root, const key_t &key) {
        if (!root) return nullptr;

        if (key < root->key) {
            root->lchild = RemoveHelper(root->lchild, key);
        } else if (key > root->key) {
            root->rchild = RemoveHelper(root->rchild, key);
        } else {
            if (!root->lchild || !root->rchild) {
                std::shared_ptr<avl_tree_node_t<key_t, value_t>> temp = root->lchild ? root->lchild : root->rchild;
                root.reset();
                return temp;
            }

            std::shared_ptr<avl_tree_node_t<key_t, value_t>> temp = MinimumHelper(root->rchild);
            root->key = temp->key;
            root->rchild = RemoveHelper(root->rchild, temp->key);
        }

        root->height = 1 + std::max(HeightHelper(root->lchild), HeightHelper(root->rchild));

        int balance = Balance(root);

        if (balance > 1 && Balance(root->lchild) >= 0) {
            return RotateRight(root);
        }

        if (balance > 1 && Balance(root->lchild) < 0) {
            root->lchild = RotateLeft(root->lchild);
            return RotateRight(root);
        }

        if (balance < -1 && Balance(root->rchild) <= 0) {
            return RotateLeft(root);
        }

        if (balance < -1 && Balance(root->rchild) > 0) {
            root->rchild = RotateRight(root->rchild);
            return RotateLeft(root);
        }

        return root;
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> SearchHelper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root, const key_t &key) {
        while (root) {
            if (key > root->key) {
                root = root->rchild;
            } else if (key < root->key) {
                root = root->lchild;
            } else {
                return root;
            }
        }
        return nullptr;
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> MinimumHelper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root) {
        if (!root) return nullptr;
        while (root->lchild) root = root->lchild;
        return root;
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> MaximumHelper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root) {
        if (!root) return nullptr;
        while (root->rchild) root = root->rchild;
        return root;
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> RotateRight(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root) {
        auto pivot = root->lchild;
        auto orphan = pivot->rchild;

        pivot->rchild = root;
        root->lchild = orphan;

        root->height = 1 + std::max(HeightHelper(root->lchild), HeightHelper(root->rchild));
        pivot->height = 1 + std::max(HeightHelper(pivot->lchild), HeightHelper(pivot->rchild));

        return pivot;
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> RotateLeft(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root) {
        auto pivot = root->rchild;
        auto orphan = pivot->lchild;

        pivot->lchild = root;
        root->rchild = orphan;

        root->height = 1 + std::max(HeightHelper(root->lchild), HeightHelper(root->rchild));
        pivot->height = 1 + std::max(HeightHelper(pivot->lchild), HeightHelper(pivot->rchild));

        return pivot;
    }

    unsigned HeightHelper(const std::shared_ptr<avl_tree_node_t<key_t, value_t>> root) const {
        if (!root) return 0;
        return root->height;
    }

    unsigned SizeHelper(const std::shared_ptr<avl_tree_node_t<key_t, value_t>> root) {
        if (!root) return 0;
        return 1 + SizeHelper(root->lchild) + SizeHelper(root->rchild);
    }

    int Balance(const std::shared_ptr<avl_tree_node_t<key_t, value_t>> root) {
        if (!root) return 0;
        return HeightHelper(root->lchild) - HeightHelper(root->rchild);
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> mRoot;

public:
    avl_tree_t() = default;

    void PreOrderTraversal(handler_t<key_t, value_t> handler) {
        PreOrderTraversalHelper(mRoot, handler);
    }

    void InOrderTraversal(handler_t<key_t, value_t> handler) {
        InOrderTraversalHelper(mRoot, handler);
    }

    void PostOrderTraversal(handler_t<key_t, value_t> handler) {
        PostOrderTraversalHelper(mRoot, handler);
    }

    void Clear() {
        mRoot.reset();
    }

    void Insert(const key_t &key, const value_t &value) {
        mRoot = InsertHelper(mRoot, key, value);
    }

    void Remove(const key_t &key) {
        mRoot = RemoveHelper(mRoot, key);
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> Search(const key_t &key) {
        return SearchHelper(mRoot, key);
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> Minimum() {
        return MinimumHelper(mRoot);
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> Maximum() {
        return MaximumHelper(mRoot);
    }

    unsigned Height() {
        return HeightHelper(mRoot);
    }

    unsigned Size() {
        return SizeHelper(mRoot);
    }
};
