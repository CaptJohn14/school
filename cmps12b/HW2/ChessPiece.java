import java.io.*;

public class ChessPiece{
     int col;
     int row;
     int color;
     ChessPiece left;
     ChessPiece right;
     public ChessPiece(int c, int r, int hue){
          col = c;
          row = r;
          color = hue;
          left = null;
          right = null;
     }
     boolean isAttacking(ChessPiece p){
          System.out.println("D:");
          return false;
     }
     void printtest(){
          System.out.println("Super");
     }
}
