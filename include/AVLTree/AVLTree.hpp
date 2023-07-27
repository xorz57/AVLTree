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

        AVLTreeNode *left = nullptr;
        AVLTreeNode *right = nullptr;

        Key key;
        Value value;

        unsigned height = 1;
    };

    void PreOrderTraversal(AVLTreeNode *root, Handler handler) {
        if (!root) return;
        handler(root->key, root->value);
        PreOrderTraversal(root->left, handler);
        PreOrderTraversal(root->right, handler);
    }

    void InOrderTraversal(AVLTreeNode *root, Handler handler) {
        if (!root) return;
        InOrderTraversal(root->left, handler);
        handler(root->key, root->value);
        InOrderTraversal(root->right, handler);
    }

    void PostOrderTraversal(AVLTreeNode *root, Handler handler) {
        if (!root) return;
        PostOrderTraversal(root->left, handler);
        PostOrderTraversal(root->right, handler);
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
            if (current->left) queue.push(current->left);
            if (current->right) queue.push(current->right);
        }
    }

    AVLTreeNode *Minimum(AVLTreeNode *root) {
        if (!root) return nullptr;
        while (root->left) root = root->left;
        return root;
    }

    AVLTreeNode *Maximum(AVLTreeNode *root) {
        if (!root) return nullptr;
        while (root->right) root = root->right;
        return root;
    }

    int Balance(const AVLTreeNode *root) {
        if (!root) return 0;
        return Height(root->left) - Height(root->right);
    }

    unsigned Height(const AVLTreeNode *root) {
        if (!root) return 0;
        return root->height;
    }

    unsigned Size(const AVLTreeNode *root) {
        if (!root) return 0;
        return Size(root->left) + Size(root->right) + 1;
    }

    AVLTreeNode *RotateRight(AVLTreeNode *root) {
        AVLTreeNode *pivot = root->left;
        AVLTreeNode *orphan = pivot->right;

        pivot->right = root;
        root->left = orphan;

        root->height = std::max(Height(root->left), Height(root->right)) + 1;
        pivot->height = std::max(Height(pivot->left), Height(pivot->right)) + 1;

        return pivot;
    }

    AVLTreeNode *RotateLeft(AVLTreeNode *root) {
        AVLTreeNode *pivot = root->right;
        AVLTreeNode *orphan = pivot->left;

        pivot->left = root;
        root->right = orphan;

        root->height = std::max(Height(root->left), Height(root->right)) + 1;
        pivot->height = std::max(Height(pivot->left), Height(pivot->right)) + 1;

        return pivot;
    }

    AVLTreeNode *Insert(AVLTreeNode *root, const Key &key, const Value &value) {
        if (!root) return new AVLTreeNode(key, value);
        if (key < root->key) root->left = Insert(root->left, key, value);
        else if (key > root->key) root->right = Insert(root->right, key, value);

        root->height = std::max(Height(root->left), Height(root->right)) + 1;

        if (Balance(root) > 1) {
            if (key < root->left->key) {
                return RotateRight(root);
            } else if (key > root->left->key) {
                root->left = RotateLeft(root->left);
                return RotateRight(root);
            }
        } else if (Balance(root) < -1) {
            if (key > root->right->key) {
                return RotateLeft(root);
            } else if (key < root->right->key) {
                root->right = RotateRight(root->right);
                return RotateLeft(root);
            }
        }

        return root;
    }

    AVLTreeNode *Remove(AVLTreeNode *root, const Key &key) {
        if (!root) return nullptr;
        else if (key < root->key) root->left = Remove(root->left, key);
        else if (key > root->key) root->right = Remove(root->right, key);
        else {
            if (!root->left && !root->right) {
                delete root;
                root = nullptr;
            } else if (!root->left) {
                AVLTreeNode *tmp{root};
                root = root->right;
                delete tmp;
            } else if (!root->right) {
                AVLTreeNode *tmp{root};
                root = root->left;
                delete tmp;
            } else {
                AVLTreeNode *min{Minimum(root->right)};
                root->key = min->key;
                root->value = min->value;
                root->right = Remove(root->right, min->key);
                //AVLTreeNode * max{ Maximum(root->left) };
                //root->key = max->key;
                //root->value = max->value;
                //root->left = Remove(root->left, max->key);
            }
        }

        if (!root) return nullptr;

        root->height = std::max(Height(root->left), Height(root->right)) + 1;

        if (Balance(root) > 1) {
            if (Balance(root->left) >= 0) {
                return RotateRight(root);
            } else {
                root->left = RotateLeft(root->left);
                return RotateRight(root);
            }
        } else if (Balance(root) < -1) {
            if (Balance(root->right) <= 0) {
                return RotateLeft(root);
            } else {
                root->right = RotateRight(root->right);
                return RotateLeft(root);
            }
        }

        return root;
    }

    AVLTreeNode *Search(AVLTreeNode *root, const Key &key) {
        while (root) {
            if (key > root->key) {
                root = root->right;
            } else if (key < root->key) {
                root = root->left;
            } else {
                return root;
            }
        }
        return nullptr;
    }

    void Clear(AVLTreeNode *root) {
        if (!root) return;
        if (root->left) Clear(root->left);
        if (root->right) Clear(root->right);
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