/* HashTableChained.java */

package dict;
import list.*;

/**
 *  HashTableChained implements a Dictionary as a hash table with chaining.
 *  All objects used as keys must have a valid hashCode() method, which is
 *  used to determine which bucket of the hash table an entry is stored in.
 *  Each object's hashCode() is presumed to return an int between
 *  Integer.MIN_VALUE and Integer.MAX_VALUE.  The HashTableChained class
 *  implements only the compression function, which maps the hash code to
 *  a bucket in the table's range.
 *
 *  DO NOT CHANGE ANY PROTOTYPES IN THIS FILE.
 **/

public class HashTableChained implements Dictionary {

  /**
   *  Place any data fields here.
   **/
  private SList[] entry_list;
  private int N;
  private int size;
  private int collisions;

  private void getMaxPrime(int num){
    if(num < 2){
      N = 3;
      return;
    }

    int i;
    for(i = num - 1; i >= 2; i--)
      if(num % i == 0) break;

    if(i == 1){
      N = num;
      return;
    }
    else{
      getMaxPrime(--num);
    }
  }

  private void initArray(){
    for(int i = 0; i < N; i++)
      entry_list[i] = new SList();
  }
  /** 
   *  Construct a new empty hash table intended to hold roughly sizeEstimate
   *  entries.  (The precise number of buckets is up to you, but we recommend
   *  you use a prime number, and shoot for a load factor between 0.5 and 1.)
   **/

  public HashTableChained(int sizeEstimate) {
    // Your solution here.
    getMaxPrime(sizeEstimate);
    entry_list = new SList[N];
    initArray();
  }

  /** 
   *  Construct a new empty hash table with a default size.  Say, a prime in
   *  the neighborhood of 100.
   **/

  public HashTableChained() {
    // Your solution here.
    N = 97;
    entry_list = new SList[N];
    initArray();
  }

  /**
   *  Converts a hash code in the range Integer.MIN_VALUE...Integer.MAX_VALUE
   *  to a value in the range 0...(size of hash table) - 1.
   *
   *  This function should have package protection (so we can test it), and
   *  should be used by insert, find, and remove.
   **/

  int compFunction(int code) {
    // Replace the following line with your solution.
    return code % N;
  }

  /** 
   *  Returns the number of entries stored in the dictionary.  Entries with
   *  the same key (or even the same key and value) each still count as
   *  a separate entry.
   *  @return number of entries in the dictionary.
   **/

  public int size() {
    // Replace the following line with your solution.
    return size;
  }

  /** 
   *  Tests if the dictionary is empty.
   *
   *  @return true if the dictionary has no entries; false otherwise.
   **/

  public boolean isEmpty() {
    // Replace the following line with your solution.
    if(size == 0){
      return true;
     }else{
      return false;
    }
  }

  /**
   *  Create a new Entry object referencing the input key and associated value,
   *  and insert the entry into the dictionary.  Return a reference to the new
   *  entry.  Multiple entries with the same key (or even the same key and
   *  value) can coexist in the dictionary.
   *
   *  This method should run in O(1) time if the number of collisions is small.
   *
   *  @param key the key by which the entry can be retrieved.
   *  @param value an arbitrary object.
   *  @return an entry containing the key and value.
   **/

  public Entry insert(Object key, Object value) {
    // Replace the following line with your solution.
    Entry entry = new Entry(key, value);
    int bucket = compFunction(key.hashCode());
    //System.out.println(""+ bucket);
    entry_list[bucket].insertBack(entry);

    size++;
    return entry;
  }

  /** 
   *  Search for an entry with the specified key.  If such an entry is found,
   *  return it; otherwise return null.  If several entries have the specified
   *  key, choose one arbitrarily and return it.
   *
   *  This method should run in O(1) time if the number of collisions is small.
   *
   *  @param key the search key.
   *  @return an entry containing the key and an associated value, or null if
   *          no entry contains the specified key.
   **/

  public Entry find(Object key){
    // Replace the following line with your solution.
    int bucket = compFunction(key.hashCode());
    if(!entry_list[bucket].isEmpty()){
      ListNode node = entry_list[bucket].front();
      while(node.isValidNode()){
        try{
          if(key.equals(((Entry)node.item()).key())){
            return (Entry)node.item();
          }else{
            node = node.next();
          }
        }catch(InvalidNodeException e){
          System.out.println("Invaild Node");
        }
      }
    }
      
    return null;
  }

  /** 
   *  Remove an entry with the specified key.  If such an entry is found,
   *  remove it from the table and return it; otherwise return null.
   *  If several entries have the specified key, choose one arbitrarily, then
   *  remove and return it.
   *
   *  This method should run in O(1) time if the number of collisions is small.
   *
   *  @param key the search key.
   *  @return an entry containing the key and an associated value, or null if
   *          no entry contains the specified key.
   */

  public Entry remove(Object key) {
    // Replace the following line with your solution.
    int bucket = compFunction(key.hashCode());
    if(!entry_list[bucket].isEmpty()){
      ListNode node = entry_list[bucket].front();
      while(node.isValidNode()){
        try{
          if(key.equals(((Entry)node.item()).key())) {
            Entry entry = (Entry)node.item();
            node.remove();
            size--;
            return entry;
          }else{
            node = node.next();
          }
        }catch(InvalidNodeException e){
          System.out.println("Invaild Node");
        }
      }
    }
    
    return null;
  }

  /**
   *  Remove all entries from the dictionary.
   */
  public void makeEmpty() {
    // Your solution here.
    ListNode pNode;
    for(int i = 0; i < N; i++){
      if(!entry_list[i].isEmpty()){
         ListNode node = entry_list[i].front(); 
         while(node.isValidNode()){ 
          try{
            pNode = node.next();
            node.remove();
            node = pNode;
          }catch(InvalidNodeException e){
            System.out.println("Invaild Node");
          }
        }
      }
    }
    size = 0;
    collisions = 0;
  }

  public void showTable(){
    collisions = 0;
    for(int i = 0; i < N; i++){
      if(!entry_list[i].isEmpty()){
        String str = "HashTable" + "[" + i + "]: ";
        ListNode node = entry_list[i].front();
        while(node.isValidNode()){ 
          try{
            str += ((Integer)((Entry)node.item()).value()).intValue() + ", ";
            node = node.next();
          }catch(InvalidNodeException e){
            System.out.println(e);
          }
        }  
        System.out.println(str);  
        collisions += entry_list[i].length() - 1; 
      }
    }
    System.out.println("number of collisions:" + collisions);
  }
}
