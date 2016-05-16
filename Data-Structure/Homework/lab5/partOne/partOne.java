/*What about arrays of objects?  Suppose xa is an array of X’s, and ya is an array of Y’s.
	(a)At compile-time, can we assign xa to ya, and vice versa?  When is a cast required?  
		Answer: it works at the same time.
	(b)At run-time, if ya references an array of Y’s, can we assign it to xa? Can we then assign it back from xa to ya?
	*/
package partOne;

class X{
	protected String str;
}

class Y extends X{

}

class Z{
	public static void main(String[] args){
		//X[] xa = new X[10];
		X[] xa;
		Y[] ya = new Y[10];

		xa = ya;
		ya = (Y[])xa;
	}
}