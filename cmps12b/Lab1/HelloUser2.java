//-----------------------------------------------------------
// Name: Johnson Le
// cruzID: jonle
// id number: 1512346
// File Name: HelloUser2.java
// Description: prints Hello <user>
// can't have 2 HelloUser class, need to rename to HelloUser2
//-----------------------------------------------------------

class HelloUser2{
   public static void main( String[] args ){
      String userName = System.getProperty("user.name");

      System.out.println("Hello "+userName);
   }
}
