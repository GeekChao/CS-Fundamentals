public class UseHuman {

	public static void main(String[] ags) {// main() method always static method
		Human amanda = new Human();//default constructor from java
		amanda.age = 6;
		amanda.name = "Amanda";
		amanda.introduce();// implicitly passes an object "this" copy from amanda as a parameter which is local variable in introduce() method

		Human vishi = new Human("vishi");
		amanda.copy(vishi);
		amanda.change(11);
		amanda.introduce();

		int kids = Human.numberOfHumans;//good behave
		int kids2 = amanda.numberOfHumans;//bad behave

		System.out.println("Human.numberOfHumans = " + kids + ", good behave! " + "amanda.numberOfHumans " + kids2 + " bad behave!!!");
		System.out.println("IMPORTANT: YOU CANNOT CHANGE THE VALUE OF \"this\"!");
		System.out.println("IMPORTANT: in \"static\" method, THERE IS NO \"this\"!");

	}
}