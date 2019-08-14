HW_SOURCE_FILE = 'hw04.py'

###############
#  Questions  #
###############
from math import sqrt, trunc

def intersection(st, ave):
    """Represent an intersection using the Cantor pairing function."""
    return (st+ave)*(st+ave+1)//2 + ave

def street(inter):
    return w(inter) - avenue(inter)

def avenue(inter):
    return inter - (w(inter) ** 2 + w(inter)) // 2

w = lambda z: int(((8*z+1)**0.5-1)/2)

def taxicab(a, b):
    """Return the taxicab distance between two intersections.

    >>> times_square = intersection(46, 7)
    >>> ess_a_bagel = intersection(51, 3)
    >>> taxicab(times_square, ess_a_bagel)
    9
    >>> taxicab(ess_a_bagel, times_square)
    9
    """
    sa, sb = street(a), street(b)
    aa, ab = avenue(a), avenue(b)

    return abs(sa - sb) + abs(aa - ab)

def squares(s):
    """Returns a new list containing square roots of the elements of the
    original list that are perfect squares.

    >>> seq = [8, 49, 8, 9, 2, 1, 100, 102]
    >>> squares(seq)
    [7, 3, 1, 10]
    >>> seq = [500, 30]
    >>> squares(seq)
    []
    """   
    squareLists = []
    for ele in s:
        temp = sqrt(ele)
        if trunc(temp) == temp : squareLists.append(trunc(temp))

    return squareLists

def g(n):
    """Return the value of G(n), computed recursively.

    >>> g(1)
    1
    >>> g(2)
    2
    >>> g(3)
    3
    >>> g(4)
    10
    >>> g(5)
    22
    """
    if n <= 3: return n

    return g(n - 1) + 2 * g(n - 2) + 3 * g(n - 3)

def g_iter(n):
    """Return the value of G(n), computed iteratively.

    >>> g_iter(1)
    1
    >>> g_iter(2)
    2
    >>> g_iter(3)
    3
    >>> g_iter(4)
    10
    >>> g_iter(5)
    22
    """
    if n <= 3: return n
    
    i, a, b, c = 4, 3, 2, 1

    while i <= n:
        a, b, c = a + 2 * b + 3 * c, a, b
        i += 1

    return a

def pingpong(n):
    """Return the nth element of the ping-pong sequence.

    >>> pingpong(7)
    7
    >>> pingpong(8)
    6
    >>> pingpong(15)
    1
    >>> pingpong(21)
    -1
    >>> pingpong(22)
    0
    >>> pingpong(30)
    6
    >>> pingpong(68)
    2
    >>> pingpong(69)
    1
    >>> pingpong(70)
    0
    >>> pingpong(71)
    1
    >>> pingpong(72)
    0
    >>> pingpong(100)
    2
    """
    i, res, dire = 1, 0, 1
    while i <= n:
        res += dire
        if has_seven(i) or i % 7 == 0: dire *= -1
        i += 1

    return res
    

def has_seven(k):
    """Returns True if at least one of the digits of k is a 7, False otherwise.

    >>> has_seven(3)
    False
    >>> has_seven(7)
    True
    >>> has_seven(2734)
    True
    >>> has_seven(2634)
    False
    >>> has_seven(734)
    True
    >>> has_seven(7777)
    True
    """
    if k % 10 == 7:
        return True
    elif k < 10:
        return False
    else:
        return has_seven(k // 10)

def count_change(amount):
    """Return the number of ways to make change for amount.

    >>> count_change(7)
    6
    >>> count_change(10)
    14
    >>> count_change(20)
    60
    >>> count_change(100)
    9828
    """
    def coins(n, num = 1):
        if num > n: return []
        elif num == n: return [num]

        return [num] + coins(n, num * 2)

    def count(amount, coins):
        if amount == 0: return 1
        elif amount < 0: return 0
        elif len(coins) == 0: return 0

        coin = coins[0]
        with_coin = count(amount - coin, coins)
        without_coin = count(amount, coins[1:])

        return with_coin + without_coin

    return count(amount, coins(amount))