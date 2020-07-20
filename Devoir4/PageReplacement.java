import java.util.HashSet;
import java.util.HashMap;
import java.util.Queue;
import java.util.Random;
import java.util.LinkedList;
import java.util.Iterator;

public class PageReplacement{
  private static final int DEFAULT_PAGE_NUMBERS = 15;
  private int page; //number of pages to generate randomly

  //constructor
  public PageReplacement(int page){
    this.page = page; // initialise the number of pages
  }

  public int getPage(){
    return page;
  }

  public static void main (String[] args){
    int page;
    if(args.length == 0){ // if no arguments given, use default number
      page = DEFAULT_PAGE_NUMBERS;
      System.out.println("No arguments specified! Using default number of pages : " + DEFAULT_PAGE_NUMBERS + ".");
    }else{
      try{
        //Read the user input
        page = Integer.parseInt(args[0]);
      }catch(IllegalArgumentException e){
        //If a number is not entered, print the error and exit the program
        System.err.println(e);
        return;
      }
    }

    PageReplacement p = new PageReplacement(page);

    //generate an array of page numbers between 0 and 9 inclusive
    Random random = new Random();
    int[] pageNumber = new int[p.getPage()];
    System.out.print("Random pages generated: ");
    for(int i = 0; i < pageNumber.length; i++){
      pageNumber[i] = random.nextInt(10);
      System.out.print(pageNumber[i] + " ");
    }
    System.out.println("");

    // run the FIFO algorithm with varying number of frames from 1 to 7
    for(int i = 1; i < 8; i++){
      System.out.println("FIFO algorithm with " + i + " frames.");
      p.fifo(i, pageNumber);
    }
    System.out.println("");
    for(int k = 1; k < 8; k++){
      System.out.println("LRU algorithm with " + k + " frames.");
      p.lru(k, pageNumber);
    }
  }

  //First in First out Algorithm
  public void fifo(int capacity, int[] pageNumbers){
    int pageFaults = 0; //counts the number of page faults

    HashSet<Integer> set = new HashSet<Integer>(capacity); // stores the numbers in the queue
    Queue<Integer> queue = new LinkedList<Integer>(); // linked list to prrserve the order of pages

    //traverse the array of pages
    for(int j = 0; j < pageNumbers.length; j++){

      if(set.size() < capacity ){ // there are frames available
        if( !set.contains(pageNumbers[j])){ // the given page is not in the set
          set.add(pageNumbers[j]); // add it to the set
          pageFaults++; // increase the number of page faults
          queue.add(pageNumbers[j]); // add it to the queue as well
        }
      }else{
        if(!set.contains(pageNumbers[j])){ // there are no frames available
          int toRemove = queue.peek(); //get the number at the front of the queue
          queue.poll(); // remove the number at the front of the queue
          set.remove(toRemove); // remove the number at the front of the queue from the set
          set.add(pageNumbers[j]); // add the new page number to the set
          queue.add(pageNumbers[j]); // add the new page number at the end of the queue
          pageFaults++; // increment the number of page faults
        }
      }
    }
    System.out.println("Number of faults: " + pageFaults);
  }

  //Least recently used algorithm
  public void lru(int capacity, int[] pageNumbers){
    int pageFaults = 0; // to record the number of faults
    HashSet<Integer> set = new HashSet<Integer>(capacity);
    HashMap<Integer,Integer> map = new HashMap<Integer, Integer>();

    for(int j = 0; j < pageNumbers.length; j++){
      if(set.size() < capacity){//there are frames available
        if(!set.contains(pageNumbers[j])){
          set.add(pageNumbers[j]);
          pageFaults++;
        }
        map.put(pageNumbers[j] ,j);
      }else{ // there are no available frames
        if(!set.contains(pageNumbers[j])){ //the set does not contain the given page
          int min = 1000;
          int toRemove = 1000 ;
          //iterate through the set to find the page that was the least recently used
          Iterator<Integer> iterator = set.iterator();
          while(iterator.hasNext()){
            int value = iterator.next();
            if(map.get(value) < min){
              min = map.get(value);
              toRemove = value;
            }
          }

          pageFaults++; // increment pageFaults
          set.remove(toRemove); // remove lru page from the set
          map.remove(toRemove);// remove lru page from the map

          set.add(pageNumbers[j]); // add the new page to the set
        }
        map.put(pageNumbers[j] , j); // add the page to the map, with the updated index

      }
    }
    System.out.println("Number of faults: " + pageFaults);
  }
}
