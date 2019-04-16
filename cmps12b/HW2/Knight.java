import java.io.*;

public class Knight extends ChessPiece{
     public Knight(int c, int r, int hue){     //constructorp
          super(c,r,hue);
     }
     boolean isAttacking(ChessPiece in){
          // System.out.println("checkme");
          if(color == in.color){
               return false;
          }
          if(col==in.col && row==in.row){
               return true;
          }
          if(col+1 == in.col && row+2 == in.row){
               return true;
          }
          else if(col+2 == in.col && row+1 == in.row){
               return true;
          }
          else if(col+2 == in.col && row-1 == in.row){
               return true;
          }
          else if(col+1 == in.col && row-2 == in.row){
               return true;
          }
          else if(col-1 == in.col && row+2 == in.row){
               return true;
          }
          else if(col-2 == in.col && row+1 == in.row){
               return true;
          }
          else if(col-1 == in.col && row-2 == in.row){
               return true;
          }
          else if(col-2 == in.col && row-1 == in.row){
               return true;
          }
          else{
               return false;
          }
     }
}
