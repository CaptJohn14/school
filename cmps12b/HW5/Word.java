import java.util.*;

public class Word implements Comparator<Word>, Comparable<Word>{
// public class Word implements Comparable<Word>{
     int f;
     String w;
     public Word(){

     }
     public Word(String w2, int f2){
          f = f2;
          w = w2;
     }
     public String getWord(){
          return this.w;
     }
     public int getFreq(){
          return this.f;
     }
     @Override
     public int compareTo(Word compareWord){
          return (this.w).compareTo(compareWord.w);
          // return (compareWord.w).compareTo(this.w);
          // return compareWord.f - this.f;
          // return this.f - compareWord.f;
     }
     // @Override
     public int compare(Word w1, Word w2){
          int comp = w2.f - w1.f;
          if(comp == 0) {
               comp = (w1.w).compareTo(w2.w);
          }
          // return w2.f - w1.f;
          return comp;
          // return (w1.w).compareTo(w2.w);
     }

     // public Comparator<Word> NameComparator = new Comparator<Word>(){
     //      @Override
     //      public int compare(Word w1, Word w2){
     //           // return w1.getWord().compareTo(w2.getWord());
     //           return (w2.w).compareTo(w1.w);
     //      }
     // };
     // public static Comparator<Word> FreqComparator = new Comparator<Word>(){
     //      @Override
     //      public int compare(Word w1, Word w2){
     //           return w2.f - w1.f;
     //      }
     // };
}
