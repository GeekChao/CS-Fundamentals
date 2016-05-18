class Human {//class definition

	public static int numberOfHumans;//static field, class variable,  shared whole calss
	// System.in and System.out are static fields
	public int age;//public means other class can read
	public String name;// field, instance variable

	public Human() {//default constructor from java 
		this.age = 0;
		this.name = "untitled";
		numberOfHumans++;
	}

	public Human(String givenName) {// constructor: same name; no return
		age = 6;
		name = givenName;
		numberOfHumans++;
	}

	public void introduce() {//method definition
		System.out.println("I'm " + name + " and I'm " + age + " years old.");
	}

	public void copy(Human original) {
		age = original.age;
		name = original.name;
	}
	
	public void change(int age) {
		String name = "Tom";
		this.age = age; //using "this" since local variable has priority
	}

	public static void printHumans() {
		//static method doesn't implicitly pass an object as a parameter
		System.out.println(numberOfHumans);
	}
}