import java.io.*;

class Prime {

	public static void main(String[] args) throws Exception {
		BufferedReader keyboad = new BufferedReader(new InputStreamReader(System.in));

		System.out.println("Type ant integer n, the programe will print all prime upto n.");
		int n = Integer.parseInt(keyboad.readLine());
		printPrime(n);
	}

	public static void printPrime(int n) {
		boolean[] primes = new boolean[n+1];
		for (int i = 2; i <= n; i++) {
			primes[i] = true;
		}

		for (int divisor = 2; divisor * divisor <= n; divisor++) {// if n not prime, then n = a*b, then a or b < sqrt(n) 
			if (primes[divisor]) {
				for(int i = 2*divisor; i <= n; i = i + divisor) {
					primes[i] = false;
				}
			}
		}

		for (int i = 2; i <= n; i++) {
			if (primes[i]) {
				System.out.println(i);
			}
		}

	}
}