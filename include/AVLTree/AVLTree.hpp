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

        AVLTreeNode(const AVLTreeNode &) = delete;

        AVLTreeNode(AVLTreeNode &&) = delete;

        AVLTreeNode &operator=(const AVLTreeNode &) = delete;

        AVLTreeNode &operator=(AVLTreeNode &&) = delete;

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

    unsigned HeightHelper(const AVLTreeNode *root) {
        if (!root) return 0;
        return root->height;
    }

    unsigned SizeHelper(const AVLTreeNode *root) {
        if (!root) return 0;
        return SizeHelper(root->lChild) + SizeHelper(root->rChild) + 1;
    }

    AVLTreeNode *InsertHelper(AVLTreeNode *root, const Key &key, const Value &value) {
        if (!root) return new AVLTreeNode(key, value);
        if (key < root->key)
            root->lChild = InsertHelper(root->lChild, key, value);
        else if (key > root->key)
            root->rChild = InsertHelper(root->rChild, key, value);

        root->height = std::max(HeightHelper(root->lChild), HeightHelper(root->rChild)) + 1;

        if (Balance(root) > 1) {
            if (key < root->lChild->key) {
                return RotateRight(root);
            } else if (key > root->lChild->key) {
                root->lChild = RotateLeft(root->lChild);
                return RotateRight(root);
            }
        } else if (Balance(root) < -1) {
            if (key > root->rChild->key) {
                return RotateLeft(root);
            } else if (key < root->rChild->key) {
                root->rChild = RotateRight(root->rChild);
                return RotateLeft(root);
            }
        }

        return root;
    }

    AVLTreeNode *RemoveHelper(AVLTreeNode *root, const Key &key) {
        if (!root) return nullptr;
        else if (key < root->key)
            root->lChild = RemoveHelper(root->lChild, key);
        else if (key > root->key)
            root->rChild = RemoveHelper(root->rChild, key);
        else {
            if (!root->lChild && !root->rChild) {
                delete root;
                root = nullptr;
            } else if (!root->lChild) {
                AVLTreeNode *tmp{root};
                root = root->rChild;
                delete tmp;
            } else if (!root->rChild) {
                AVLTreeNode *tmp{root};
                root = root->lChild;
                delete tmp;
            } else {
                AVLTreeNode *min{MinimumHelper(root->rChild)};
                root->key = min->key;
                root->value = min->value;
                root->rChild = RemoveHelper(root->rChild, min->key);
                //AVLTreeNode * max{ MaximumHelper(root->lChild) };
                //root->key = max->key;
                //root->value = max->value;
                //root->lChild = RemoveHelper(root->lChild, max->key);
            }
        }

        if (!root) return nullptr;

        root->height = std::max(HeightHelper(root->lChild), HeightHelper(root->rChild)) + 1;

        if (Balance(root) > 1) {
            if (Balance(root->lChild) >= 0) {
                return RotateRight(root);
            } else {
                root->lChild = RotateLeft(root->lChild);
                return RotateRight(root);
            }
        } else if (Balance(root) < -1) {
            if (Balance(root->rChild) <= 0) {
                return RotateLeft(root);
            } else {
                root->rChild = RotateRight(root->rChild);
                return RotateLeft(root);
            }
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

    void ClearHelper(AVLTreeNode *root) {
        if (!root) return;
        if (root->lChild) ClearHelper(root->lChild);
        if (root->rChild) ClearHelper(root->rChild);
        delete root;
    }

    int Balance(const AVLTreeNode *root) {
        if (!root) return 0;
        return HeightHelper(root->lChild) - HeightHelper(root->rChild);
    }

    AVLTreeNode *RotateRight(AVLTreeNode *root) {
        AVLTreeNode *pivot = root->lChild;
        AVLTreeNode *orphan = pivot->rChild;

        pivot->rChild = root;
        root->lChild = orphan;

        root->height = std::max(HeightHelper(root->lChild), HeightHelper(root->rChild)) + 1;
        pivot->height = std::max(HeightHelper(pivot->lChild), HeightHelper(pivot->rChild)) + 1;

        return pivot;
    }

    AVLTreeNode *RotateLeft(AVLTreeNode *root) {
        AVLTreeNode *pivot = root->rChild;
        AVLTreeNode *orphan = pivot->lChild;

        pivot->lChild = root;
        root->rChild = orphan;

        root->height = std::max(HeightHelper(root->lChild), HeightHelper(root->rChild)) + 1;
        pivot->height = std::max(HeightHelper(pivot->lChild), HeightHelper(pivot->rChild)) + 1;

        return pivot;
    }

    AVLTreeNode *mRoot{nullptr};

public:

    AVLTree() = default;

    AVLTree(const AVLTree &) = delete;

    AVLTree(AVLTree &&) = delete;

    AVLTree &operator=(const AVLTree &) = delete;

    AVLTree &operator=(AVLTree &&) = delete;

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

    void Insert(const Key &key, const Value &value) {
        mRoot = InsertHelper(mRoot, key, value);
    }

    void Remove(const Key &key) {
        mRoot = RemoveHelper(mRoot, key);
    }

    AVLTreeNode *Search(const Key &key) {
        return SearchHelper(mRoot, key);
    }

    void Clear() {
        ClearHelper(mRoot);
        mRoot = nullptr;
    }
};