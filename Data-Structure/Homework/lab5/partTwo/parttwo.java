/*Suppose a subclass inherits a method implementation from a superclass, and implements a Java interface (that’s the "interface" keyword) that contains
a method with the same name and prototype.
	(a)  Will Java compile the result?  
		Answer: yes, it will.
	(b)  What if the method declaration in the interface has a different return type?
		Answer: error: print() in Y cannot override print() in X
	(c)  What if the method declaration in the interface has the same return type, but a signature with a different parameter type?
		Answer: it works.
	(d)  What if the method declaration in the interface has the same return type, and the same number of parameters and parameter types, but those 
	parameters have different names?
		Answer: it works.
*/

package partTwo;

interface I{
	//public void print(int);
	public int print(int i);
	//public void print(String i);
	//public void print(int j);
}

class X{
	public void print(int i){

	}
}

class Y extends X implements I{
	public int print(int i){

	}
	/*public void print(String str){

	}*/
	/*public void print(int j){

	}*/
	public static void main(String[] args){

	}
}
