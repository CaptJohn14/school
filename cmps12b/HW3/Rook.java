import java.io.*;

public class Rook extends ChessPiece{
     public Rook(int c, int r, int hue){     //constructor
          super(c,r,hue);
          // System.out.println("in here please");
     }
     boolean isAttacking(ChessPiece attackie, DoublyLinkedList list){
          if(this.color == attackie.color){
               return false;
          }
          if(this.col==attackie.col || this.row==attackie.row){
               //so if rook 'COULD' phase through other pieces, would be true
               //if something is blocking, than can't attack
               //if NO blocker, then false
               if(isAnyBlocker(attackie,list)==false){
                    return true;
               }
               // return true;
          }
          return false;
     }
     boolean isAnyBlocker(ChessPiece attackie, DoublyLinkedList list){
          //will be called only AFTER isAttacking returns true
          //if false, need to continue to check ALL cases. true if block
          //at the end will return false after going thru all cases
          //and nothing is blocking.
          //in the end should only have 1 return false.

          int attackieDir = checkDir(attackie.col, attackie.row);
          for(int i=list.getSize() ; i>=1 ; i--){
               //temp = potential blocker
               ChessPiece temp = list.traverse(i);
               if(temp==attackie || temp==this){
                    continue;
               }
               int tempDir = checkDir(temp.col, temp.row);
               if(attackieDir != tempDir){
                    //not in the same direction so false for not block
                    continue;
               }
               //now that we know it is in the same direction.. need to separate
               //into 4 cases for 4 directions
               if(tempDir==1 && temp.col==this.col && temp.row<attackie.row){
                    return true;
               }
               else if(tempDir==2 && temp.row==this.row && temp.col<attackie.col){
                    return true;
               }
               else if(tempDir==3 && temp.col==this.col && temp.row>attackie.row){
                    return true;
               }
               else if(tempDir==4 && temp.row==this.row && temp.col>attackie.col){
                    return true;
               }
               else{
                    //after all these cases, that means was not blocking so
                    //false for no block. continue;
                    continue;
               }
          }
          // list.printList();
          return false;
     }
     boolean isValidMove(int moveC, int moveR, DoublyLinkedList list){
          //this doesn't take into account that a piece may be at the spot
          if(moveC==this.col && moveR==this.row){
               System.out.print("can't move to same spot duh\n");
               return false;
          }
          //need this one to be all true->continue so can keep
          //checking the other pieces
          if(moveC==this.col || moveR==this.row){
               int moveDir = checkDir(moveC,moveR);
               for(int i=list.getSize() ; i>=1 ; i--){
                    ChessPiece temp = list.traverse(i);
                    if(temp==this){
                         continue;
                    }
                    int tempDir = checkDir(temp.col,temp.row);
                    if(tempDir != moveDir){
                         //not in same direction so can't block ->true
                         continue;
                    }
                    //now that potential blocker is in the direction where
                    //want to go. if it is inbetween, than is block so return false
                    if(tempDir==1 && temp.col==this.col && temp.row<moveR){
                         return false;
                    }
                    else if(tempDir==2 && temp.row==this.row && temp.col<moveC){
                         return false;
                    }
                    else if(tempDir==3 && temp.col==this.col && temp.row>moveR){
                         return false;
                    }
                    else if(tempDir==4 && temp.row==this.row && temp.col>moveC){
                         return false;
                    }
                    else{
                         continue;
                    }
               }
               //so after exhausting all pieces in list as
               //non potetential blockers. path is truly free
               //will deal with if a piece is on move location later
               return true;
          }
          //if where want to move is not even possible, false
          return false;
     }
     int checkDir(int c, int r){
          //1=up, 2=right, 3=down, 4=left
          if(c==this.col && r>this.row){
               return 1;
          }
          else if(c>this.col && r==this.row){
               return 2;
          }
          else if(c==this.col && r<this.row){
               return 3;
          }
          else if(c<this.col && r==this.row){     //if(c<this.col && r==this.row)
               return 4;
          }
          else{
               return 0;
          }
     }
     void whatAmI(){
          if(this.color==0){
               System.out.print("(W)Rook");
          }
          else{
               System.out.print("(B)Rook");
          }
     }
     char whatCharAmI(){
          if(this.color==0){
               return 'r';
          }
          else{
               return 'R';
          }
     }
     boolean amIKing(){
          return super.amIKing();
     }
     void printtest(){
          // super.printtest();
          System.out.println("rook");
     }
}
