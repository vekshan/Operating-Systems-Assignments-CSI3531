import java.lang.Runnable;
import java.lang.Thread;
import java.util.ArrayList;
import java.io.PrintWriter;
import java.io.File;

public class PrimeNumber implements Runnable{
  // Number at which a thread starts determining prime numbers
  private int startNumber;
  private int stopNumber;
  //Name of the output file created
  private String fileName;
  //If this thread is the last thread
  private boolean last;

  public PrimeNumber(int startNumber, int stopNumber, String fileName, boolean last){
    this.startNumber = startNumber;
    this.stopNumber = stopNumber;
    this.fileName = fileName;
    this.last = last;
  }

  public void run(){
    ArrayList<Integer> primeNumbers = new ArrayList<Integer>();
    int i,j;
    for(i = startNumber; i < stopNumber; i++){
      if(isPrime(i)){
        primeNumbers.add(i);
      }
    }
    if(last){
      if(isPrime(stopNumber)){
        primeNumbers.add(stopNumber);
      }
    }

    try{
      PrintWriter temporaryFile = new PrintWriter(new File(fileName));
      for(int k = 0; k < primeNumbers.size(); k++){
        temporaryFile.println(primeNumbers.get(k));
      }
      temporaryFile.close();
    }catch(Exception e){
      System.out.println("Writing to files interrupted");
    }

    }

    //Algorithm for finding prime numbers
    public static boolean isPrime(int n){
      if(n == 1 || n == 0){
        return false;
      }
      for(int i = 2; i < n; i++ ){
        if(n % i == 0){
          return false;
        }
      }
      return true;
    }


}
