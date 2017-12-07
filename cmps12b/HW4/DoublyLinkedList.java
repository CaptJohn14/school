import java.io.*;

class DoublyLinkedList{
     Queen head;
     Queen tail;
     DoublyLinkedList(){
          head = null;
          tail = null;
     }
     void addRight(Queen p){
          Queen temp = p;
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
     void addLeft(Queen p){
          Queen temp = p;
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
          Queen temp = head;
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
     Queen grab(int c, int r){
          Queen temp = head;
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
     int myRow(int c){
          //because there should only ever be 1 queen in a col, this should
          //be valid? haha. 
          Queen temp = head;
          while(temp.left != null){
               if(temp.col == c){
                    return temp.row;
               }
               temp = temp.left;
          }
          if(temp.col == c){
               return temp.row;
          }
          return -1;
     }
     Queen traverse(int inc){
          //grabs the 'inc' placed Queen.
          //if inc is 1, will grab the tail. if inc is listSize, grab head.
          Queen temp = head;
          for(int i=inc ; i>1 ; i--){
               temp=temp.left;
          }
          return temp;
     }
     Queen remove(int c, int r){
          Queen sideL = null;
          Queen curr = head;
          Queen sideR = null;
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
     void move(Queen in, int c, int r){
          in.col=c;
          in.row=r;
     }
     int getSize(){
          int size=1;
          Queen temp = head;
          while(temp.left != null){
               size++;
               temp = temp.left;
          }
          return size;
     }
     void printList(){
          Queen curr = tail;
          if(head == null){
               System.out.println("list is empty");
               return;
          }
          while(curr.right != null){
               // curr.whatAmI();
               System.out.print("["+curr.col+","+curr.row+"]");
               curr = curr.right;
          }
          // curr.whatAmI();
          System.out.print("["+curr.col+","+curr.row+"]\n");
     }
}
