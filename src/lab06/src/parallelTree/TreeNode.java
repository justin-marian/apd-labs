package parallelTree;

public class TreeNode {
	int name;
	TreeNode left = null;
	TreeNode right = null;
	private final Object treeLock = new Object();

	public TreeNode(int name) {
		this.name = name;
	}

	public void addChild(TreeNode child) {
		synchronized (treeLock) {
			if (left == null) {
				left = child;
			} else if (right == null) {
				right = child;
			}
		}
	}

	public TreeNode getNode(int name) {
		synchronized (treeLock) {
			if (this.name == name) {
				return this;
			}
			TreeNode foundNode = null;
			if (left != null) {
				foundNode = left.getNode(name);
			}
			if (foundNode == null && right != null) {
				foundNode = right.getNode(name);
			}
			return foundNode;
		}
	}
}
