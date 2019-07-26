""" Optional problems for Lab 3 """

from lab03 import *
from math import sqrt, floor

## Lambda expressions

def is_palindrome(n):
    """
    Fill in the blanks '_____' to check if a number
    is a palindrome.

    >>> is_palindrome(12321)
    True
    >>> is_palindrome(42)
    False
    >>> is_palindrome(2015)
    False
    >>> is_palindrome(55)
    True
    """
    x, y = n, 0
    f = lambda: y * 10 + x % 10
    while x > 0:
        x, y = x // 10, f()
    return y == n

## More recursion practice

def skip_mul(n):
    """Return the product of n * (n - 2) * (n - 4) * ...

    >>> skip_mul(5) # 5 * 3 * 1
    15
    >>> skip_mul(8) # 8 * 6 * 4 * 2
    384
    """
    if n == 1:
        return 1
    if n == 2:
        return 2
    else:
        return n * skip_mul(n - 2)

def is_prime(n):
    """Returns True if n is a prime number and False otherwise.

    >>> is_prime(2)
    True
    >>> is_prime(16)
    False
    >>> is_prime(521)
    True
    """
    if n <= 1: return False
    if n == 2: return True
    
    k = 2
    while k <= floor(sqrt(n)):
        if n % k == 0: return False
        k += 1
    return True

def interleaved_sum(n, odd_term, even_term):
    """Compute the sum odd_term(1) + even_term(2) + odd_term(3) + ..., up
    to n.

    >>> # 1 + 2^2 + 3 + 4^2 + 5
    ... interleaved_sum(5, lambda x: x, lambda x: x*x)
    29
    """
    def helper(n, sum = 0):
        if n == 0: return sum

        if n & 1 == 0: sum += even_term(n)
        else: sum += odd_term(n)

        return helper(n - 1, sum)

    return helper(n)

def ten_pairs(n):
    """Return the number of ten-pairs within positive integer n.

    >>> ten_pairs(7823952)
    3
    >>> ten_pairs(55055)
    6
    >>> ten_pairs(9641469)
    6
    """
    def count_digit(n, digit):
        if n == 0:
            return 0
        elif n % 10 == digit:
            return 1 + count_digit(n // 10, digit)
        else:
            return count_digit(n // 10, digit)

    if n < 10:  
        return 0
    
    return ten_pairs(n // 10) + count_digit(n // 10, 10 - n % 10)

