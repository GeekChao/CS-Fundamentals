public class MyGem{

	private double total = 0.0;

	private double item(double r, int N){
		if(N == 0)	return 1.0;

		return item(r, N-1) * r;
	}

	public double sum(double r, int n){
		total += item(r, n);
		if(n == 0) return 0.0;
		n--;
		sum(r, n);		
		return total;
	}

	public static void main(String[] args){
		MyGem gem = new MyGem();
		System.out.println("Total: " + gem.sum(1.0/2, 3));
	}
}