import java.io.*;

class DoublyLinkedList{
     ChessPiece head;
     ChessPiece tail;
     DoublyLinkedList(){
          head = null;
          tail = null;
     }
     void addRight(ChessPiece p){
          ChessPiece temp = p;
          if(head == null){
               head = temp;
               tail = head;
          }
          else{
               head.right = temp;
               temp.left = head;
               head = temp;
          }
     }
     void addLeft(ChessPiece p){
          ChessPiece temp = p;
          if(head == null){
               head = temp;
               tail = head;
          }
          else{
               temp.right = tail;
               tail.left = temp;
               tail = temp;
          }
     }
     boolean find(int c, int r){
          ChessPiece temp = head;
          while(temp.left != null){
               if(temp.col == c && temp.row == r){
                    return true;
               }
               temp = temp.left;
          }
          //hasn't checked tail yet
          if(temp.col == c && temp.row == r){
               return true;
          }
          return false;
     }
     ChessPiece grab(int c, int r){
          ChessPiece temp = head;
          while(temp.left != null){
               if(temp.col == c && temp.row == r){
                    return temp;
               }
               temp = temp.left;
          }
          if(temp.col == c && temp.row == r){
               return temp;
          }
          return null;
     }
     boolean findKing(int color){
          ChessPiece temp = head;
          while(temp.left != null){
               if(temp.amIKing()==true){
                    if(temp.color==color){
                         return true;
                    }
               }
               temp=temp.left;
          }
          if(temp.amIKing()==true){
               if(temp.color==color){
                    return true;
               }
          }
          return false;
     }
     ChessPiece grabKing(int color){
          ChessPiece temp = head;
          while(temp.left != null){
               if(temp.amIKing()==true){
                    if(temp.color==color){
                         return temp;
                    }
               }
               temp=temp.left;
          }
          if(temp.amIKing()==true){
               if(temp.color==color){
                    return temp;
               }
          }
          return null;
     }
     ChessPiece traverse(int inc){
          //grabs the 'inc' placed Chesspiece.
          //if inc is 1, will grab the tail. if inc is listSize, grab head.
          ChessPiece temp = head;
          for(int i=inc ; i>1 ; i--){
               temp=temp.left;
          }
          return temp;
     }
     ChessPiece remove(int c, int r){
          ChessPiece sideL = null;
          ChessPiece curr = head;
          ChessPiece sideR = null;
          if(head==null){
               System.out.println("list is empty");
               return null;
          }
          if(curr.col == c && curr.row == r){
               head = head.left;
               head.right = null;
               return curr;
          }
          if(tail.col == c && tail.row == r){
               tail = tail.right;
               tail.left = null;
               return curr;
          }
          while(curr.left!=null){
               sideR = curr;
               curr = curr.left;
               sideL = curr.left;
               if(curr.col == c && curr.row == r){
                    sideR.left = curr.left;
                    sideL.right = curr.right;
                    return curr;
               }
          }
          return null;
     }
     void move(ChessPiece in, int c, int r){
          in.col=c;
          in.row=r;
     }
     int getSize(){
          int size=1;
          ChessPiece temp = head;
          while(temp.left != null){
               size++;
               temp = temp.left;
          }
          return size;
     }
     void printList(){
          ChessPiece curr = tail;
          if(head == null){
               System.out.println("list is empty");
               return;
          }
          while(curr.right != null){
               curr.whatAmI();
               System.out.print("["+curr.col+","+curr.row+"] | ");
               curr = curr.right;
          }
          curr.whatAmI();
          System.out.print("["+curr.col+","+curr.row+"]\n");
     }
}
