package list;

class Test{

public static void main(String[] args){
    DList l1 = new DList();
    l1.insertFront(new String("1"));
    l1.insertFront(new String("2"));
    l1.insertBack(new String("3"));
    l1.insertBack(new String("4"));
    l1.insertBack(new String("5"));
    System.out.println(l1.toString());
    System.out.println("Front: " + l1.front().item);
    System.out.println("Back: " + l1.back().item);
    System.out.println("the item of Node after node2 is: " + l1.next(l1.next(l1.next(l1.head))).item);
    System.out.println("the item of Node before node2 is: " + l1.prev(l1.next(l1.next(l1.head))).item);
    l1.insertBefore(new String("10"), l1.next(l1.next(l1.head)));
    System.out.println("insertFront: " + l1.toString());
    l1.insertAfter(new String("11"), l1.next(l1.next(l1.head)));
    System.out.println("insertBack: " + l1.toString());
    l1.remove(l1.next(l1.next(l1.head)));
    System.out.println("remove: " + l1.toString());
    System.out.println("length: " + l1.size);

    /*LockDList*/
    LockDList l2 = new LockDList();
    l2.insertAfter(new String("10"), l2.head);
    l2.insertAfter(new String("11"), l2.head);
    l2.insertAfter(new String("12"), l2.head);
    /*Lock*/
    System.out.println("Lock Befor calling remove:" + l2.toString());
    l2.remove(l2.next(l2.next(l2.head)));
    System.out.println("Lock After calling remove:" + l2.toString());
    /*Unlock*/
    LockDListNode pNode = (LockDListNode)l2.next(l2.next(l2.head));
    pNode.status = false;
    System.out.println("Unlock Befor calling remove:" + l2.toString());
    l2.remove(pNode);
    System.out.println("unlock After calling remove:" + l2.toString());
  }
}