import java.io.*;

class DrawPascalTriangle {
	public static void main(String[] args) throws Exception {
		BufferedReader keyboard = new BufferedReader(new InputStreamReader(System.in));

		System.out.print("Type integer N, the programe will draw an pascal triangle upto N row: ");
		int n = Integer.parseInt(keyboard.readLine());

		int[][] pt = pascalTriangle(n);

		printPascalTriangle(pt);
	}

	public static int[][] pascalTriangle (int n) {
		int[][] pt = new int[n+1][];
		for (int i = 0; i <= n; i++) {
			pt[i] = new int[i+1];
			pt[i][0] = 1; //leftmost for row i
			for (int j = 1; j < i; j++) {
				pt[i][j] = pt[i-1][j-1] + pt[i-1][j];
			}
			pt[i][i] = 1;//rightmost for row i
		}
		return pt; 
	}

	public static void printPascalTriangle (int[][] pt) {
		System.out.println(pt[0][0]); //row 0
		for (int i = 1; i < pt.length; i++) {
			System.out.print(pt[i][0]); // leftmost for row i
			for (int j = 1; j < i; j++) {
				System.out.print(" " + pt[i][j]);
			}
			System.out.println(" " + pt[i][i]); // rightmost for row i
		}
	}
}