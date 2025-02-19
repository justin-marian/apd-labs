package parallelTree;

import java.util.*;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Main {
	static int N_ITERATIONS = 100;

	public static void main(String[] args) {
		System.out.println("Parallel tree problem");

		for (int j = 0; j < N_ITERATIONS; j++) {
			TreeNode tree = new TreeNode(1);
			Map<Integer, List<Integer>> childParentMap = new HashMap<>();

			Thread[] threads = new Thread[2];
			threads[0] = new Thread(new ReadTreePart("treePart1.txt", childParentMap));
			threads[1] = new Thread(new ReadTreePart("treePart2.txt", childParentMap));

			for (int i = 0; i < 2; i++) {
				threads[i].start();
			}

			for (int i = 0; i < 2; i++) {
				try {
					threads[i].join();
				} catch (InterruptedException e) {
					Logger.getLogger(Main.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
				}
			}

			// Build the tree
			for (Map.Entry<Integer, List<Integer>> entry : childParentMap.entrySet()) {
				TreeNode parentNode = tree.getNode(entry.getKey());
				if (parentNode != null) {
					for (int child : entry.getValue()) {
						parentNode.addChild(new TreeNode(child));
					}
				}
			}

			// Now all nodes are added, start the verification thread
			Thread verifyThread = new Thread(new VerifyTree(tree));
			verifyThread.start();
			try {
				verifyThread.join(); // Wait for the verification thread to finish
			} catch (InterruptedException e) {
				Logger.getLogger(Main.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
			}
		}
	}
}
