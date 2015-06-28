import java.io.*;
import java.lang.*;

class Nuke2{

	public static String removeSecondCharFromStr(String str){
		return str.substring(0,1) + str.substring(2,str.length());
	}

	public static void main(String[] args)throws IOException{
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		System.out.println(removeSecondCharFromStr(br.readLine()));
	}
}