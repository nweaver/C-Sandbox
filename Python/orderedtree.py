#!/usr/bin/env python3

from tree import TreeNode
import random


class OrderedTree:

    @staticmethod
    def rotate_left(node):
        new_top = node.right
        node.right = new_top.left
        new_top.left = node
        new_top.left.update_height()
        new_top.update_height()
        return new_top

    @staticmethod
    def rotate_right(node):
        new_top = node.left
        node.left = new_top.right
        new_top.right = node
        new_top.right.update_height()
        new_top.update_height()
        return new_top

    def __init__(self, *args):
        self.tree = None
        for a in args:
            for data in a:
                self.avl_insert(data)

    def avl_insert(self, data):
        def avl_insert_internal(node):
            if node is None:
                return TreeNode(data, None, None)
            elif data < node.data:
                node.left = avl_insert_internal(node.left)
            else:
                node.right = avl_insert_internal(node.right)
            node.update_height()
            return OrderedTree.avl_rebalance(node)
        self.tree = avl_insert_internal(self.tree)

    @staticmethod
    def avl_rebalance(node):
        if abs(node.balance()) < 2:
            return node
        if node.balance() == 2:
            # Right heavy.  Is our right subtree left heavy?
            if node.right.balance() == -1:
                node.right = OrderedTree.rotate_right(node.right)
            return OrderedTree.rotate_left(node)
        if node.balance() == -2:
            if node.left.balance() == 1:
                node.left = OrderedTree.rotate_left(node.left)
            return OrderedTree.rotate_right(node)

    def __repr__(self):
        return repr(self.tree)

    def is_ordered(self):
        if self.tree is None:
            return True
        last = None
        for x in self.tree:
            if last is None or last <= x:
                last = x
            else:
                return False
        return True

    def assert_correct_balance(self):
        def check_balance(node):
            if node is None:
                return
            balance = node.balance()
            if node.left:
                node.left.update_height()
                check_balance(node.left)
            if node.right:
                node.right.update_height()
                check_balance(node.right)
            node.update_height()
            assert balance == node.balance()

        check_balance(self.tree)

    def __contains__(self, data):
        def internal(tree):
            if tree is None:
                return False
            elif tree.data == data:
                return True
            elif data < tree.data:
                return internal(tree.left)
            return internal(tree.right)
        return internal(self.tree)


def avl_testing():
    test = OrderedTree([1, 2, 3, 4, 5, 6, 7, 8, 9])
    print(test)
    test.assert_correct_balance()


def full_test():
    avl_testing()
    alphabet = "ABCDEFGHIJK"
    data = []
    for x in alphabet:
        data.append(x)
    random.shuffle(data)
    test = OrderedTree(data)
    print(repr(test))
    i = 0
    for x in test.tree:
        assert x == alphabet[i]
        i += 1
    test.assert_correct_balance()

    print(repr(OrderedTree(alphabet)))

    data = []
    for x in range(0, 80, 2):
        data.append(x)
    random.shuffle(data)
    test = OrderedTree(data)
    print(repr(test))
    for x in range(-1, 80, 1):
        if x % 2 == 0:
            assert x in test
        else:
            assert x not in test
    assert test.is_ordered()
    test.assert_correct_balance()


if __name__ == "__main__":
    full_test()
