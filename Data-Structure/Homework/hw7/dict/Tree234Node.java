/* Tree234Node.java */

package dict;

/**
 *  A Tree234Node is a node in a 2-3-4 tree (Tree234 class).
 *
 *  DO NOT CHANGE ANYTHING IN THIS FILE.
 *  You may add helper methods and additional constructors, though.
 **/
class Tree234Node {

  /**
   *  keys is the number of keys in this node.  Always 1, 2, or 3.
   *  key1 through key3 are the keys of this node.  If keys == 1, the value
   *    of key2 doesn't matter.  If keys < 3, the value of key3 doesn't matter.
   *  parent is this node's parent; null if this is the root.
   *  child1 through child4 are the children of this node.  If this is a leaf
   *    node, they must all be set to null.  If this node has no third and/or
   *    fourth child, child3 and/or child4 must be set to null.
   **/
  int keys;
  int key1;
  int key2;
  int key3;
  Tree234Node parent;
  Tree234Node child1;
  Tree234Node child2;
  Tree234Node child3;
  Tree234Node child4;

  Tree234Node(Tree234Node p, int key) {
    keys = 1;
    key1 = key;
    parent = p;
    child1 = null;
    child2 = null;
    child3 = null;
    child4 = null;
  }

  public int addKeyInParent(Tree234Node p, int key){
    if(p != null && p.keys < 3){
      switch(p.keys){
        case 1:
          if(key < p.key1){
            p.key2 = p.key1;
            p.key1 = key;
            p.child3 = p.child2;
            p.keys++;
            return 1;
          }else{
            p.key2 = key;
            p.keys++;
            return 2;
          }
        case 2:
          if(key < p.key1){
            p.key3 = p.key2;
            p.key2 = p.key1;
            p.key1 = key;
            p.child4 = p.child3;
            p.child3 = p.child2;
            p.keys++;
            return 1;
          }else if(key > p.key1 && key < p.key2){
            p.key3 = p.key2;
            p.key2 = key;
            p.child4 = p.child3;
            p.keys++;
            return 2;
          }else if(key > p.key2){
            p.key3 = key;
            p.keys++;
            return 3;
          }
        default:
          return 0;
      }
    }
    return 0;
  }

  public void addKeyInLeafNode(int key){
    if(this != null && keys < 3){
      switch(keys){
        case 1:
          if(key < key1){
            key2 = key1;
            key1 = key;
          }else{
            key2 = key;
          }
        case 2:
          if(key < key1){
            key3 = key2;
            key2 = key1;
            key1 = key;
          }else if(key > key1 && key < key2){
            key3 = key2;
            key2 = key;
          }else if(key > key2){
            key3 = key;
          }
        default:
          keys++;
      }
    }
  }

  public Tree234Node splitThreeKeyNode(Tree234Node p, int keyNum){
    Tree234Node childNode1 = new Tree234Node(p, key1);
    Tree234Node childNode2 = new Tree234Node(p, key3);
    // set parent's chlidren
    switch(keyNum){
      case 1:
        p.child1 = childNode1;
        p.child2 = childNode2; 
        break;
      case 2:
        p.child2 = childNode1;
        p.child3 = childNode2;
        break;
      case 3:
        p.child3 = childNode1;
        p.child4 = childNode2;
        break;
      default:
        break;
    }
    //set node's children
    childNode1.child1 = child1;
    childNode1.child2 = child2;
    childNode2.child1 = child3;
    childNode2.child2 = child4;
    if(child1 != null)
    	child1.parent = childNode1;
    if(child2 != null)
    	child2.parent = childNode1;
    if(child3 != null)
    	child3.parent = childNode2;
    if(child4 != null)
    	child4.parent = childNode2;
    // set itself node invaild
    this.disableNode();
    return p;
  }

  public void disableNode(){
    if(this != null){
      keys = 0;
      parent = null;
      child1 = null;
      child2 = null;
      child3 = null;
      child4 = null;
    }
  }
  /**
   *  toString() recursively prints this Tree234Node and its descendants as
   *  a String.  Each node is printed in the form such as (for a 3-key node)
   *
   *      (child1)key1(child2)key2(child3)key3(child4)
   *
   *  where each child is a recursive call to toString, and null children
   *  are printed as a space with no parentheses.  Here's an example.
   *      ((1)7(11 16)22(23)28(37 49))50((60)84(86 95 100))
   *
   *  DO NOT CHANGE THIS METHOD.
   **/
  public String toString() {
    String s = "";

    if (child1 != null) {
      s = "(" + child1.toString() + ")";
    }
    s = s + key1;
    if (child2 != null) {
      s = s + "(" + child2.toString() + ")";
    } else if (keys > 1) {
      s = s + " ";
    }
    if (keys > 1) {
      s = s + key2;
      if (child3 != null) {
        s = s + "(" + child3.toString() + ")";
      } else if (keys > 2) {
        s = s + " ";
      }
    }
    if (keys > 2) {
      s = s + key3;
      if (child4 != null) {
        s = s + "(" + child4.toString() + ")";
      }
    }
    return s;
  }

  /**
   *  printSubtree() recursively prints this Tree234Node and its descendants as
   *  a tree (albeit sideways).
   *
   *  You're welcome to change this method if you like.  It won't be tested.
   **/
  public void printSubtree(int spaces) {
    if (child4 != null) {
      child4.printSubtree(spaces + 5);
    }
    if (keys == 3) {
      for (int i = 0; i < spaces; i++) {
        System.out.print(" ");
      }
      System.out.println(key3);
    }

    if (child3 != null) {
      child3.printSubtree(spaces + 5);
    }
    if (keys > 1) {
      for (int i = 0; i < spaces; i++) {
        System.out.print(" ");
      }
      System.out.println(key2);
    }

    if (child2 != null) {
      child2.printSubtree(spaces + 5);
    }
    for (int i = 0; i < spaces; i++) {
      System.out.print(" ");
    }
    System.out.println(key1);

    if (child1 != null) {
      child1.printSubtree(spaces + 5);
    }
  }
}
