package parallelTree;

public class VerifyTree implements Runnable {
	TreeNode tree;

	public VerifyTree(TreeNode tree) {
		this.tree = tree;
	}

	public boolean isCorrect(TreeNode tree) {
		if (tree == null) {
			return true;
		}

		if (tree.name >= 64) {
			return true;
		}

		if (tree.left == null || tree.right == null) {
			return false;
		}

		boolean aux = ((tree.left.name + tree.right.name) == (tree.name * 4 + 1));
		return aux && isCorrect(tree.left) && isCorrect(tree.right);
	}

	@Override
	public void run() {
		if (isCorrect(tree))
			System.out.println("Correct");
		else
			System.out.println("Wrong");
	}
}