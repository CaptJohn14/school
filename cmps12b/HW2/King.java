import java.io.*;

public class King extends ChessPiece{
     public King(int c, int r, int hue){     //constructorp
          super(c,r,hue);
     }
     boolean isAttacking(ChessPiece in){
          if(color == in.color){
               return false;
          }
          if(col==in.col && row==in.row){
               return true;
          }
          //diagonals
          if(col+1==in.col && row+1==in.row){          //RU
               return true;
          }
          else if(col+1==in.col && row-1==in.row){     //RD
               return true;
          }
          else if(col-1==in.col && row+1==in.row){     //LU
               return true;
          }
          else if(col-1==in.col && row-1==in.row){     //LD
               return true;
          }
          //sides
          else if(col+1==in.col && row==in.row){        //R
               return true;
          }
          else if(col-1==in.col && row==in.row){        //L
               return true;
          }
          else if(col==in.col && row+1==in.row){        //u
               return true;
          }
          else if(col==in.col && row-1==in.row){        //D
               return true;
          }
          else{
               return false;
          }
     }
}
