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
private:
    using Handler = std::function<void(const Key &, Value &)>;

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

    void PreOrderTraversal(AVLTreeNode *root, Handler handler) {
        if (!root) return;
        handler(root->key, root->value);
        PreOrderTraversal(root->lChild, handler);
        PreOrderTraversal(root->rChild, handler);
    }

    void InOrderTraversal(AVLTreeNode *root, Handler handler) {
        if (!root) return;
        InOrderTraversal(root->lChild, handler);
        handler(root->key, root->value);
        InOrderTraversal(root->rChild, handler);
    }

    void PostOrderTraversal(AVLTreeNode *root, Handler handler) {
        if (!root) return;
        PostOrderTraversal(root->lChild, handler);
        PostOrderTraversal(root->rChild, handler);
        handler(root->key, root->value);
    }

    void BreadthFirstTraversal(AVLTreeNode *root, Handler handler) {
        std::queue<AVLTreeNode *> queue;
        if (!root) return;
        queue.push(root);
        while (!queue.empty()) {
            AVLTreeNode *current{queue.front()};
            handler(current->key, current->value);
            queue.pop();
            if (current->lChild) queue.push(current->lChild);
            if (current->rChild) queue.push(current->rChild);
        }
    }

    AVLTreeNode *Minimum(AVLTreeNode *root) {
        if (!root) return nullptr;
        while (root->lChild) root = root->lChild;
        return root;
    }

    AVLTreeNode *Maximum(AVLTreeNode *root) {
        if (!root) return nullptr;
        while (root->rChild) root = root->rChild;
        return root;
    }

    int Balance(const AVLTreeNode *root) {
        if (!root) return 0;
        return Height(root->lChild) - Height(root->rChild);
    }

    unsigned Height(const AVLTreeNode *root) {
        if (!root) return 0;
        return root->height;
    }

    unsigned Size(const AVLTreeNode *root) {
        if (!root) return 0;
        return Size(root->lChild) + Size(root->rChild) + 1;
    }

    AVLTreeNode *RotateRight(AVLTreeNode *root) {
        AVLTreeNode *pivot = root->lChild;
        AVLTreeNode *orphan = pivot->rChild;

        pivot->rChild = root;
        root->lChild = orphan;

        root->height = std::max(Height(root->lChild), Height(root->rChild)) + 1;
        pivot->height = std::max(Height(pivot->lChild), Height(pivot->rChild)) + 1;

        return pivot;
    }

    AVLTreeNode *RotateLeft(AVLTreeNode *root) {
        AVLTreeNode *pivot = root->rChild;
        AVLTreeNode *orphan = pivot->lChild;

        pivot->lChild = root;
        root->rChild = orphan;

        root->height = std::max(Height(root->lChild), Height(root->rChild)) + 1;
        pivot->height = std::max(Height(pivot->lChild), Height(pivot->rChild)) + 1;

        return pivot;
    }

    AVLTreeNode *Insert(AVLTreeNode *root, const Key &key, const Value &value) {
        if (!root) return new AVLTreeNode(key, value);
        if (key < root->key)
            root->lChild = Insert(root->lChild, key, value);
        else if (key > root->key)
            root->rChild = Insert(root->rChild, key, value);

        root->height = std::max(Height(root->lChild), Height(root->rChild)) + 1;

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

    AVLTreeNode *Remove(AVLTreeNode *root, const Key &key) {
        if (!root) return nullptr;
        else if (key < root->key)
            root->lChild = Remove(root->lChild, key);
        else if (key > root->key)
            root->rChild = Remove(root->rChild, key);
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
                AVLTreeNode *min{Minimum(root->rChild)};
                root->key = min->key;
                root->value = min->value;
                root->rChild = Remove(root->rChild, min->key);
                //AVLTreeNode * max{ Maximum(root->lChild) };
                //root->key = max->key;
                //root->value = max->value;
                //root->lChild = Remove(root->lChild, max->key);
            }
        }

        if (!root) return nullptr;

        root->height = std::max(Height(root->lChild), Height(root->rChild)) + 1;

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

    AVLTreeNode *Search(AVLTreeNode *root, const Key &key) {
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

    void Clear(AVLTreeNode *root) {
        if (!root) return;
        if (root->lChild) Clear(root->lChild);
        if (root->rChild) Clear(root->rChild);
        delete root;
    }

    AVLTreeNode *mRoot{nullptr};

public:
    using Handler = std::function<void(const Key &, Value &)>;

    AVLTree() = default;

    AVLTree(const AVLTree &) = delete;

    AVLTree(AVLTree &&) = delete;

    AVLTree &operator=(const AVLTree &) = delete;

    AVLTree &operator=(AVLTree &&) = delete;

    ~AVLTree() {
        Clear();
    }

    void PreOrderTraversal(Handler handler) {
        PreOrderTraversal(mRoot, handler);
    }

    void InOrderTraversal(Handler handler) {
        InOrderTraversal(mRoot, handler);
    }

    void PostOrderTraversal(Handler handler) {
        PostOrderTraversal(mRoot, handler);
    }

    void BreadthFirstTraversal(Handler handler) {
        BreadthFirstTraversal(mRoot, handler);
    }

    AVLTreeNode *Minimum() {
        return Minimum(mRoot);
    }

    AVLTreeNode *Maximum() {
        return Maximum(mRoot);
    }

    unsigned Height() {
        return Height(mRoot);
    }

    unsigned Size() {
        return Size(mRoot);
    }

    void Insert(const Key &key, const Value &value) {
        mRoot = Insert(mRoot, key, value);
    }

    void Remove(const Key &key) {
        mRoot = Remove(mRoot, key);
    }

    AVLTreeNode *Search(const Key &key) {
        return Search(mRoot, key);
    }

    void Clear() {
        Clear(mRoot);
        mRoot = nullptr;
    }
};