import java.io.*;

public class Bishop extends ChessPiece{
     public Bishop(int c, int r, int hue){     //constructorp
          super(c,r,hue);
     }
     boolean isAttacking(ChessPiece attackie, DoublyLinkedList list){
          if(this.color==attackie.color){
               return false;
          }
          if(Math.abs(this.col-attackie.col) == Math.abs(this.row-attackie.row)){
               if(isAnyBlocker(attackie,list)==false){
                    return true;
               }
               // return true;
          }
          return false;
     }
     boolean isAnyBlocker(ChessPiece attackie, DoublyLinkedList list){
          int attackieDir = checkDir(attackie.col, attackie.row);
          for(int i=list.getSize() ; i>=1 ; i--){
               ChessPiece temp = list.traverse(i);
               if(temp == attackie || temp == this){
                    continue;
               }
               int tempDir = checkDir(temp.col, temp.row);
               if(attackieDir != tempDir){
                    continue;
               }
               if(tempDir==1 && temp.col<attackie.col && temp.row<attackie.row){
                    return true;
               }
               else if(tempDir==2 && temp.col<attackie.col && temp.row>attackie.row){
                    return true;
               }
               else if(tempDir==3 && temp.col>attackie.col && temp.row>attackie.row){
                    return true;
               }
               else if(tempDir==4 && temp.col>attackie.col && temp.row<attackie.row){
                    return true;
               }
               else{
                    continue;
               }
          }
          return false;
     }
     boolean isValidMove(int moveC, int moveR, DoublyLinkedList list){
          // System.out.print("hello\n");
          if(moveC==this.col && moveR==this.row){
               System.out.print("can't move to same spot duh\n");
               return false;
          }
          //keep true->continue to keep check all other pieces
          //first check if spot want is in diagonal.
          if(Math.abs(this.col-moveC)==Math.abs(this.row-moveR)){
               int moveDir = checkDir(moveC,moveR);
               for(int i=list.getSize() ; i>=1 ; i--){
                    ChessPiece temp = list.traverse(i);
                    if(temp==this){
                         continue;
                    }
                    int tempDir = checkDir(temp.col,temp.row);
                    if(tempDir!=moveDir){
                         continue;
                    }
                    //potential blocker same direction as move location
                    if(tempDir==1 && temp.col<moveC && temp.row<moveR){//UR
                         return false;
                    }
                    else if(tempDir==2 && temp.col<moveC && temp.row>moveR){//DR
                         return false;
                    }
                    else if(tempDir==3 && temp.col>moveC && temp.row>moveR){//DL
                         return false;
                    }
                    else if(tempDir==4 && temp.col>moveC && temp.row<moveR){//UL
                         return false;
                    }
                    else{
                         continue;
                    }
               }
               return true;
          }
          //move loc not even in diagonal so return false
          return false;
     }
     int checkDir(int c, int r){
          //think imma check if it is even in diagonal, if not return 0.
          //1=UR, 2=DR, 3= DL, 4=UL
          //only call this after checking if something is even in the diagonals
          if(Math.abs(c-this.col)!=Math.abs(r-this.row)){
               return 0;
          }
          if(c>this.col && r>this.row){
               return 1;
          }
          else if(c>this.col && r<this.row){
               return 2;
          }
          else if(c<this.col && r<this.row){
               return 3;
          }
          else if(c<this.col && r>this.row){
               return 4;
          }
          else{
               return 0;
          }
     }
     void whatAmI(){
          if(this.color==0){
               System.out.print("(W)Bishop");
          }
          else{
               System.out.print("(B)Bishop");
          }
     }
     char whatCharAmI(){
          if(this.color==0){
               return 'b';
          }
          else{
               return 'B';
          }
     }
     boolean amIKing(){
          return super.amIKing();
     }
}
