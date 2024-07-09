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
public:
    avl_tree_t() = default;

    void pre_order_traversal(handler_t<key_t, value_t> handler) {
        pre_order_traversal_helper(m_root, handler);
    }

    void in_order_traversal(handler_t<key_t, value_t> handler) {
        in_order_traversal_helper(m_root, handler);
    }

    void post_order_traversal(handler_t<key_t, value_t> handler) {
        post_order_traversal_helper(m_root, handler);
    }

    void clear() {
        m_root.reset();
    }

    void insert(const key_t &key, const value_t &value) {
        m_root = insert_helper(m_root, key, value);
    }

    void remove(const key_t &key) {
        m_root = remove_helper(m_root, key);
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> search(const key_t &key) {
        return search_helper(m_root, key);
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> minimum() {
        return minimum_helper(m_root);
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> maximum() {
        return maximum_helper(m_root);
    }

    unsigned height() {
        return height_helper(m_root);
    }

    unsigned size() {
        return size_helper(m_root);
    }

private:
    void pre_order_traversal_helper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root, handler_t<key_t, value_t> handler) {
        if (!root) return;
        handler(root->key, root->value);
        pre_order_traversal_helper(root->lchild, handler);
        pre_order_traversal_helper(root->rchild, handler);
    }

    void in_order_traversal_helper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root, handler_t<key_t, value_t> handler) {
        if (!root) return;
        in_order_traversal_helper(root->lchild, handler);
        handler(root->key, root->value);
        in_order_traversal_helper(root->rchild, handler);
    }

    void post_order_traversal_helper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root, handler_t<key_t, value_t> handler) {
        if (!root) return;
        post_order_traversal_helper(root->lchild, handler);
        post_order_traversal_helper(root->rchild, handler);
        handler(root->key, root->value);
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> insert_helper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root, const key_t &key, const value_t &value) {
        if (!root) return std::make_shared<avl_tree_node_t<key_t, value_t>>(key, value);

        if (key < root->key) {
            root->lchild = insert_helper(root->lchild, key, value);
        } else if (key > root->key) {
            root->rchild = insert_helper(root->rchild, key, value);
        } else {
            return root;
        }

        root->height = 1 + std::max(height_helper(root->lchild), height_helper(root->rchild));

        if (balance(root) > 1 && key < root->lchild->key) {
            return rrorate(root);
        }

        if (balance(root) < -1 && key > root->rchild->key) {
            return lrotate(root);
        }

        if (balance(root) > 1 && key > root->lchild->key) {
            root->lchild = lrotate(root->lchild);
            return rrorate(root);
        }

        if (balance(root) < -1 && key < root->rchild->key) {
            root->rchild = rrorate(root->rchild);
            return lrotate(root);
        }

        return root;
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> remove_helper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root, const key_t &key) {
        if (!root) return nullptr;

        if (key < root->key) {
            root->lchild = remove_helper(root->lchild, key);
        } else if (key > root->key) {
            root->rchild = remove_helper(root->rchild, key);
        } else {
            if (!root->lchild || !root->rchild) {
                std::shared_ptr<avl_tree_node_t<key_t, value_t>> temp = root->lchild ? root->lchild : root->rchild;
                root.reset();
                return temp;
            }

            std::shared_ptr<avl_tree_node_t<key_t, value_t>> temp = minimum_helper(root->rchild);
            root->key = temp->key;
            root->rchild = remove_helper(root->rchild, temp->key);
        }

        root->height = 1 + std::max(height_helper(root->lchild), height_helper(root->rchild));

        if (balance(root) > 1 && balance(root->lchild) >= 0) {
            return rrorate(root);
        }

        if (balance(root) > 1 && balance(root->lchild) < 0) {
            root->lchild = lrotate(root->lchild);
            return rrorate(root);
        }

        if (balance(root) < -1 && balance(root->rchild) <= 0) {
            return lrotate(root);
        }

        if (balance(root) < -1 && balance(root->rchild) > 0) {
            root->rchild = rrorate(root->rchild);
            return lrotate(root);
        }

        return root;
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> search_helper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root, const key_t &key) {
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

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> minimum_helper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root) {
        if (!root) return nullptr;
        while (root->lchild) root = root->lchild;
        return root;
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> maximum_helper(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root) {
        if (!root) return nullptr;
        while (root->rchild) root = root->rchild;
        return root;
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> rrorate(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root) {
        auto pivot = root->lchild;
        auto orphan = pivot->rchild;

        pivot->rchild = root;
        root->lchild = orphan;

        root->height = 1 + std::max(height_helper(root->lchild), height_helper(root->rchild));
        pivot->height = 1 + std::max(height_helper(pivot->lchild), height_helper(pivot->rchild));

        return pivot;
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> lrotate(std::shared_ptr<avl_tree_node_t<key_t, value_t>> root) {
        auto pivot = root->rchild;
        auto orphan = pivot->lchild;

        pivot->lchild = root;
        root->rchild = orphan;

        root->height = 1 + std::max(height_helper(root->lchild), height_helper(root->rchild));
        pivot->height = 1 + std::max(height_helper(pivot->lchild), height_helper(pivot->rchild));

        return pivot;
    }

    unsigned height_helper(const std::shared_ptr<avl_tree_node_t<key_t, value_t>> root) const {
        if (!root) return 0;
        return root->height;
    }

    unsigned size_helper(const std::shared_ptr<avl_tree_node_t<key_t, value_t>> root) {
        if (!root) return 0;
        return 1 + size_helper(root->lchild) + size_helper(root->rchild);
    }

    int balance(const std::shared_ptr<avl_tree_node_t<key_t, value_t>> root) {
        if (!root) return 0;
        return height_helper(root->lchild) - height_helper(root->rchild);
    }

    std::shared_ptr<avl_tree_node_t<key_t, value_t>> m_root;
};
