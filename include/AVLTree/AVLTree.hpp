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
#include <queue>
#include <utility>

template<typename Key, typename Value>
class AVLTree {
public:
    using Handler = std::function<void(const Key &, Value &)>;

private:
    struct AVLTreeNode {
        AVLTreeNode() = default;

        AVLTreeNode(const Key &key, const Value &value) : key(key), value(value) {}

        ~AVLTreeNode() = default;

        AVLTreeNode *lChild = nullptr;
        AVLTreeNode *rChild = nullptr;

        Key key;
        Value value;

        unsigned height = 1;
    };

    void PreOrderTraversalHelper(AVLTreeNode *root, Handler handler) {
        if (!root) return;
        handler(root->key, root->value);
        PreOrderTraversalHelper(root->lChild, handler);
        PreOrderTraversalHelper(root->rChild, handler);
    }

    void InOrderTraversalHelper(AVLTreeNode *root, Handler handler) {
        if (!root) return;
        InOrderTraversalHelper(root->lChild, handler);
        handler(root->key, root->value);
        InOrderTraversalHelper(root->rChild, handler);
    }

    void PostOrderTraversalHelper(AVLTreeNode *root, Handler handler) {
        if (!root) return;
        PostOrderTraversalHelper(root->lChild, handler);
        PostOrderTraversalHelper(root->rChild, handler);
        handler(root->key, root->value);
    }

    void ClearHelper(AVLTreeNode *root) {
        if (!root) return;
        if (root->lChild) ClearHelper(root->lChild);
        if (root->rChild) ClearHelper(root->rChild);
        delete root;
    }

    AVLTreeNode *InsertHelper(AVLTreeNode *root, const Key &key, const Value &value) {
        if (!root) {
            return new AVLTreeNode(key, value);
        }

        if (key < root->key) {
            root->lChild = InsertHelper(root->lChild, key, value);
        } else if (key > root->key) {
            root->rChild = InsertHelper(root->rChild, key, value);
        } else {
            // Duplicate keys are not allowed in AVL tree
            return root;
        }

        root->height = 1 + std::max(HeightHelper(root->lChild), HeightHelper(root->rChild));

        int balance = Balance(root);

        // Left-Left case
        if (balance > 1 && key < root->lChild->key) {
            return RotateRight(root);
        }

        // Right-Right case
        if (balance < -1 && key > root->rChild->key) {
            return RotateLeft(root);
        }

        // Left-Right case
        if (balance > 1 && key > root->lChild->key) {
            root->lChild = RotateLeft(root->lChild);
            return RotateRight(root);
        }

        // Right-Left case
        if (balance < -1 && key < root->rChild->key) {
            root->rChild = RotateRight(root->rChild);
            return RotateLeft(root);
        }

        return root;
    }

    AVLTreeNode *RemoveHelper(AVLTreeNode *root, const Key &key) {
        if (!root) {
            return root;
        }

        if (key < root->key) {
            root->lChild = RemoveHelper(root->lChild, key);
        } else if (key > root->key) {
            root->rChild = RemoveHelper(root->rChild, key);
        } else {
            if (!root->lChild || !root->rChild) {
                AVLTreeNode* temp = root->lChild ? root->lChild : root->rChild;
                delete root;
                return temp;
            }

            AVLTreeNode* temp = MinimumHelper(root->rChild);
            root->key = temp->key;
            root->rChild = RemoveHelper(root->rChild, temp->key);
        }

        root->height = 1 + std::max(HeightHelper(root->lChild), HeightHelper(root->rChild));

        int balance = Balance(root);

        // Left-Left case
        if (balance > 1 && Balance(root->lChild) >= 0) {
            return RotateRight(root);
        }

        // Left-Right case
        if (balance > 1 && Balance(root->lChild) < 0) {
            root->lChild = RotateLeft(root->lChild);
            return RotateRight(root);
        }

        // Right-Right case
        if (balance < -1 && Balance(root->rChild) <= 0) {
            return RotateLeft(root);
        }

        // Right-Left case
        if (balance < -1 && Balance(root->rChild) > 0) {
            root->rChild = RotateRight(root->rChild);
            return RotateLeft(root);
        }

        return root;
    }

    AVLTreeNode *SearchHelper(AVLTreeNode *root, const Key &key) {
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

    AVLTreeNode *MinimumHelper(AVLTreeNode *root) {
        if (!root) return nullptr;
        while (root->lChild) root = root->lChild;
        return root;
    }

    AVLTreeNode *MaximumHelper(AVLTreeNode *root) {
        if (!root) return nullptr;
        while (root->rChild) root = root->rChild;
        return root;
    }

    AVLTreeNode *RotateRight(AVLTreeNode *root) {
        auto pivot = root->lChild;
        auto orphan = pivot->rChild;

        pivot->rChild = root;
        root->lChild = orphan;

        root->height = 1 + std::max(HeightHelper(root->lChild), HeightHelper(root->rChild));
        pivot->height = 1 + std::max(HeightHelper(pivot->lChild), HeightHelper(pivot->rChild));

        return pivot;
    }

    AVLTreeNode *RotateLeft(AVLTreeNode *root) {
        auto pivot = root->rChild;
        auto orphan = pivot->lChild;

        pivot->lChild = root;
        root->rChild = orphan;

        root->height = 1 + std::max(HeightHelper(root->lChild), HeightHelper(root->rChild));
        pivot->height = 1 + std::max(HeightHelper(pivot->lChild), HeightHelper(pivot->rChild));

        return pivot;
    }

    unsigned HeightHelper(const AVLTreeNode *root) {
        if (!root) return 0;
        return root->height;
    }

    unsigned SizeHelper(const AVLTreeNode *root) {
        if (!root) return 0;
        return 1 + SizeHelper(root->lChild) + SizeHelper(root->rChild);
    }

    int Balance(const AVLTreeNode *root) {
        if (!root) return 0;
        return HeightHelper(root->lChild) - HeightHelper(root->rChild);
    }

    AVLTreeNode *mRoot = nullptr;

public:
    AVLTree() = default;

    ~AVLTree() {
        Clear();
    }

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
        ClearHelper(mRoot);
        mRoot = nullptr;
    }

    void Insert(const Key &key, const Value &value) {
        mRoot = InsertHelper(mRoot, key, value);
    }

    void Remove(const Key &key) {
        mRoot = RemoveHelper(mRoot, key);
    }

    AVLTreeNode *Search(const Key &key) {
        return SearchHelper(mRoot, key);
    }

    AVLTreeNode *Minimum() {
        return MinimumHelper(mRoot);
    }

    AVLTreeNode *Maximum() {
        return MaximumHelper(mRoot);
    }

    unsigned Height() {
        return HeightHelper(mRoot);
    }

    unsigned Size() {
        return SizeHelper(mRoot);
    }
};