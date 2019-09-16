# Extra Questions

from lab11 import *

# Q6
def hailstone(n):
    """
    Pick a positive integer n as the start.
    If n is even, divide it by 2.
    If n is odd, multiply it by 3 and add 1.
    Continue this process until n is 1.

    >>> for num in hailstone(10):
    ...     print(num)
    ...
    10
    5
    16
    8
    4
    2
    1
    """
    yield n
    try:
        if n == 1:
            return
        elif n % 2 == 0:
            yield from hailstone(n // 2)
        else:
            yield from hailstone(n * 3 + 1)
    except StopIteration:
        pass

# Q7
def repeated(t, k):
    """Return the first value in iterable T that appears K times in a row.

    >>> s = [3, 2, 1, 2, 1, 4, 4, 5, 5, 5]
    >>> repeated(trap(s, 7), 2)
    2
    >>> repeated(trap(s, 10), 3)
    5
    >>> print(repeated([4, None, None, None], 3))
    None
    """
    assert k > 1
    dict = {}

    for ele in t:
        if ele in dict:
            dict[ele] += 1
            if dict[ele] == k:
                return ele
        else:
            dict[ele] = 1
    
    raise ValueError('Not Found')

# Q8
def merge(s0, s1):
    """Yield the elements of strictly increasing iterables s0 and s1, removing
    repeats. Assume that s0 and s1 have no repeats. You can also assume that s0
    and s1 represent infinite sequences.

    >>> m = merge([0, 2, 4, 6, 8, 10, 12, 14], [0, 3, 6, 9, 12, 15])
    >>> type(m)
    <class 'generator'>
    >>> list(m)
    [0, 2, 3, 4, 6, 8, 9, 10, 12, 14, 15]
    >>> def big(n):
    ...    k = 0
    ...    while True: yield k; k += n
    >>> m = merge(big(2), big(3))
    >>> [next(m) for _ in range(11)]
    [0, 2, 3, 4, 6, 8, 9, 10, 12, 14, 15]
    """
    i0, i1 = iter(s0), iter(s1)
    e0, e1 = next(i0, None), next(i1, None)
    "*** YOUR CODE HERE ***"
    def compare(e0, e1):
        if e0 == None:
            yield e1
            yield from i1
        elif e1 == None:
            yield e0
            yield from i0
        elif e0 == e1:
            yield e0
            yield from compare(next(i0, None), next(i1, None))
        elif e0 < e1:
            yield e0
            yield from compare(next(i0, None), e1)
        elif e0 > e1:
            yield e1
            yield from compare(e0, next(i1, None))

    yield from compare(e0, e1)

# Q9
def remainders_generator(m):
    """
    Takes in an integer m, and yields m different remainder groups
    of m.

    >>> remainders_mod_four = remainders_generator(4)
    >>> for rem_group in remainders_mod_four:
    ...     for _ in range(3):
    ...         print(next(rem_group))
    0
    4
    8
    1
    5
    9
    2
    6
    10
    3
    7
    11
    """
    "*** YOUR CODE HERE ***"
    def infi_generator(remainder):
        i = 0
        while True:
            yield i * m + remainder
            i += 1

    try:
        for remainder in range(m):
            yield infi_generator(remainder)
    except StopIteration:
        pass

# Q10
def zip_generator(*iterables):
    """
    Takes in any number of iterables and zips them together.
    Returns a generator that outputs a series of lists, each
    containing the nth items of each iterable.
    >>> z = zip_generator([1, 2, 3], [4, 5, 6], [7, 8])
    >>> for i in z:
    ...     print(i)
    ...
    [1, 4, 7]
    [2, 5, 8]
    """
    "*** YOUR CODE HERE ***"
    iterArr = [iter(iterable) for iterable in iterables]
    result = []

    while True:
        result = [next(it, None) for it in iterArr]

        if None in result: return

        yield result
