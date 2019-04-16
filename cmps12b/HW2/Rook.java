import java.io.*;

public class Rook extends ChessPiece{
     public Rook(int c, int r, int hue){     //constructor
          super(c,r,hue);
          // System.out.println("in here please");
     }
     boolean isAttacking(ChessPiece in){
          // System.out.println(":D");
          if(color == in.color){
               return false;
          }
          if(col==in.col && row==in.row){
               return true;
          }
          if(col==in.col || row==in.row){
               return true;
          }
          return false;
     }
     void printtest(){
          // super.printtest();
          System.out.println("rook");
     }
}
