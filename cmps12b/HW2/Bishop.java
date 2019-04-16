import java.io.*;

public class Bishop extends ChessPiece{
     public Bishop(int c, int r, int hue){     //constructorp
          super(c,r,hue);
     }
     boolean isAttacking(ChessPiece in){
          if(color == in.color){
               return false;
          }
          if(col==in.col && row==in.row){
               return true;
          }
          if(Math.abs(row-in.row) == Math.abs(col-in.col)){
               return true;
          }
          return false;
     }
}
