package list;

public class LockDList extends DList{

	@Override
	protected DListNode newNode(Object item, DListNode prev, DListNode next) {
    	LockDListNode pNode = new LockDListNode(item, prev, next);
    	lockNode(pNode);
    	return pNode;
  	}

	public LockDList(){
		super();
	}

	public void lockNode(DListNode node){
		((LockDListNode)node).status = true;
	}

	@Override
	public void remove(DListNode node) {
    	if(((LockDListNode)node).status) 
    		return;
    	else
    		super.remove(node);
  	}
}