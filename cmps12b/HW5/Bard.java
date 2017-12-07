import java.io.*;
import java.util.*;
import java.util.Comparator;
/* what supposed to do
Main Objective: for any word length x and number y, what is the most y most
     frequent word of length x.
~ grab input file line by line.

*/
class Bard{

     private static Hashtable<String, Integer> hash;
     private static ArrayList<Word>[] wordList = new ArrayList[40];

     public static void main(String[] args) throws IOException{
          if(args.length < 2){
               System.out.println("Usage: java -jar Bard.jar <input file> <output file>");
               System.exit(1);
          }
          hash =  new Hashtable<String, Integer>();
          for(int i=0 ; i<wordList.length ; i++){
               wordList[i] = new ArrayList<Word>();
          }
          Scanner shakespeare = new Scanner(new File("shakespeare.txt"));
          Scanner inputFile = new Scanner(new File(args[0]));
          PrintWriter outputFile = new PrintWriter(new FileWriter(args[1]));
          while(shakespeare.hasNextLine()){
               String input = shakespeare.nextLine();
               StringParseToHash(input);
          }
          hashToList();
          // System.out.println(("world").compareTo("hello"));
          for(int i=0 ; i<wordList.length ; i++){
               // Collections.sort(wordList[i]);
               Collections.sort(wordList[i], new Word());
               // Collections.sort(wordList[i], new Word());
          }
          // System.out.println("15:4 ~ "+wordList[15].get(4).w);
          // System.out.println("15:5 ~ "+wordList[15].get(5).w);
          // System.out.println("15:6 ~ "+wordList[15].get(6).w);
          // printList(wordList[15]);
          while(inputFile.hasNextLine()){
               String input = inputFile.nextLine();
               String[] token = input.split("\\s+");
               int x = Integer.parseInt(token[0]);
               int y = Integer.parseInt(token[1]);
               // System.out.println(x + " " + y);
               if(wordList[x].isEmpty()){
                    outputFile.print("-");
               }
               else if(y>=wordList[x].size()){
                    outputFile.print("-");
               }
               else{
                    outputFile.print(wordList[x].get(y).w);
               }
               // int[] index = new int[token.length];
               // for(int i=0 ; i < token.length ; i++){
               //      index[i] = Integer.parseInt(token[i]);
               // }
               // for(int i=0 ; i<index.length ; i=i+2){
               //      // if(index[i] >= wordList.length){
               //      //      outputFile.print("-");
               //      //      continue;
               //      // }
               //      if(wordList[index[i]].isEmpty()){
               //           outputFile.print("-");
               //           continue;
               //      }
               //      else if((index[i+1]) >= wordList[index[i]].size()){
               //           outputFile.print("-");
               //           continue;
               //      }
               //      else{
               //           outputFile.print(wordList[index[i]].get(index[i+1]).w);
               //      }
               // }
               outputFile.print("\n");
          }
          inputFile.close();
          shakespeare.close();
          outputFile.close();

     }
     private static void StringParseToHash(String input){
          input = input.replaceAll("[?,.!:;<>()]"," ").replaceAll("\\]"," ").replaceAll("\\[", " ").replaceAll("\\s", " ");
          String[] token = input.toLowerCase().trim().split("\\s+");
          for(int i=0 ; i<token.length ; i++){
               if(hash.containsKey(token[i])){
                    int n = hash.get(token[i])+1;
                    hash.replace(token[i], n);
               }
               else{
                    hash.put(token[i], 1);
               }
          }
     }
     private static void hashToList(){
          Enumeration<String> s = hash.keys();
          while(s.hasMoreElements()){
               String string = s.nextElement();
               int index = string.length();
               Word temp = new Word(string, hash.get(string));
               wordList[index].add(temp);
          }
          // System.out.println(wordList[3].get(2).w);
     }
     private static void printHash(){
          Enumeration<String> s = hash.keys();
          Enumeration<Integer> n = hash.elements();
          while(s.hasMoreElements()){
               System.out.println(s.nextElement() + " - " + n.nextElement());
          }
     }
     private static void printArrayList(){
          for(int i=0 ; i<wordList.length ; i++){
               if(!wordList[i].isEmpty()){
                    System.out.print(i+ ": ");
                    printList(wordList[i]);
                    System.out.print("\n");
               }
          }
     }
     private static void printList(ArrayList<Word> list){
          if(list.isEmpty()){
               System.out.println("List is empty");
               return;
          }
          int n = list.size();
          // System.out.println(n);
          for(int i=0; i<n ; i++){
               System.out.print("["+list.get(i).w + " - " + list.get(i).f + "] ");
          }
     }
     private static void printEmpties(){
          for(int i=0 ; i<wordList.length ; i++){
               if(wordList[i].isEmpty()){
                    System.out.println(i);
               }
          }
     }
     private static int[] findEmpty(){
          int n = 0;
          for(int i=0 ; i<wordList.length ; i++){
               if(wordList[i].isEmpty()){
                    n++;
               }
          }
          int[] temp = new int[n];
          int index = 0;
          for(int i=0 ; i<wordList.length ; i++){
               if(wordList[i].isEmpty()){
                    temp[index]=i;
                    index++;
               }
          }
          return temp;
     }
}
