/* ListSorts.java */

import list.*;

public class ListSorts {

  private final static int SORTSIZE = 1000000;

  /**
   *  makeQueueOfQueues() makes a queue of queues, each containing one item
   *  of q.  Upon completion of this method, q is empty.
   *  @param q is a LinkedQueue of objects.
   *  @return a LinkedQueue containing LinkedQueue objects, each of which
   *    contains one object from q.
   **/
  public static LinkedQueue makeQueueOfQueues(LinkedQueue q) {
    if(q.isEmpty())
      return null;
    // Replace the following line with your solution.
    LinkedQueue queneList = new LinkedQueue();
    try{
      while(!q.isEmpty()){
        LinkedQueue quene = new LinkedQueue();
        quene.enqueue(q.dequeue());
        queneList.enqueue(quene);
      }
    }catch(QueueEmptyException e){
      System.out.println(e);
    }

    return queneList;
  }

  /**
   *  mergeSortedQueues() merges two sorted queues into a third.  On completion
   *  of this method, q1 and q2 are empty, and their items have been merged
   *  into the returned queue.
   *  @param q1 is LinkedQueue of Comparable objects, sorted from smallest 
   *    to largest.
   *  @param q2 is LinkedQueue of Comparable objects, sorted from smallest 
   *    to largest.
   *  @return a LinkedQueue containing all the Comparable objects from q1 
   *   and q2 (and nothing else), sorted from smallest to largest.
   **/
  public static LinkedQueue mergeSortedQueues(LinkedQueue q1, LinkedQueue q2) {
    // Replace the following line with your solution.
    LinkedQueue q = new LinkedQueue();
    try{
      while(!q1.isEmpty() && !q2.isEmpty()){
        Object o1 = q1.front();
        Object o2 = q2.front();
        int comp = ((Comparable)o1).compareTo(o2);
        if(comp > 0){
          q.enqueue(o2);
          q2.dequeue();
        }else if(comp < 0){
          q.enqueue(o1);
          q1.dequeue();
        }else{
          q.enqueue(o1);
          q.enqueue(o2);
          q1.dequeue();
          q2.dequeue();
        }
      }
    }catch(QueueEmptyException e){
      System.out.println(e);
    }

    if(q1.isEmpty()){
      q.append(q2);
    }else{
      q.append(q1);
    }

    return q;
  }

  /**
   *  partition() partitions qIn using the pivot item.  On completion of
   *  this method, qIn is empty, and its items have been moved to qSmall,
   *  qEquals, and qLarge, according to their relationship to the pivot.
   *  @param qIn is a LinkedQueue of Comparable objects.
   *  @param pivot is a Comparable item used for partitioning.
   *  @param qSmall is a LinkedQueue, in which all items less than pivot
   *    will be enqueued.
   *  @param qEquals is a LinkedQueue, in which all items equal to the pivot
   *    will be enqueued.
   *  @param qLarge is a LinkedQueue, in which all items greater than pivot
   *    will be enqueued.  
   **/   
  public static void partition(LinkedQueue qIn, Comparable pivot, 
                               LinkedQueue qSmall, LinkedQueue qEquals, 
                               LinkedQueue qLarge) {
    // Your solution here.
    if(qIn.size() < 2)
      return;

    try{
      int comp;
      while(!qIn.isEmpty()){
        comp = pivot.compareTo(qIn.front());
        if(comp < 0){
          qLarge.enqueue(qIn.dequeue());
        }else if(comp > 0){
          qSmall.enqueue(qIn.dequeue());
        }else{
          qEquals.enqueue(qIn.dequeue());
        }
      }
    }catch(QueueEmptyException e){
      System.out.println(e);
    }
  }

  /**
   *  mergeSort() sorts q from smallest to largest using mergesort.
   *  @param q is a LinkedQueue of Comparable objects.
   **/
  public static void mergeSort(LinkedQueue q) {
    // Your solution here.
    LinkedQueue queneList = makeQueueOfQueues(q);
    if(queneList == null)
      return;

    LinkedQueue mergedQuene = null;
    try{
      LinkedQueue q1, q2;
      while(queneList.size() > 1){
        q1 = (LinkedQueue)queneList.dequeue();
        q2 = (LinkedQueue)queneList.dequeue();
        mergedQuene = mergeSortedQueues(q1, q2);
        queneList.enqueue(mergedQuene);
      }
      if(queneList.size() == 1){
        q.append((LinkedQueue)queneList.dequeue());
      }
    }catch(QueueEmptyException e){
      System.out.println(e);
    }
  }

  /**
   *  quickSort() sorts q from smallest to largest using quicksort.
   *  @param q is a LinkedQueue of Comparable objects.
   **/
  public static void quickSort(LinkedQueue q) {
    // Your solution here.
    int index = (int)(Math.random() * q.size() + 1);
    Comparable pivot = (Comparable)q.nth(index);

    LinkedQueue qSmall = new LinkedQueue() , qEquals = new LinkedQueue(), qLarge = new LinkedQueue();
    partition(q, pivot, qSmall, qEquals, qLarge);
    if(qSmall.size() > 1){
      quickSort(qSmall);
    }
    if(qLarge.size() > 1){
      quickSort(qLarge);
    }
    q.append(qSmall);
    q.append(qEquals);
    q.append(qLarge);
  }

  /**
   *  makeRandom() builds a LinkedQueue of the indicated size containing
   *  Integer items.  The items are randomly chosen between 0 and size - 1.
   *  @param size is the size of the resulting LinkedQueue.
   **/
  public static LinkedQueue makeRandom(int size) {
    LinkedQueue q = new LinkedQueue();
    for (int i = 0; i < size; i++) {
      q.enqueue(new Integer((int) (size * Math.random())));
    }
    return q;
  }

  /**
   *  main() performs some tests on mergesort and quicksort.  Feel free to add
   *  more tests of your own to make sure your algorithms works on boundary
   *  cases.  Your test code will not be graded.
   **/
  public static void main(String [] args) {

    LinkedQueue q = makeRandom(10);
    System.out.println(q.toString());
    mergeSort(q);
    System.out.println(q.toString());

    q = makeRandom(10);
    System.out.println(q.toString());
    quickSort(q);
    System.out.println(q.toString());

    //Remove these comments for Part III.
    Timer stopWatch = new Timer();
    q = makeRandom(SORTSIZE);
    stopWatch.start();
    mergeSort(q);
    stopWatch.stop();
    System.out.println("Mergesort time, " + SORTSIZE + " Integers:  " +
                       stopWatch.elapsed() + " msec.");

    stopWatch.reset();
    q = makeRandom(SORTSIZE);
    stopWatch.start();
    quickSort(q);
    stopWatch.stop();
    System.out.println("Quicksort time, " + SORTSIZE + " Integers:  " +
                       stopWatch.elapsed() + " msec.");
    
  }

}
