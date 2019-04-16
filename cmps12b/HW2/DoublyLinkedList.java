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
          return null;
     }
     boolean doesQueryAttackAny(int c, int r){
          ChessPiece temp = head;
          ChessPiece move = temp;

          while(temp != null){
               if(temp.col == c && temp.row == r){
                    while(move != null){
                         if(temp != move){
                              if(temp.isAttacking(move)==true){
                                   // System.out.println("please");
                                   return true;
                              }
                         }
                         move=move.left;
                    }
               }
               temp=temp.left;
          }
          return false;
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
     boolean checkValid(){
          ChessPiece temp = head;
          ChessPiece move = temp;
          if(head==null){
               System.out.println("list is empty");
               return true;
          }
          while(temp.left != null){
               while(move.left != null){
                    move = move.left;
                    if(temp.col==move.col && temp.row==move.row){
                         return false;
                    }
               }
               temp = temp.left;
               move = temp;
          }
          return true;
     }
     // boolean checkQuery(int c, int r){
     //      //don't really need this. can just find o.o
     //      ChessPiece temp = head;
     //      if(temp.col == c && temp.row == r){
     //           return true;
     //      }
     //      while(temp.left != null){
     //           temp = temp.left;
     //           if(temp.col == c && temp.row == r){
     //                return true;
     //           }
     //      }
     //      return false;
     // }
     boolean doesAnyAttack(){
          //thought the goal was to see if any pieces attack each other... woops
          ChessPiece temp = head;
          ChessPiece move = temp;
          if(head==null){
               System.out.println("list is empty so default win?");
               return true;
          }
          while(temp.left != null){
               while(move.left != null){
                    move = move.left;
                    if(temp.isAttacking(move)==true || move.isAttacking(temp)==true){
                         return false;
                    }
               }
               temp = temp.left;
               move = temp;
          }
          return true;
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
               System.out.print("["+curr.col+","+curr.row+"]");
               curr = curr.right;
          }
          System.out.print("["+curr.col+","+curr.row+"]");
     }
}
