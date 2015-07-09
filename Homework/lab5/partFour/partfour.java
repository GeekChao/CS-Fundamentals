package partFour;
import java.io.*;

class X{
	public void print(int i){
		System.out.println("X:" + i);
	}
}

class Y extends X{
	public void print(int i){
		System.out.println("Y:" + i);
	}
	public static void main(String[] args){
		Y y = new Y();
		((X)y).print(1); // calls the subclass method.

		/*X x = new X();
		((Y)x).print(2);*/	//ClassCastException: partFour.X cannot be cast to partFour.Y

		Y yy = new Y();
		yy.print(2);
	}
}