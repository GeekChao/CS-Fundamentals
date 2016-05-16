/* OpenCommercial.java */

import java.net.*;
import java.io.*;

/**  A class that provides a main function to read five lines of a commercial
 *   Web page and print them in reverse order, given the name of a company.
 */

class OpenCommercial {

  /** Prompts the user for the name X of a company (a single string), opens
   *  the Web site corresponding to www.X.com, and prints the first five lines
   *  of the Web page in reverse order.
   *  @param arg is not used.
   *  @exception Exception thrown if there are any problems parsing the 
   *             user's input or opening the connection.
   */

  private String address;
  private String[] text;

  public OpenCommercial(String company){
      address = "http://www.".concat(company).concat(".com");
      System.out.println(address);
  }

  public void getTextFromWeb(int number) throws IOException{
    URL url = new URL(address);
    BufferedReader br = new BufferedReader(new InputStreamReader(url.openStream()));
    text = new String[number];
    for(int index=0; index<number; index++)
        text[index] = br.readLine();
  }

  public void printTextFromWeb(){
    for(int index=text.length-1; index>=0; index--)
      System.out.println(text[index]);
  }

  public static void main(String[] arg) throws Exception {

    BufferedReader keyboard;
    String inputLine;

    keyboard = new BufferedReader(new InputStreamReader(System.in));

    System.out.print("Please enter the name of a company (without spaces): ");
    System.out.flush();        /* Make sure the line is printed immediately. */
    inputLine = keyboard.readLine();

    /* Replace this comment with your solution.  */
    OpenCommercial oc = new OpenCommercial(inputLine);
    oc.getTextFromWeb(5);
    oc.printTextFromWeb();
  }
}