package aoclib;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Arrays;

public abstract class BaseSolution {

	protected final int day;
	protected final String inputfilePath;
	protected String inputfileText;
	protected String[] inputfileLines;
	private String part1;
	private String part2;
	private boolean part1Set;
	private boolean part2Set;
	
	public BaseSolution(int day) {
		this.day = day;
		this.inputfilePath = "./input/" + day +".txt";
		part1Set = false;
		part2Set = false;
	}
	
	public final void solve() {
		System.out.println("Solving day " + day + "...");
		long start = System.nanoTime();
		loadFile();
		long end = System.nanoTime();
		System.out.println("Reading file " + inputfilePath + " done in "+(end-start)/1000000+"ms. size=" + inputfileText.length() + "  lines=" + inputfileLines.length);

		start = System.nanoTime();
		solveDay();
		end = System.nanoTime();
		System.out.println("==========================");
		long secondsNeededToSolve = (end-start)/1000000000;
		long millisecondsNeededToSolve = ((end-start) - secondsNeededToSolve*1000000000)/1000000;
		System.out.println("SOLVED IN in "+secondsNeededToSolve+" sec " + millisecondsNeededToSolve + " ms");
		String p1Txt = "n.a.";
		String p2Txt = "n.a.";
		if(part1Set) {
			p1Txt = part1 +"";
		}
		if(part2Set) {
			p2Txt = part2 +"";
		}
		System.out.println("PART 1: " + p1Txt);
		System.out.println("PART 2: " + p2Txt);
	}
	
	private void loadFile() {
		try {
			inputfileText = new String(Files.readAllBytes(Paths.get(inputfilePath)));
		} catch (IOException e) {
			throw new RuntimeException("INPUT FILE " + inputfilePath + " can not be read (" + e +  ")" );
		}
		inputfileLines = inputfileText.split("\n");
		int i;
		for(i = inputfileLines.length-1; i >= 0; i--) {
			String line = inputfileLines[i];
			if(line.length() == 0) {
				continue;
			}
			if(line.isBlank()) {
				continue;
			}
			if(line.isEmpty()) {
				continue;
			}
			break;
		}
		inputfileLines = Arrays.copyOf(inputfileLines, i+1);
		for(i = 0; i < inputfileLines.length; i++) {
			String g = inputfileLines[i];
			if(g.endsWith("\r")) {
				inputfileLines[i] = g.substring(0, g.length()-1);
			}
		}
	}
	
	protected void hi() {
		System.out.println(" <DEBUG> hi() -> " + Thread.currentThread().getStackTrace()[2]);
	}
	

	protected void hi(String text) {
		System.out.println(" <"+text+"> hi() -> " + Thread.currentThread().getStackTrace()[2]);
	}
	
	protected void print(String g) {
		System.out.println(g);
	}
	
	protected void part1(long value) {
		part1Set = true;
		part1 = value+"";
	}
	
	protected void part2(long value) {
		part2Set = true;
		part2 = value+"";
	}
	
	protected void part1(String value) {
		part1Set = true;
		part1 = value;
	}
	
	protected void part2(String value) {
		part2Set = true;
		part2 = value;
	}
	
	abstract protected void solveDay();
}
