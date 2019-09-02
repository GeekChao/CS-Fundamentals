from lab07 import *

# Q6
def cumulative_sum(t):
    """Mutates t where each node's root becomes the sum of all entries in the
    corresponding subtree rooted at t.

    >>> t = Tree(1, [Tree(3, [Tree(5)]), Tree(7)])
    >>> cumulative_sum(t)
    >>> t
    Tree(16, [Tree(8, [Tree(5)]), Tree(7)])
    """
    def traverse(t):
        if t.is_leaf():
            return t

        total = sum(map(lambda t: t.label, [traverse(b) for b in t.branches]))
        t.label = t.label + total

        return t
    
    traverse(t)

# Q7
def reverse_other(t):
    """Reverse the entries of every other level of the tree using mutation.

    >>> t = Tree(1, [Tree(2), Tree(3), Tree(4)])
    >>> reverse_other(t)
    >>> t
    Tree(1, [Tree(4), Tree(3), Tree(2)])
    >>> t = Tree(1, [Tree(2, [Tree(5, [Tree(7), Tree(8)]), Tree(6)]), Tree(3)])
    >>> reverse_other(t)
    >>> t
    Tree(1, [Tree(3, [Tree(5, [Tree(8), Tree(7)]), Tree(6)]), Tree(2)])
    """ 
    def traverse(t, depth = 1):
        if t.is_leaf(): return 
        
        for b in t.branches:
            traverse(b, depth + 1)

        if depth % 2 == 1:
            i, length = 0, len(t.branches) - 1
            while i <= length / 2:
                couter = length - i
                if couter != i:
                    t.branches[couter].label, t.branches[i].label = t.branches[i].label, t.branches[couter].label
                i += 1

    traverse(t)

# Q8
def deep_map_mut(fn, link):
    """Mutates a deep link by replacing each item found with the
    result of calling fn on the item.  Does NOT create new Links (so
    no use of Link's constructor)

    Does not return the modified Link object.

    >>> link1 = Link(3, Link(Link(4), Link(5, Link(6))))
    >>> deep_map_mut(lambda x: x * x, link1)
    >>> print(link1)
    <9 <16> 25 36>
    """
    cur = link
    while cur is not Link.empty:
        if isinstance(cur.first, Link):
            cur.first.first = fn(cur.first.first)
        else:
            cur.first = fn(cur.first)

        cur = cur.rest

# Q9
def has_cycle(link):
    """Return whether link contains a cycle.

    >>> s = Link(1, Link(2, Link(3)))
    >>> s.rest.rest.rest = s
    >>> has_cycle(s)
    True
    >>> t = Link(1, Link(2, Link(3)))
    >>> has_cycle(t)
    False
    >>> u = Link(2, Link(2, Link(2)))
    >>> has_cycle(u)
    False
    """
    cur, track = link, []
    while cur is not Link.empty:
        if cur in track: return True

        track.append(cur)
        cur = cur.rest

    return False

def has_cycle_constant(link):
    """Return whether link contains a cycle.

    >>> s = Link(1, Link(2, Link(3)))
    >>> s.rest.rest.rest = s
    >>> has_cycle_constant(s)
    True
    >>> t = Link(1, Link(2, Link(3)))
    >>> has_cycle_constant(t)
    False
    """
# Official solution
    if link is Link.empty:
        return False
    slow, fast = link, link.rest
    while fast is not Link.empty:
        if fast.rest == Link.empty:
            return False
        elif fast == slow or fast.rest == slow:
            return True
        else:
            slow, fast = slow.rest, fast.rest.rest
    return False
