/* Set.java */

import list.*;

/**
 *  A Set is a collection of Comparable elements stored in sorted order.
 *  Duplicate elements are not permitted in a Set.
 **/
public class Set {
  /* Fill in the data fields here. */
  List list;
  /**
   * Set ADT invariants:
   *  1)  The Set's elements must be precisely the elements of the List.
   *  2)  The List must always contain Comparable elements, and those elements 
   *      must always be sorted in ascending order.
   *  3)  No two elements in the List may be equal according to compareTo().
   **/

  /**
   *  Constructs an empty Set. 
   *
   *  Performance:  runs in O(1) time.
   **/
  public Set() { 
    // Your solution here.
    //list = new DList();
    list = new SList();
  }

  /**
   *  cardinality() returns the number of elements in this Set.
   *
   *  Performance:  runs in O(1) time.
   **/
  public int cardinality() {
    // Replace the following line with your solution.
    return list.length();
  }

  /**
   *  insert() inserts a Comparable element into this Set.
   *
   *  Sets are maintained in sorted order.  The ordering is specified by the
   *  compareTo() method of the java.lang.Comparable interface.
   *
   *  Performance:  runs in O(this.cardinality()) time.
   **/
  public void insert(Comparable c) throws InvalidNodeException{
    // Your solution here.
    if(list.length() == 0){ // no elements in the set
      list.insertFront(c);
    }else{
      ListNode a = list.front(), b;
      for(int i = 1; i <= list.length(); i++){
        if(a.compareTo(c) == 0){ // a == c
          break;
        }else if(a.compareTo(c) == 1){ // a > c
          a.insertBefore(c);
          break;
        }else{   //a < c
          if(i == list.length()){  // the end node of list
            a.insertAfter(c);
            break;
          }else{
             b = a.next();
            if(b.compareTo(c) == 1){  // b > c
              a.insertAfter(c);
              break;
            }else if(b.compareTo(c) == 0){  //b == c
              break;
            }else{  //b < c
                a = b;
            }
          }
        }
      }
    }    
  }

  /**
   *  union() modifies this Set so that it contains all the elements it
   *  started with, plus all the elements of s.  The Set s is NOT modified.
   *  Make sure that duplicate elements are not created.
   *
   *  Performance:  Must run in O(this.cardinality() + s.cardinality()) time.
   *
   *  Your implementation should NOT copy elements of s or "this", though it
   *  will copy _references_ to the elements of s.  Your implementation will
   *  create new _nodes_ for the elements of s that are added to "this", but
   *  you should reuse the nodes that are already part of "this".
   *
   *  DO NOT MODIFY THE SET s.
   *  DO NOT ATTEMPT TO COPY ELEMENTS; just copy _references_ to them.
   **/
  public void union(Set s) throws InvalidNodeException{
    // Your solution here.
    if(s.list.length() == 0) // no elements in 's' set
      return;

    ListNode pNode = s.list.front();
    for(int i = 1; i <= s.list.length(); i++){ 
      insert((Comparable)pNode.item());
      pNode = pNode.next();
    }
  }

  /**
   *  intersect() modifies this Set so that it contains the intersection of
   *  its own elements and the elements of s.  The Set s is NOT modified.
   *
   *  Performance:  Must run in O(this.cardinality() + s.cardinality()) time.
   *
   *  Do not construct any new ListNodes during the execution of intersect.
   *  Reuse the nodes of "this" that will be in the intersection.
   *
   *  DO NOT MODIFY THE SET s.
   *  DO NOT CONSTRUCT ANY NEW NODES.
   *  DO NOT ATTEMPT TO COPY ELEMENTS.
   **/
  public void intersect(Set s) throws InvalidNodeException{
    // Your solution here.
    ListNode pNode, qNode, tmp;
    int i, j;
    if( (list.length() > 0 ) && (s.list.length() > 0) ){
      pNode = list.front();
      for(i = 1; i <= list.length(); i++){
        qNode = s.list.front();

        for(j = 1; j <= s.list.length(); j++){
          if(pNode.compareTo(qNode.item()) == 0)
            break;
          
          qNode = qNode.next();
          if(j == s.list.length()){ //no same element
            tmp = pNode;
            pNode = pNode.next();
            tmp.remove();
          }
        }

        if(j <= s.list.length())
          pNode = pNode.next();
      }
    }else{
      if(list.length() == 0)  // Null Set
        return;
      if(s.list.length() == 0){ //Clean this set if 's' set has no elements.
        pNode = list.back();
        for(i = 1; i <= list.length(); i++){
          pNode.remove();
          pNode = pNode.prev();
        }
      }     
    }
  }

  /**
   *  toString() returns a String representation of this Set.  The String must
   *  have the following format:
   *    {  } for an empty Set.  No spaces before "{" or after "}"; two spaces
   *            between them.
   *    {  1  2  3  } for a Set of three Integer elements.  No spaces before
   *            "{" or after "}"; two spaces before and after each element.
   *            Elements are printed with their own toString method, whatever
   *            that may be.  The elements must appear in sorted order, from
   *            lowest to highest according to the compareTo() method.
   *
   *  WARNING:  THE AUTOGRADER EXPECTS YOU TO PRINT SETS IN _EXACTLY_ THIS
   *            FORMAT, RIGHT UP TO THE TWO SPACES BETWEEN ELEMENTS.  ANY
   *            DEVIATIONS WILL LOSE POINTS.
   **/
  public String toString(){
    // Replace the following line with your solution.
    if(list.length() == 0)
      return "{  }";

    String setStr = "{  ";
    ListNode pNode = list.front();
    for(int i = 1; i <= list.length(); i++){
      try{
         setStr += pNode.item() + "  ";
          pNode = pNode.next();
      }catch (InvalidNodeException lbe) {
        System.out.println("p.next() should throw an exception, and did.");
      }
    }
    setStr += "}";
    return setStr;
  }

  public static void main(String[] argv) throws InvalidNodeException{
    Set s = new Set();
    s.insert(new Integer(3));
    s.insert(new Integer(4));
    s.insert(new Integer(3));
    System.out.println("Set s = " + s);

    Set s2 = new Set();
    s2.insert(new Integer(4));
    s2.insert(new Integer(5));
    s2.insert(new Integer(5));
    System.out.println("Set s2 = " + s2);

    Set s3 = new Set();
    s3.insert(new Integer(5));
    s3.insert(new Integer(3));
    s3.insert(new Integer(8));
    System.out.println("Set s3 = " + s3);

    s.union(s2);
    System.out.println("After s.union(s2), s = " + s);

    s.intersect(s3);
    System.out.println("After s.intersect(s3), s = " + s);

    System.out.println("s.cardinality() = " + s.cardinality());
    // You may want to add more (ungraded) test code here.
  }
}
