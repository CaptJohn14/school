import java.io.*;

public class Pawn extends ChessPiece{
     public Pawn(int c, int r, int hue){     //constructorp
          super(c,r,hue);
     }
     boolean isAttacking(ChessPiece in, DoublyLinkedList list){
          if(color == in.color){
               return false;
          }
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
     boolean isValidMove(int moveC, int moveR, DoublyLinkedList list){
          if(moveC==this.col && moveR==this.row){
               System.out.print("can't move to same spot duh\n");
               return false;
          }
          if(moveR==this.row){
               //can't move horizontially no matter what
               return false;
          }
          // ChessPiece temp = isAnyThere(moveC,moveR,list);
          Boolean temp = list.find(moveC,moveR);
          if(this.color==0){//white only move up
               if(moveC==this.col && moveR==this.row+1 && temp==false){
                    //for right above, can't have piece there
                    return true;
               }
               else if(this.row==2 && moveC==this.col && moveR==this.row+2 && temp==false){
                    if(list.find(this.col,this.row+1)==true){
                         return false;
                    }
                    return true;
               }
               else if(moveC==this.col+1 && moveR==this.row+1 && temp==true){
                    //UR, piece must be there
                    //i'll deal with the color later.
                    return true;
               }
               else if(moveC==this.col-1 && moveR==this.row+1 && temp==true){
                    //UL, piece must be there
                    return true;
               }
               else{
                    return false;
               }
          }
          else if(this.color==1){ //black only move down
               // System.out.print("duh");
               if(moveC==this.col && moveR==this.row-1 && temp==false){
                    //D. can't be any piece there
                    return true;
               }
               else if(this.row==7 && moveC==this.col && moveR==this.row-2 && temp==false){
                    if(list.find(this.col,this.row-1)==true){
                         return false;
                    }
                    return true;
               }
               else if(moveC==this.col+1 && moveR==this.row-1 && temp==true){
                    //DR
                    return true;
               }
               else if(moveC==this.col-1 && moveR==this.row-1 && temp==true){
                    //DL
                    return true;
               }
               else{
                    return false;
               }
          }
          return false;//this return won't matter. one of the cases above should be true;
     }
     void whatAmI(){
          if(this.color==0){
               System.out.print("(W)Pawn");
          }
          else{
               System.out.print("(B)Pawn");
          }
     }
     char whatCharAmI(){
          if(this.color==0){
               return 'p';
          }
          else{
               return 'P';
          }
     }
     boolean amIKing(){
          return super.amIKing();
     }
}
