import java.io.*;
import java.util.Scanner;
import java.util.Stack;

//really only using linked list to print. lol.
//had it set up from previous hw already so why not.

class NQueens{
     private static Stack<Integer> stack;
     private static DoublyLinkedList list;
     private static int boardSize;
     private static char[][] board;
     private static int[] initCols;

     public static void main(String[] args) throws IOException{
          // boardSize = 8;
          // test1();
          if(args.length < 2){
               System.out.println("Usage: java -jar NQueens.jar <input file> <output file>");
               System.exit(1);
          }

          Scanner inputFile = new Scanner(new File(args[0]));
          PrintWriter outputFile = new PrintWriter(new FileWriter(args[1]));
          int line = 0;
          System.out.print("=================================\n");
          while(inputFile.hasNextLine()){
               // Stack<Integer> stack = new Stack<Integer>();
               stack = new Stack<Integer>();

               list = new DoublyLinkedList();
               line=line+1;
               String input = inputFile.nextLine().trim() + " ";
               String[] token = input.split("\\s+");
               // int[] initCols = new int[token.length/2];
               initCols = new int[token.length/2];
               boardSize = Integer.parseInt(token[0]);

               int k=0;
               for(int i=1 ; i<token.length ; i=i+2){
                    list.addRight(new Queen(Integer.parseInt(token[i]),Integer.parseInt(token[i+1])));
                    initCols[k]=Integer.parseInt(token[i]);
                    k++;
               }
               stackInit(boardSize, initCols);
               System.out.println("The stack: "+stack);

               System.out.print("Input line "+line+": ");
               list.printList();
               visualize();

               boolean firstQueensValid = true;
               loop:
               for(int i=list.getSize() ; i>=1 ; i--){
                    Queen temp1 = list.traverse(i);
                    for(int j=list.getSize() ; j>=1 ; j--){
                         Queen temp2 = list.traverse(j);
                         if(temp1==temp2){
                              continue;
                         }
                         if(temp1.isAttacking(temp2)){
                              System.out.print("initial queens attack each other\n");
                              firstQueensValid = false;
                              outputFile.print("No solution");
                              break loop;
                         }
                    }
               }
               if(firstQueensValid){
                    if(solve()==false){
                         outputFile.print("No solution");
                    }
                    else if(solve()==true){
                         for(int i=1 ; i<=boardSize ; i++){
                              for(int j=1; j<=boardSize ; j++){
                                   if(board[i][j]=='Q'){
                                        outputFile.print(i+" "+j+" ");
                                   }
                              }
                         }
                    }
               }
               // solve();
               // visualize();
               outputFile.print("\n");
               System.out.print("=================================\n");
          }
          inputFile.close();
          outputFile.close();
     }
     private static boolean solve(){
          int row = 1;
          mainloop:
          while(true){
               if(stack.empty() == true){
                    return true;
               }
               int col = stack.peek();
               for(int i=0 ; i<initCols.length ; i++){
                    if(col==initCols[i]){
                         stack.pop();
                         System.out.println("Stack: "+stack);
                         if(stack.empty() == true){
                              return true;
                         }
                         continue mainloop;
                    }
               }
               col = stack.peek();
               subloop:
               while(row<=boardSize){
                    if(board[col][row]=='-'){
                         list.addRight(new Queen(col,row));
                         visualize();
                         row = 1;
                         stack.pop();
                         if(stack.empty() == true){
                              return true;
                         }
                         System.out.println("Stack: "+stack);
                         continue mainloop;
                    }
                    row++;
                    // System.out.println(row);
               }
               //if reaches here, current col can't be placed.
               //okay, bug. when consecutive initCols, breaks.
               boolean uh = false;
               // int prevCol = col-1;
               int prevCol = col;
               uhloop:
               while(uh==false){
                    prevCol--;
                    if(prevCol<1){
                         return false;
                    }
                    for(int i=0 ; i<initCols.length ; i++){
                         if(prevCol==initCols[i]){
                              continue uhloop;
                         }
                    }
                    uh=true;
               }
               // for(int i=0 ; i<initCols.length ; i++){
               //      if(prevCol==initCols[i]){
               //           prevCol--;
               //           if(prevCol<1){
               //                return false;
               //           }
               //      }
               // }
               // if(prevCol<1){
               //      return false;
               // }
               stack.push(prevCol);
               System.out.println("Stack: "+stack);
               row = list.myRow(prevCol);
               list.remove(prevCol,row);
               visualize();
               row++;
          }
          // return false;
     }
     private static void stackInit(int n, int[] ignore){
          outerloop:     //did not know you can do jumps like hat o: YES!(:
          for(int i=n ; i>0 ; i--){
               // for(int j=0 ; j<ignore.length ; j++){
               //      if(i==ignore[j]){
               //           continue outerloop;
               //      }
               // }
               stack.push(i);
          }
     }
     private static void printStack(){
          System.out.println("The stack: "+stack);
     }
     private static void test1(){
          boardSize=5;
          list = new DoublyLinkedList();
          list.addRight(new Queen(1,1));
          list.addRight(new Queen(2,3));
          visualize();
          System.exit(1);
     }
     private static void visualize(){
          //this just places queens.
          board = new char[boardSize+1][boardSize+1];
          for(int k=list.getSize() ; k>=1 ; k--){
               Queen temp = list.traverse(k);
               for(int i=1 ; i<=boardSize ; i++){
                    for(int j=1 ; j<=boardSize ; j++){
                         if(i==temp.col && j==temp.row){
                              board[i][j]='Q';
                         }
                         else{
                              if(board[i][j]=='\0'){
                                   board[i][j]='-';
                              }
                         }
                    }
               }
          }
          updateBlocked();
          print2Dboard(board);
     }
     private static void updateBlocked(){
          for(int i=1 ; i<=boardSize ; i++){
               for(int j=1 ; j<=boardSize ; j++){
                    if(board[i][j]=='Q'){
                         for(int k=0 ; ((i+k)<(boardSize+1)) && ((j+k)<(boardSize+1)) ; k++){
                              if(board[i+k][j+k]=='Q'){
                                   continue;
                              }
                              board[i+k][j+k]='x';
                         }
                         for(int k=0 ; ((i+k)<(boardSize+1)) && ((j-k)>0) ; k++){
                              if(board[i+k][j-k]=='Q'){
                                   continue;
                              }
                              board[i+k][j-k]='x';
                         }
                         for(int k=0 ; ((i-k)>0) && ((j+k)<(boardSize+1)) ; k++){
                              if(board[i-k][j+k]=='Q'){
                                   continue;
                              }
                              board[i-k][j+k]='x';
                         }
                         for(int k=0 ; ((i-k)>0) && ((j-k)>0) ; k++){
                              if(board[i-k][j-k]=='Q'){
                                   continue;
                              }
                              board[i-k][j-k]='x';
                         }
                         for(int k=0 ; (i+k)<(boardSize+1) ; k++){
                              if(board[i+k][j]=='Q'){
                                   continue;
                              }
                              board[i+k][j]='x';
                         }
                         for(int k=0 ; (i-k)>0 ; k++){
                              if(board[i-k][j]=='Q'){
                                   continue;
                              }
                              board[i-k][j]='x';
                         }
                         for(int k=0 ; (j+k)<(boardSize+1) ; k++){
                              if(board[i][j+k]=='Q'){
                                   continue;
                              }
                              board[i][j+k]='x';
                         }
                         for(int k=0 ; (j-k)>0 ; k++){
                              if(board[i][j-k]=='Q'){
                                   continue;
                              }
                              board[i][j-k]='x';
                         }
                    }
               }
          }
     }
     private static void print2Dboard(char[][] in){
          // System.out.print("  -------------------\n");
          System.out.print("  --");
          for(int i=0 ; i<boardSize ; i++){
               System.out.print("--");
          }
          System.out.print("-\n");
          for(int i=boardSize ; i>=1 ; i--){
               if(i<10){
                    System.out.print(i+" | ");
               }
               else{
                    System.out.print(i+"| ");
               }
               for(int j=1 ; j<=boardSize ; j++){
                    System.out.print(in[j][i] + " ");
               }
               System.out.print("|\n");
          }
          // System.out.print("  -------------------\n");
          System.out.print("  --");
          for(int i=0 ; i<boardSize ; i++){
               System.out.print("--");
          }
          System.out.print("-\n");
          // System.out.print("    1 2 3 4 5 6 7 8\n");
          System.out.print("    ");
          for(int i=1 ; i<=boardSize ; i++){
               System.out.print(i+" ");
          }
          System.out.print("\n");
     }
}
