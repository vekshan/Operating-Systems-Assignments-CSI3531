import java.lang.Runnable;
import java.lang.Thread;


public class PrimeNumber implements Runnable{
  // Number at which a thread starts determining prime numbers
  private int number;

  public PrimeNumber(int number){
    this.number = number;
  }

  public void run(){
    int i,j;
    for(i = 1; i <= number; i++){
      for(j = 2; j < i; j++){
        if(i % j == 0){
          break;
        }
      }
      if(i == j){
        System.out.println(i);
      }
    }
  }

  public static void main(String[] args){
    int number;
    try{
      number = Integer.parseInt(args[0]);
    }catch(Exception e){
      System.out.println("Invalid input");
      return;
    }
    Thread th = new Thread(new PrimeNumber(number));
    th.start();
  }
}
