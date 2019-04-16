import java.io.*;

public class ChessPiece{
     int col;
     int row;
     int color;
     ChessPiece left;
     ChessPiece right;
     public ChessPiece(int c, int r, int hue){
          col = c;
          row = r;
          color = hue;
          left = null;
          right = null;
     }
     boolean isAttacking(ChessPiece attackie, DoublyLinkedList list){
          System.out.print("D:\n");
          return false;
     }
     boolean isValidMove(int moveC, int moveR, DoublyLinkedList list){
          System.out.print("NOOOOO D:\n");
          return false;
     }
     void whatAmI(){
          System.out.print("I am super!\n");
     }
     char whatCharAmI(){
          System.out.print("I am super!\n");
          return 'S';
     }
     boolean amIKing(){
          return false;
     }
     int checkDir(int c, int r){
          return -1;
     }
     void printtest(){
          System.out.print("Super!!\n");
     }
}
