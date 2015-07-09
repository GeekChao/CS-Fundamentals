/*Suppose a subclass inherits a "public static final" constant from a superclass,and implements a Java interface that contains a "public static final" constant
with the same name.
(a)  Will Java compile the result?  Does it make any difference whether the constant in the superclass and the constant in the interface have the
     same value?
     Answer: it works.
(b)  Write a main() method in the subclass that accesses the constant using the same name used in the superclass and the Java interface.  Will Java
     compile the result?  Does it make any difference whether the constant in the superclass and the constant in the interface have the same value?
     Answer:  error: reference to a is ambiguous, both variable a in X and variable a in I match, whatever their value are.
(c)  Figure out how to modify your main() method so that it accesses and prints one of the two conflicting values.  (Look to the Lecture 9 notes for
     clues.)  Make sure your code compiles and runs without errors.

     */
package partThree;
import java.io.*;

interface I{
	public static final int a = 1;
}

class X{
	public static final int a = 2;
}

class Y extends X implements I{
	public static void main(String[] args){
		System.out.println("Class X.a:" + X.a + "Interface I.a:" + I.a);
	}
}