import java.io.*;

public class Pawn extends ChessPiece{
     public Pawn(int c, int r, int hue){     //constructorp
          super(c,r,hue);
     }
     boolean isAttacking(ChessPiece in){
          if(color == in.color){
               return false;
          }
          if(col==in.col && row==in.row){
               return true;
          }
          //0 for white, 1 for black
          if(color==0){
               if(row+1==in.row){
                    if(col+1==in.col || col-1==in.col){
                         return true;
                    }
               }
          }
          if(color==1){
               if(row-1==in.row){
                    if(col+1==in.col || col-1==in.col){
                         return true;
                    }
               }
          }
          return false;
     }
}
