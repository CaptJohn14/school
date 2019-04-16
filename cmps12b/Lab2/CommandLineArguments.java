//----------------------------------------------------------------------
// Name: Johnson Le
// cruzID: jonle
// id number: 1512346
// File Name: CommandLineArguments.java
// Description: returns the amount of arguments given
//----------------------------------------------------------------------
class CommandLineArguments{
   public static void main(String[] args){
      int n = args.length;
      System.out.println("args.length = " + n);
      for(int i=0; i<n; i++) System.out.println(args[i]);
   }
}
