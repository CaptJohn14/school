import java.io.*;

public class King extends ChessPiece{
     public King(int c, int r, int hue){     //constructorp
          super(c,r,hue);
     }
     boolean isAttacking(ChessPiece in, DoublyLinkedList list){
          if(color == in.color){
               return false;
          }
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
     boolean isValidMove(int moveC, int moveR, DoublyLinkedList list){
          int validCol[] = {0, 0, 1,-1,-1, 1,-1, 1};
          int validRow[] = {1,-1, 0, 0, 1, 1,-1,-1};
          if(moveC==this.col && moveR==this.row){
               System.out.print("can't move to same spot duh\n");
               return false;
          }
          //don't need to check if blocking, will handle in main class
          for(int i=0 ; i<8 ; i++){
               if(moveC==this.col+validCol[i] && moveR==this.row+validRow[i]){
                    //if the spot moved is equal to 1 of the legal cases, return true;
                    return true;
               }
               //will loop to keep finding, if can't find legal move, than exit
               //for loop and return false;
          }

          return false;
     }
     void whatAmI(){
          if(color==0){
               System.out.print("(W)King");
          }
          else{
               System.out.print("(B)King");
          }
     }
     char whatCharAmI(){
          if(this.color==0){
               return 'k';
          }
          else{
               return 'K';
          }
     }
     boolean amIKing(){
          return true;
     }
}
