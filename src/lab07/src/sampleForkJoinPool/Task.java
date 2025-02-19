package sampleForkJoinPool;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.RecursiveTask;

public class Task extends RecursiveTask<Void> {
	private final String path;

	public Task(String path) {
		this.path = path;
	}

	@Override
	protected Void compute() {
		File file = new File(path);
		if (file.isFile()) {
			System.out.println(file.getPath());
		} else if (file.isDirectory()) {
			File[] files = file.listFiles();
			List<Task> tasks = new ArrayList<>();

			if (files != null) {
				for (File f : files) {
					Task t = new Task(f.getPath());
					tasks.add(t);
					t.fork();
				}
			}

			for (Task task : tasks) {
				task.join();
			}
		}
		return null;
	}
}
