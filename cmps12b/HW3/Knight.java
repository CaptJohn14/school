import java.io.*;

public class Knight extends ChessPiece{
     public Knight(int c, int r, int hue){     //constructorp
          super(c,r,hue);
     }
     boolean isAttacking(ChessPiece in, DoublyLinkedList list){
          // System.out.println("checkme");
          if(color == in.color){
               return false;
          }
          // if(col==in.col && row==in.row){
          //      return true;
          // }
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
     boolean isValidMove(int moveC, int moveR, DoublyLinkedList list){
          int validCol[] = { 1, 2, 2, 1,-1,-2,-1,-2};
          int validRow[] = { 2, 1,-1,-2, 2, 1,-2,-1};
          if(moveC==this.col && moveR==this.row){
               System.out.print("can't move to same spot duh\n");
               return false;
          }
          //again, don't need check for block. handle later
          for(int i=0 ; i<8 ; i++){
               if(moveC==this.col+validCol[i] && moveR==this.row+validRow[i]){
                    return true;
               }
          }
          return false;
     }
     void whatAmI(){
          if(this.color==0){
               System.out.print("(W)Knight");
          }
          else{
               System.out.print("(B)Knight");
          }
     }
     char whatCharAmI(){
          if(this.color==0){
               return 'n';
          }
          else{
               return 'N';
          }
     }
     boolean amIKing(){
          return super.amIKing();
     }
}
