import java.io.*;
import java.util.Scanner;

class Chessboard{

     private static DoublyLinkedList list;

     public static void main(String[] args) throws IOException{
          if(args.length < 2){
               System.out.println("Usage: java -jar Chessboard.jar <input file> <output file>");
               System.exit(1);
          }
          Scanner inputFile = new Scanner(new File(args[0]));
          PrintWriter outputFile = new PrintWriter(new FileWriter(args[1]));

          while(inputFile.hasNextLine()){
               list = new DoublyLinkedList();
               boolean valid;

               String input = inputFile.nextLine().trim() + " ";
               String[] token = input.split(":\\s+");
               String leftInput = token[0].trim() + " ";
               String[] leftToken = leftInput.split("\\s+");
               String rightInput = token[1].trim() + " ";
               String[] rightToken = rightInput.split("\\s");
               // for(int i=0; i<rightToken.length;i++){
               //      System.out.println(i+": "+rightToken[i]);
               // }
               int queryCol = Integer.parseInt(leftToken[0]);
               int queryRow = Integer.parseInt(leftToken[1]);
               for(int i=0; i<rightToken.length; i=i+3){
                    if(rightToken[i].equals("q")){
                         list.addRight(new Queen(Integer.parseInt(rightToken[i+1]),Integer.parseInt(rightToken[i+2]),0));
                    }
                    else if(rightToken[i].equals("Q")){
                         list.addRight(new Queen(Integer.parseInt(rightToken[i+1]),Integer.parseInt(rightToken[i+2]),1));
                    }
                    else if(rightToken[i].equals("k")){
                         list.addRight(new King(Integer.parseInt(rightToken[i+1]),Integer.parseInt(rightToken[i+2]),0));
                    }
                    else if(rightToken[i].equals("K")){
                         list.addRight(new King(Integer.parseInt(rightToken[i+1]),Integer.parseInt(rightToken[i+2]),1));
                    }
                    else if(rightToken[i].equals("r")){
                         list.addRight(new Rook(Integer.parseInt(rightToken[i+1]),Integer.parseInt(rightToken[i+2]),0));
                    }
                    else if(rightToken[i].equals("R")){
                         list.addRight(new Rook(Integer.parseInt(rightToken[i+1]),Integer.parseInt(rightToken[i+2]),1));
                    }
                    else if(rightToken[i].equals("n")){
                         list.addRight(new Knight(Integer.parseInt(rightToken[i+1]),Integer.parseInt(rightToken[i+2]),0));
                    }
                    else if(rightToken[i].equals("N")){
                         list.addRight(new Knight(Integer.parseInt(rightToken[i+1]),Integer.parseInt(rightToken[i+2]),1));
                    }
                    else if(rightToken[i].equals("b")){
                         list.addRight(new Bishop(Integer.parseInt(rightToken[i+1]),Integer.parseInt(rightToken[i+2]),0));
                    }
                    else if(rightToken[i].equals("B")){
                         list.addRight(new Bishop(Integer.parseInt(rightToken[i+1]),Integer.parseInt(rightToken[i+2]),1));
                    }
                    else if(rightToken[i].equals("p")){
                         list.addRight(new Pawn(Integer.parseInt(rightToken[i+1]),Integer.parseInt(rightToken[i+2]),0));
                    }
                    else if(rightToken[i].equals("P")){
                         list.addRight(new Pawn(Integer.parseInt(rightToken[i+1]),Integer.parseInt(rightToken[i+2]),1));
                    }
               }

               list.printList();
               System.out.print("\n");
               if(list.checkValid()==false){
                    outputFile.print("Invalid");
               }
               else if(list.find(queryCol,queryRow)==false){
                    outputFile.print("-");
               }
               else{
                    for(int i=0; i<rightToken.length; i=i+3){
                         if(queryCol==Integer.parseInt(rightToken[i+1]) && queryRow==Integer.parseInt(rightToken[i+2])){
                              outputFile.print(rightToken[i]+" ");
                              if(list.doesQueryAttackAny(queryCol,queryRow)==true){
                                   outputFile.print("y");
                              }
                              else{
                                   outputFile.print("n");
                              }
                              break;
                         }
                    }
               }
               outputFile.print("\n");
          }
          inputFile.close();
          outputFile.close();
     }
     private static void test1(){
          list = new DoublyLinkedList();
          list.addRight(new Rook(1,1,0));
          list.addRight(new Rook(2,2,1));
          list.addRight(new Knight(4,4,0));
          list.addLeft(new King(5,6,1));
          list.remove(2,2);
          list.addRight(new Pawn(4,4,1));
          if(list.checkValid()==true){
               System.out.println("hurrah");
          }
          else{
               System.out.println("denied hoe");
          }

          if(list.find(2,2)==true){
               System.out.println("hurrah");
          }
          else{
               System.out.println("denied hoe");
          }

          // if(list.solve()==true){
          //      System.out.println("hurrah");
          // }
          // else{
          //      System.out.println("denied hoe");
          // }
          list.printList();
     }
}
