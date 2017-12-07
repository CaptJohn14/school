//----------------------------------------------------------------------
// Name: Johnson Le
// cruzID: jonle
// id number: 1512346
// File Name: FileCopy.java
// Description: takes a file and copies it creating another
//----------------------------------------------------------------------

import java.io.*;
import java.util.Scanner;

class FileCopy{

   public static void main(String[] args) throws IOException{

      // check number of command line arguments is at least 2
      if(args.length < 2){
         System.out.println("Usage: java –jar FileCopy.jar <input file> <output file>");
         //ALWAYS DO THIS
         System.exit(1);
      }

      // open files. scanner in takes in a file. printwriter outputs.
      Scanner in = new Scanner(new File(args[0]));
      PrintWriter out = new PrintWriter(new FileWriter(args[1]));

      // read lines from in, write lines to out
      while( in.hasNextLine() ){
         String line = in.nextLine();
         out.println( line );
      }

      // close files
      in.close();
      out.close();
   }
}
