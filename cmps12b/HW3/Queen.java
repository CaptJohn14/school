import java.io.*;

public class Queen extends ChessPiece{
     public Queen(int c, int r, int hue){     //constructorp
          super(c,r,hue);
     }
     boolean isAttacking(ChessPiece attackie, DoublyLinkedList list){
          //should be 100% working now.
          if(this.color == attackie.color){
               return false;
          }
          if(this.col==attackie.col || this.row==attackie.row){
               if(isAnyBlockerRook(attackie,list)==false){
                    return true;
               }
          }
          else if(Math.abs(this.col-attackie.col) == Math.abs(this.row-attackie.row)){
               if(isAnyBlockerBish(attackie,list)==false){
                    return true;
               }
          }
          return false;
     }
     boolean isAnyBlockerRook(ChessPiece attackie, DoublyLinkedList list){
          int attackieDir = checkDir(attackie.col, attackie.row);
          for(int i=list.getSize() ; i>=1 ; i--){
               ChessPiece temp = list.traverse(i);
               if(temp==attackie || temp==this){
                    continue;
               }
               int tempDir = checkDir(temp.col, temp.row);
               if(attackieDir != tempDir){
                    continue;
               }
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
                    continue;
               }
          }
          return false;
     }
     boolean isAnyBlockerBish(ChessPiece attackie, DoublyLinkedList list){
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
               if(tempDir==5 && temp.col<attackie.col && temp.row<attackie.row){
                    return true;
               }
               else if(tempDir==6 && temp.col<attackie.col && temp.row>attackie.row){
                    return true;
               }
               else if(tempDir==7 && temp.col>attackie.col && temp.row>attackie.row){
                    return true;
               }
               else if(tempDir==8 && temp.col>attackie.col && temp.row<attackie.row){
                    return true;
               }
               else{
                    continue;
               }
          }
          return false;
     }
     boolean isValidMove(int moveC, int moveR, DoublyLinkedList list){
          if(moveC==this.col && moveR==this.row){
               System.out.print("can't move to same spot duh\n");
               return false;
          }
          //do like rook first
          if(moveC==this.col || moveR==this.row){
               int moveDir = checkDir(moveC,moveR);
               for(int i=list.getSize() ; i>=1 ; i--){
                    ChessPiece temp = list.traverse(i);
                    if(temp==this){
                         continue;
                    }
                    int tempDir = checkDir(temp.col,temp.row);
                    if(tempDir != moveDir){
                         continue;
                    }
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
               return true;
          }

          //now like bishop
          else if(Math.abs(this.col-moveC)==Math.abs(this.row-moveR)){
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
                    if(tempDir==5 && temp.col<moveC && temp.row<moveR){//UR
                         return false;
                    }
                    else if(tempDir==6 && temp.col<moveC && temp.row>moveR){//DR
                         return false;
                    }
                    else if(tempDir==7 && temp.col>moveC && temp.row>moveR){//DL
                         return false;
                    }
                    else if(tempDir==8 && temp.col>moveC && temp.row<moveR){//UL
                         return false;
                    }
                    else{
                         continue;
                    }
               }
               return true;
          }
          return false;
     }
     int checkDir(int c, int r){
          //1-4-> U,R,D,L
          //5-8-> UR,DR,DL,UL
          if(c==this.col && r>this.row){
               return 1;
          }
          else if(c>this.col && r==this.row){
               return 2;
          }
          else if(c==this.col && r<this.row){
               return 3;
          }
          else if(c<this.col && r==this.row){
               return 4;
          }
          //diagonals
          else if(Math.abs(c-this.col)!=Math.abs(r-this.row)){
               return 0;
          }
          else if(c>this.col && r>this.row){
               return 5;
          }
          else if(c>this.col && r<this.row){
               return 6;
          }
          else if(c<this.col && r<this.row){
               return 7;
          }
          else if(c<this.col && r>this.row){
               return 8;
          }
          else{
               return 0;
          }
     }
     void whatAmI(){
          if(this.color==0){
               System.out.print("(W)Queen");
          }
          else{
               System.out.print("(B)Queen");
          }
     }
     char whatCharAmI(){
          if(this.color==0){
               return 'q';
          }
          else{
               return 'Q';
          }
     }
     boolean amIKing(){
          return super.amIKing();
     }
}
