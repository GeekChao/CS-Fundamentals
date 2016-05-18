package list;

public class LockDListNode extends DListNode{

    boolean status;

	public LockDListNode(Object i, DListNode p, DListNode n){
		super(i, p, n);
   	    status = false;
	}
}