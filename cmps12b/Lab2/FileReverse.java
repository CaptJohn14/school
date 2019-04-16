//----------------------------------------------------------------------
// Name: Johnson Le
// cruzID: jonle
// id number: 1512346
// File Name: FileReverse.java
// Description: takes tokens and reverses it. ie abc-> cba.
//----------------------------------------------------------------------

import java.io.*;
import java.util.Scanner;

class FileReverse{
     public static void main(String[] args) throws IOException{
          int lineNumber = 0;
          // check number of command line argument is at least 2
          if(args.length <2){
               System.out.println("Usage: java -jar FileReverse.jar <input file> <output file>");
               System.exit(1);
          }
          // open file
          Scanner in = new Scanner(new File(args[0]));
          PrintWriter out = new PrintWriter(new FileWriter(args[1]));

          while(in.hasNextLine()){
               lineNumber++;

               String line = in.nextLine().trim() + " "; //think white space? and adds 1? idk lol.
               String[] token = line.split("\\s+");    //splits based on whitespace. the \\s
                                                       //  \\s means 1 white. \\s+ means more

               int n = token.length; //length of token array
               for(int i=0; i<n; i++){
                    String reverse = stringReverse(token[i]);
                    token[i]= reverse;
                    out.println("  "+token[i]);
               }
          }
          in.close();
          out.close();
     }
     public static String stringReverse(String s){
          String backwards = "";
          for (int i=s.length()-1 ; i>=0 ; i--){
               backwards += s.charAt(i);
          }
          return backwards;
     }
}
