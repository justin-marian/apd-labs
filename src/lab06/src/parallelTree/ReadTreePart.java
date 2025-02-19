package parallelTree;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;

public class ReadTreePart implements Runnable {
	String fileName;
	final Map<Integer, List<Integer>> childParentMap;

	public ReadTreePart(String fileName, Map<Integer, List<Integer>> childParentMap) {
		this.fileName = fileName;
		this.childParentMap = childParentMap;
	}

	@Override
	public void run() {
		try {
			Scanner scanner = new Scanner(new File(fileName));
			synchronized (childParentMap) {
				while (scanner.hasNextInt()) {
					int child = scanner.nextInt();
					int parent = scanner.nextInt();
					childParentMap.computeIfAbsent(parent, k -> new ArrayList<>()).add(child);
				}
			}
		} catch (FileNotFoundException e) {
			Logger.getLogger(ReadTreePart.class.getName()).log(Level.SEVERE, "File not found", e);
		}
	}
}
