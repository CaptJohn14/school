import java.io.*;
import java.util.Scanner;
/*
     -valid if alternates move
     -valid if a piece is there to move
     -valid if piece can move accordingly
     -not valid if piece moves onto same color
     -not valid if move path is blocked. knights void.
     -not valid if King is checked after moving.
*/

class ChessMoves{
     private static ChessPiece kingW, kingB;
     private static DoublyLinkedList list;

     public static void main(String[] args) throws IOException{
          if(args.length < 2){
               System.out.println("Usage: java -jar ChessMovs.jar <input file> <output file>");
               System.exit(1);
          }
          // test1();
          Scanner inputFile = new Scanner(new File(args[0]));
          PrintWriter outputFile = new PrintWriter(new FileWriter(args[1]));

          while(inputFile.hasNextLine()){
               System.out.print("_____________________________\n");
               list = new DoublyLinkedList();
               String input = inputFile.nextLine().trim() + " ";
               String[] token = input.split(":\\s+");
               String leftInput = token[0].trim() + " ";
               String[] leftToken = leftInput.split("\\s+");
               String rightInput = token[1].trim() + " ";
               String[] rightToken = rightInput.split("\\s");

               placePieces(leftToken);
               list.printList();

               int which = 1; //start with black but not really. first
               //iteration of for loop makes it white first.
               boolean legal = true;

               for(int i = 0 ; i<rightToken.length ; i=i+4){
                    if(which==0){
                         which=1;
                    }
                    else{
                         which=0;
                    }
                    int currC = Integer.parseInt(rightToken[i]);
                    int currR = Integer.parseInt(rightToken[i+1]);
                    int moveC = Integer.parseInt(rightToken[i+2]);
                    int moveR = Integer.parseInt(rightToken[i+3]);
                    if(list.find(currC,currR)==false){
                         legal = false;
                         outputFile.print(currC+" "+currR+" "+moveC+" "+moveR+" illegal\n");
                         break;
                    }
                    ChessPiece myPiece = list.grab(currC,currR);
                    System.out.print("["+currC+","+currR+"] --> ["+moveC+","+moveR+"] | \n");
                    //first check correct player's turn
                    if(myPiece.color!=which){
                         legal = false;
                         System.out.print("uh\n");
                         outputFile.print(currC+" "+currR+" "+moveC+" "+moveR+" illegal\n");
                         break;
                    }
                    if(canMove(moveC,moveR,myPiece)==true){
                         list.move(myPiece,moveC,moveR);
                         list.printList();
                         if(amISafe(myPiece.color)==false){
                              System.out.print("in check\n");
                              legal=false;
                              outputFile.print(currC+" "+currR+" "+moveC+" "+moveR+" illegal\n");
                              break;
                         }
                    }
                    else{
                         legal = false;
                         outputFile.print(currC+" "+currR+" "+moveC+" "+moveR+" illegal\n");
                         break;
                    }
               }
               // System.out.print("hello?");
               if(legal == true){
                    outputFile.print("legal\n");
               }
               System.out.print("\n");
          }
          inputFile.close();
          outputFile.close();
     }
     private static void placePieces(String[] tok){
          for(int i=0 ; i<tok.length ; i=i+3){
               if(tok[i].equals("q")){
                    list.addRight(new Queen(Integer.parseInt(tok[i+1]),Integer.parseInt(tok[i+2]),0));
               }
               else if(tok[i].equals("Q")){
                    list.addRight(new Queen(Integer.parseInt(tok[i+1]),Integer.parseInt(tok[i+2]),1));
               }
               else if(tok[i].equals("k")){
                    list.addRight(new King(Integer.parseInt(tok[i+1]),Integer.parseInt(tok[i+2]),0));
                    kingW=list.grab(Integer.parseInt(tok[i+1]),Integer.parseInt(tok[i+2]));
               }
               else if(tok[i].equals("K")){
                    list.addRight(new King(Integer.parseInt(tok[i+1]),Integer.parseInt(tok[i+2]),1));
                    kingB=list.grab(Integer.parseInt(tok[i+1]),Integer.parseInt(tok[i+2]));
               }
               else if(tok[i].equals("r")){
                    list.addRight(new Rook(Integer.parseInt(tok[i+1]),Integer.parseInt(tok[i+2]),0));
               }
               else if(tok[i].equals("R")){
                    list.addRight(new Rook(Integer.parseInt(tok[i+1]),Integer.parseInt(tok[i+2]),1));
               }
               else if(tok[i].equals("n")){
                    list.addRight(new Knight(Integer.parseInt(tok[i+1]),Integer.parseInt(tok[i+2]),0));
               }
               else if(tok[i].equals("N")){
                    list.addRight(new Knight(Integer.parseInt(tok[i+1]),Integer.parseInt(tok[i+2]),1));
               }
               else if(tok[i].equals("b")){
                    list.addRight(new Bishop(Integer.parseInt(tok[i+1]),Integer.parseInt(tok[i+2]),0));
               }
               else if(tok[i].equals("B")){
                    list.addRight(new Bishop(Integer.parseInt(tok[i+1]),Integer.parseInt(tok[i+2]),1));
               }
               else if(tok[i].equals("p")){
                    list.addRight(new Pawn(Integer.parseInt(tok[i+1]),Integer.parseInt(tok[i+2]),0));
               }
               else if(tok[i].equals("P")){
                    list.addRight(new Pawn(Integer.parseInt(tok[i+1]),Integer.parseInt(tok[i+2]),1));
               }
          }
     }
     private static void test1(){
          list = new DoublyLinkedList();
          list.addRight(new Pawn(1,8,1));
          list.addRight(new Pawn(1,7,1));
          list.addRight(new Bishop(1,1,0));
          list.addRight(new Pawn(1,6,1));
          list.addRight(new Pawn(5,5,1));
          list.printList();
          ChessPiece tester = list.traverse(3);
          int moveC=5;
          int moveR=5;
          if(canMove(moveC,moveR,tester)==true){
               list.move(tester,moveC,moveR);
               list.printList();
          }
          System.exit(1);
     }
     private static void test2(){
          list = new DoublyLinkedList();
          list.addRight(new King(3,3,0));
          list.addRight(new Knight(2,2,1));
          list.printList();
          ChessPiece tester = list.traverse(2);
          int moveC=2;
          int moveR=2;
          if(canMove(moveC,moveR,tester)==true){
               list.move(tester,moveC,moveR);
               list.printList();
          }
          System.exit(1);
     }
     private static boolean canMove(int c, int r, ChessPiece toMove){
          if(toMove.isValidMove(c,r,list)==false){
               toMove.whatAmI();
               System.out.print("["+toMove.col+","+toMove.row+"] can't move to ["+c+","+r+"]\n");
               return false;
          }
          for(int i=list.getSize() ; i>=1 ; i--){
               ChessPiece temp = list.traverse(i);
               if(temp==toMove){
                    continue;
               }
               else{ //possible valid move, but check if something alrdy there
                    if(c==temp.col && r==temp.row && temp.color!=toMove.color){
                         //if same spot & opposite color, capture
                         toMove.whatAmI();
                         System.out.print("["+toMove.col+","+toMove.row+"] captures ");
                         temp.whatAmI();
                         System.out.print("["+temp.col+","+temp.row+"]\n");
                         list.remove(c,r);
                         return true;
                    }
                    else if(c==temp.col && r==temp.row && temp.color==toMove.color){
                         //same spot but same color, can't move there
                         toMove.whatAmI();
                         System.out.print("["+toMove.col+","+toMove.row+"] can't move to ["+c+","+r+"]\n");
                         return false;
                    }
                    //if not the same spot, move on and check the others
                    continue;
               }
          }
          //after exhausting all pieces to not be in the desired move location,
          //then must be true. already checked that nothing is in the way
          toMove.whatAmI();
          System.out.print("["+toMove.col+","+toMove.row+"] moves to ["+c+","+r+"] \n");
          return true;
     }
     private static boolean amISafe(int color){
          ChessPiece theKing = list.grabKing(color);
          //theKing is the attackie
          for(int i=list.getSize() ; i>-1 ; i--){
               ChessPiece attacker = list.traverse(i);
               if(attacker==theKing){
                    continue;
               }
               else if(attacker.isAttacking(theKing, list)==true){
                    //basically will check if any piece is attacking king
                    //if any does, that it is in check.
                    return false;
               }
          }
          return true;
     }
}
//can you capture a king?
//somehow check if AFTER moving, if king is in check.
//add validMoves to all pieces
//got bishop isValidMove working super well.
//maybe redo isValidMove for rook using Bishop as reference to make queen easier.
//created a new isValidMove for rook in temp.java but haven't tested. GOOD NIGHT self
//implemnted all isValidMove. now only need to see if in check
//need to update isAttacking to see if something is in the way.
//update isAttacking for rook and bishop. then copy to queen

//fuckkkk. might need dto also change isValidMove to go thru
//list to make sure nothing blocking. right now, its going 1by1
