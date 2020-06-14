import java.lang.Runnable;
import java.lang.Thread;
import java.lang.Exception;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
public class Test{
  private static int numberOfThreads = 5; //number of threads running

  public static void main(String[] args){
    //starting value for a thread. It is initialised at 1
    int starting = 1;
    // the increment value for each thread
    int increment,number;
     // the arraylist containing all the 6 threads
    ArrayList<Thread> threadList = new ArrayList<Thread>();
    //the final arrayList containing all the primeNumbers
    ArrayList<Integer> finalPrimeList = new ArrayList<Integer>();
    try{
      // read the value the user passed as argument
      number = Integer.parseInt(args[0]);
    }catch(IllegalArgumentException e){
      System.out.println("Invalid Input");
      return;
    }
    // The increment value is determined
    increment = number/numberOfThreads;
    //creating the threads
    for(int i = 0; i < numberOfThreads - 1 ;i++){
      threadList.add(new Thread(new PrimeNumber(starting, starting + increment , "Output" + i +".txt", false)));
      starting = starting + increment;
    }
    threadList.add(new Thread(new PrimeNumber(starting, number , "Output" + (numberOfThreads - 1) + ".txt", true)));
    //Each thread starts generating prime numbers
    for(int j = 0; j < numberOfThreads;j++){
      threadList.get(j).start();
    }
    //The main program waits for each thread to finish executing
    try{
      for(int k = 0; k < numberOfThreads;k++){
        threadList.get(k).join();
      }
    }catch(Exception e){
      System.out.println("Process interrupted");
    }

    //Once all the created threads have finished working, we read all the prime numbers generated in the
    //different .txt files
    try{
      for(int l = 0; l < numberOfThreads;l++){
        Scanner scanner = new Scanner(new File("Output" + l + ".txt"));
        while(scanner.hasNextInt()){
          finalPrimeList.add(scanner.nextInt());
        }
      }

    }catch(Exception e){
      System.out.println("Process interrupted");
    }

    //Print all the prime numbers
    for(int m = 0; m < finalPrimeList.size();m++){
      System.out.println(finalPrimeList.get(m));
    }






}
}
