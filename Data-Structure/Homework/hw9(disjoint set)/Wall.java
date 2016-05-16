/*Wall.java*/

public class Wall{
	private int x;
	private int y;
	private int type;
	public static final int HORIZ_WALL = 0;
	public static final int VERT_WALL = 1;

	public Wall(int x, int y, int type){
		this.x = x;
		this.y = y;
		this.type = type;
	}

	public int getX(){
		return x;
	}

	public int getY(){
		return y;
	}

	public int getType(){
		return type;
	}

	public String toString(){
		return "X: " + x + " Y: " + y + " Type: " + type + "\n";
	}
}