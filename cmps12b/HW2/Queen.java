import java.io.*;

public class Queen extends ChessPiece{
     public Queen(int c, int r, int hue){     //constructorp
          super(c,r,hue);
     }
     boolean isAttacking(ChessPiece inputPiece){
          //check if same color
          if(color == inputPiece.color){
               return false;
          }
          //check if same space
          if(col==inputPiece.col && row==inputPiece.row){
               return true;
          }
          //check if row or col attack
          if(col==inputPiece.col || row==inputPiece.row){
               return true;
          }
          else if(Math.abs(row-inputPiece.row) == Math.abs(col-inputPiece.col)){
               return true;
          }
          else{
               return false;
          }
     }
}
