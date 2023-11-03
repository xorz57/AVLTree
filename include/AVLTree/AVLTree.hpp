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

template<typename Key, typename Value>
class AVLTree {
public:
    using Handler = std::function<void(const Key &, Value &)>;

private:
    struct AVLTreeNode {
        AVLTreeNode(const Key &key, const Value &value) : key(key), value(value) {}

        std::shared_ptr<AVLTreeNode> lChild;
        std::shared_ptr<AVLTreeNode> rChild;

        Key key;
        Value value;

        unsigned height = 1;
    };

    void PreOrderTraversalHelper(std::shared_ptr<AVLTreeNode> root, Handler handler) {
        if (!root) return;
        handler(root->key, root->value);
        PreOrderTraversalHelper(root->lChild, handler);
        PreOrderTraversalHelper(root->rChild, handler);
    }

    void InOrderTraversalHelper(std::shared_ptr<AVLTreeNode> root, Handler handler) {
        if (!root) return;
        InOrderTraversalHelper(root->lChild, handler);
        handler(root->key, root->value);
        InOrderTraversalHelper(root->rChild, handler);
    }

    void PostOrderTraversalHelper(std::shared_ptr<AVLTreeNode> root, Handler handler) {
        if (!root) return;
        PostOrderTraversalHelper(root->lChild, handler);
        PostOrderTraversalHelper(root->rChild, handler);
        handler(root->key, root->value);
    }

    std::shared_ptr<AVLTreeNode> InsertHelper(std::shared_ptr<AVLTreeNode> root, const Key &key, const Value &value) {
        if (!root) return std::make_shared<AVLTreeNode>(key, value);

        if (key < root->key) {
            root->lChild = InsertHelper(root->lChild, key, value);
        } else if (key > root->key) {
            root->rChild = InsertHelper(root->rChild, key, value);
        } else {
            return root;
        }

        root->height = 1 + std::max(HeightHelper(root->lChild), HeightHelper(root->rChild));

        int balance = Balance(root);

        if (balance > 1 && key < root->lChild->key) {
            return RotateRight(root);
        }

        if (balance < -1 && key > root->rChild->key) {
            return RotateLeft(root);
        }

        if (balance > 1 && key > root->lChild->key) {
            root->lChild = RotateLeft(root->lChild);
            return RotateRight(root);
        }

        if (balance < -1 && key < root->rChild->key) {
            root->rChild = RotateRight(root->rChild);
            return RotateLeft(root);
        }

        return root;
    }

    std::shared_ptr<AVLTreeNode> RemoveHelper(std::shared_ptr<AVLTreeNode> root, const Key &key) {
        if (!root) return nullptr;

        if (key < root->key) {
            root->lChild = RemoveHelper(root->lChild, key);
        } else if (key > root->key) {
            root->rChild = RemoveHelper(root->rChild, key);
        } else {
            if (!root->lChild || !root->rChild) {
                std::shared_ptr<AVLTreeNode> temp = root->lChild ? root->lChild : root->rChild;
                root.reset();
                return temp;
            }

            std::shared_ptr<AVLTreeNode> temp = MinimumHelper(root->rChild);
            root->key = temp->key;
            root->rChild = RemoveHelper(root->rChild, temp->key);
        }

        root->height = 1 + std::max(HeightHelper(root->lChild), HeightHelper(root->rChild));

        int balance = Balance(root);

        if (balance > 1 && Balance(root->lChild) >= 0) {
            return RotateRight(root);
        }

        if (balance > 1 && Balance(root->lChild) < 0) {
            root->lChild = RotateLeft(root->lChild);
            return RotateRight(root);
        }

        if (balance < -1 && Balance(root->rChild) <= 0) {
            return RotateLeft(root);
        }

        if (balance < -1 && Balance(root->rChild) > 0) {
            root->rChild = RotateRight(root->rChild);
            return RotateLeft(root);
        }

        return root;
    }

    std::shared_ptr<AVLTreeNode> SearchHelper(std::shared_ptr<AVLTreeNode> root, const Key &key) {
        while (root) {
            if (key > root->key) {
                root = root->rChild;
            } else if (key < root->key) {
                root = root->lChild;
            } else {
                return root;
            }
        }
        return nullptr;
    }

    std::shared_ptr<AVLTreeNode> MinimumHelper(std::shared_ptr<AVLTreeNode> root) {
        if (!root) return nullptr;
        while (root->lChild) root = root->lChild;
        return root;
    }

    std::shared_ptr<AVLTreeNode> MaximumHelper(std::shared_ptr<AVLTreeNode> root) {
        if (!root) return nullptr;
        while (root->rChild) root = root->rChild;
        return root;
    }

    std::shared_ptr<AVLTreeNode> RotateRight(std::shared_ptr<AVLTreeNode> root) {
        auto pivot = root->lChild;
        auto orphan = pivot->rChild;

        pivot->rChild = root;
        root->lChild = orphan;

        root->height = 1 + std::max(HeightHelper(root->lChild), HeightHelper(root->rChild));
        pivot->height = 1 + std::max(HeightHelper(pivot->lChild), HeightHelper(pivot->rChild));

        return pivot;
    }

    std::shared_ptr<AVLTreeNode> RotateLeft(std::shared_ptr<AVLTreeNode> root) {
        auto pivot = root->rChild;
        auto orphan = pivot->lChild;

        pivot->lChild = root;
        root->rChild = orphan;

        root->height = 1 + std::max(HeightHelper(root->lChild), HeightHelper(root->rChild));
        pivot->height = 1 + std::max(HeightHelper(pivot->lChild), HeightHelper(pivot->rChild));

        return pivot;
    }

    unsigned HeightHelper(const std::shared_ptr<AVLTreeNode> root) const {
        if (!root) return 0;
        return root->height;
    }

    unsigned SizeHelper(const std::shared_ptr<AVLTreeNode> root) {
        if (!root) return 0;
        return 1 + SizeHelper(root->lChild) + SizeHelper(root->rChild);
    }

    int Balance(const std::shared_ptr<AVLTreeNode> root) {
        if (!root) return 0;
        return HeightHelper(root->lChild) - HeightHelper(root->rChild);
    }

    std::shared_ptr<AVLTreeNode> mRoot;

public:
    AVLTree() = default;

    void PreOrderTraversal(Handler handler) {
        PreOrderTraversalHelper(mRoot, handler);
    }

    void InOrderTraversal(Handler handler) {
        InOrderTraversalHelper(mRoot, handler);
    }

    void PostOrderTraversal(Handler handler) {
        PostOrderTraversalHelper(mRoot, handler);
    }

    void Clear() {
        mRoot.reset();
    }

    void Insert(const Key &key, const Value &value) {
        mRoot = InsertHelper(mRoot, key, value);
    }

    void Remove(const Key &key) {
        mRoot = RemoveHelper(mRoot, key);
    }

    std::shared_ptr<AVLTreeNode> Search(const Key &key) {
        return SearchHelper(mRoot, key);
    }

    std::shared_ptr<AVLTreeNode> Minimum() {
        return MinimumHelper(mRoot);
    }

    std::shared_ptr<AVLTreeNode> Maximum() {
        return MaximumHelper(mRoot);
    }

    unsigned Height() {
        return HeightHelper(mRoot);
    }

    unsigned Size() {
        return SizeHelper(mRoot);
    }
};
