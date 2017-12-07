
public class Queen{
     int col;
     int row;
     Queen left;
     Queen right;
     public Queen(int c, int r){
          col = c;
          row = r;
          left = null;
          right = null;
     }
     boolean isAttacking(Queen in){
          if(col==in.col || row==in.row || Math.abs(col-in.col) == Math.abs(row-in.row)){
               return true;
          }
          else{
               return false;
          }
     }
}
