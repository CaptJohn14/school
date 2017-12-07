//----------------------------------------------------------------------
// Name: Johnson Le
// cruzID: jonle
// id number: 1512346
// File Name: NQueens.java
// Description:
//----------------------------------------------------------------------

import java.io.*;
import java.util.Scanner;
import java.util.ArrayList;

class NQueens{
     private static int[][] board;
     private static int[][] spotsFilled;
     private static int boardSize;
     private static int firstCol;
     private static int firstRow;

     //check to see if there is 2 command line arguments.
     public static void main(String[] args) throws IOException{
          if(args.length < 2){
               System.out.println("Usage: java -jar NQueens.jar <input file> <output file>");
               System.exit(1);
          }
          //open file and prepare the print
          Scanner inputFile = new Scanner(new File(args[0]));
          PrintWriter outputFile = new PrintWriter(new FileWriter(args[1]));

          while(inputFile.hasNextInt()){
               // System.out.println("__________________________________");
               // System.out.println("__________________________________");
               String input = inputFile.nextLine().trim() + " ";
               String[] token = input.split("\\s+");

               boardSize = Integer.parseInt(token[0]);
               firstCol = Integer.parseInt(token[1]);
               firstRow = Integer.parseInt(token[2]);
               board = new int[boardSize+1][boardSize+1];

               board[firstCol][firstRow]=1;
               // printCheck(board);
               spotsUpdate();

               if(solve(1)==true){
                    // System.out.println("solved");
                    for(int i=1 ; i<boardSize+1 ; i++){
                         for(int j=1 ; j<boardSize+1 ; j++){
                              if(board[i][j]==1){
                                   outputFile.print(+i+" "+j+" ");
                              }
                         }
                    }
                    outputFile.println("");
               }
               else{
                    outputFile.print("No solution\n");
               }
          }
          inputFile.close();
          outputFile.close();

     }
     //the main recursion stuff here. 999999th attempt ):
     private static boolean solve(int col){
          if(col==boardSize+1){
               return true;
          }
          int skipper=1;
          if(col==firstCol){
               if(solve(col+skipper)==true){
                    return true;
               }
          }
          if((col+1)==firstCol){
               skipper=2;
          }
          int row=0;
          while(true){
               row++;
               if(row==boardSize+1){
                    return false;
               }
               if(isSpotOpen(col,row)==true){
                    board[col][row]=1;
                    // System.out.println("___________________");
                    // printCheck(board);
                    spotsUpdate();
                    if(solve(col+skipper)==true){
                         return true;
                    }
                    board[col][row]=0;
                    // System.out.println("___________________");
                    // printCheck(board);
                    spotsUpdate();
               }
          }
     }
     //this will check if the spot given in arguments is open.
     private static boolean isSpotOpen(int x, int y){
          spotsUpdate();
          if(spotsFilled[x][y]==1){
               return false;
          }
          return true;
     }
     //basically edits a 2d array. if an index=1. takes in the board and edits accordingly.
     private static void spotsUpdate(){
          spotsFilled = new int[boardSize+1][boardSize+1];
          for(int i=1 ; i<boardSize+1 ; i++){
               for(int j=1 ; j<boardSize+1 ; j++){
                    if(board[i][j]==1){
                         for(int k=0 ; ((i+k)<(boardSize+1)) && ((j+k)<(boardSize+1)) ; k++){
                              spotsFilled[i+k][j+k]=1;
                         }
                         for(int k=0 ; ((i+k)<(boardSize+1)) && ((j-k)>0) ; k++){
                              spotsFilled[i+k][j-k]=1;
                         }
                         for(int k=0 ; ((i-k)>0) && ((j+k)<(boardSize+1)) ; k++){
                              spotsFilled[i-k][j+k]=1;
                         }
                         for(int k=0 ; ((i-k)>0) && ((j-k)>0) ; k++){
                              spotsFilled[i-k][j-k]=1;
                         }
                         for(int k=0 ; (i+k)<(boardSize+1) ; k++){
                              spotsFilled[i+k][j]=1;
                         }
                         for(int k=0 ; (i-k)>0 ; k++){
                              spotsFilled[i-k][j]=1;
                         }
                         for(int k=0 ; (j+k)<(boardSize+1) ; k++){
                              spotsFilled[i][j+k]=1;
                         }
                         for(int k=0 ; (j-k)>0 ; k++){
                              spotsFilled[i][j-k]=1;
                         }
                    }
               }
          }
     }
     //this just prints 0s and 1s to help me debug. makes it easier to see where my code messes up.
     private static void printCheck(int[][] in){
          int[][] out = new int[boardSize][boardSize];
          for(int a=0 ; a<boardSize ; a++){
               for(int b=0 ; b<boardSize ; b++){
                    out[a][b] = in[a+1][b+1];
               }
          }
          for(int i=boardSize-1 ; i>=0 ; i--){
               for(int j=0 ; j<boardSize ; j++){
                    System.out.print(out[j][i] + " ");
               }
               System.out.println("");
          }
     }
}
