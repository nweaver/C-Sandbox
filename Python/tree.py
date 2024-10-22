#!/usr/bin/env python3

from collections import deque

def get_height(node):
    if node is None:
        return 0
    return node.height

class TreeNode:
    def __init__(self, data, left, right):
        self.data = data
        self.left = left
        self.right = right
        self.height = 1

    def update_height(self):
        self.height = 1 + max(get_height(self.left), get_height(self.right))

    def balance(self):
        return(get_height(self.right) - get_height(self.left))

    def __repr__(self):
        res = "("
        if self.left:
            res += repr(self.left) + " "
        res += repr(self.data) + "/b:{}".format(self.balance())
        if self.right:
            res += " " + repr(self.right)
        return res + ")"

    def _nodes(self):
        """ An internal iterator that returns nodes rather than the data"""
        yield self
        if self.left != None:
            if isinstance(self.left, TreeNode):
                yield from self.left._nodes()
            else:
                yield self.left
        if self.right is not None:
            if isinstance(self.right, TreeNode):
                yield from self.right._nodes()
            else:
                yield self.right

        
    def is_well_formed(self):
        """ Returns true if this is a proper tree, false otherwise.
        Useful for a lot of assertions"""
        s = set()
        for x in self._nodes():
            if x in s:
                return False
            if not isinstance(x, TreeNode):
                return False
            s.add(x)
        return True

    def __bool__(self):
        return True

    # This is an in-order iteration
    def __iter__(self):
        assert self.is_well_formed()
        if self.left:
            yield from self.left
        yield self.data
        if self.right:
            yield from self.right

    # THis is the pre-order iteration
    def preorder(self):
        assert self.is_well_formed()
        yield self.data
        if self.left:
            yield from self.left.preorder()
        if self.right:
            yield from self.right.preorder()

    # THis is the post-order iteration
    def postorder(self):
        assert self.is_well_formed()
        if self.left:
            yield from self.left.postorder()
        if self.right:
            yield from self.right.postorder()
        yield self.data

    # This is the level order
    def levelorder(self):
        assert self.is_well_formed()
        queue = deque()
        # A double ended queue has a "Left" and "Right"
        queue.append(self)
        while len(queue) > 0:
            at = queue.popleft()
            yield at.data
            if at.left is not None:
                queue.append(at.left)
            if at.right is not None:
                queue.append(at.right)
            
            
def test_tree():
    return TreeNode("A",
                    TreeNode("B",
                             TreeNode("D", None,
                                      TreeNode("H", None, None)),
                             TreeNode("E",
                                      TreeNode("I", None, None),
                                      TreeNode("J", None,
                                               TreeNode("K", None, None)))),
                    TreeNode("C",
                             TreeNode("F", None, None),
                             TreeNode("G", None, None)))

if __name__ == "__main__":
    print(repr(test_tree()))
    t = test_tree()
    print("In order    " + ", ".join(t))
    print("Preorder    " + ", ".join(t.preorder()))
    print("Postorder   " + ", ".join(t.postorder()))
    print("Level order " + ", ".join(t.levelorder()))
    assert t.is_well_formed()
    broken = test_tree()
    broken.left.right = broken.left.left
    assert not broken.is_well_formed()
    broken = test_tree()
    broken.left.right = broken
    assert not broken.is_well_formed()
    broken = test_tree()
    broken.left.right = 32
    assert not broken.is_well_formed()
    

