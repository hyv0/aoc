import java.util.ArrayList;
import java.util.Comparator;
import java.util.Iterator;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day25 extends BaseSolution {
	
	

	public Day25() {
		super(25);
	}
	

	
	@Override
	protected void solveDay() {
		long p1 = 0;
		
		int lineIndex = 0;
		int matrixHeight = 7;
		ArrayList<Input> inputs = new ArrayList<Day25.Input>();
		while(lineIndex < super.inputfileLines.length) {
			if(super.inputfileLines[lineIndex].isBlank()) {
				lineIndex++;
			}else {
				char[][] input = Parser.parseMatrix(super.inputfileLines, lineIndex,matrixHeight);
				inputs.add(new Input(input));
				lineIndex += matrixHeight;
			}
		}
		ArrayList<Input> keys = new ArrayList<Day25.Input>();
		ArrayList<Input> locks = new ArrayList<Day25.Input>();
		for(Input p : inputs) {
			if(p.isKey) {
				keys.add(p);
			}else {
				locks.add(p);
			}
		}
		/*
		keys.sort(new Comparator<Input>() {
			public int compare(Day25.Input o1, Day25.Input o2) {
				return o1.compareTo(o2);
			}
		});
		locks.sort(new Comparator<Input>() {
			public int compare(Day25.Input o1, Day25.Input o2) {
				return o2.compareTo(o1);
			}
		});
		System.out.println(keys.size() + " keys for " + locks.size() + " locks");
		
		for(int i = 0; i < keys.size(); i++) {
			System.out.println(i+"  key: " + keys.get(i) + "    lock: " + locks.get(i));
		}
		*/
		
		// let's just bruteforce part1. 250 *250 is not worth implementing a smart algorithm
		for(Input key : keys) {
			for(Input lock : locks) {
				if(key.fitsIn(lock)) {
					p1++;
				}
			}
			
		}
		
		
		
		
		part1(p1);
		part2(0);
	}
	
	public static class Input{
		char[][] matrix;
		boolean isKey;
		int[] columns;
		
		public Input(char[][] matrix) {
			this.matrix = matrix;
			boolean lock = true;
			for(int i = 0; i < matrix[0].length; i++) {
				if(matrix[0][i] != '#') {
					lock = false;
					break;
				}
			}
			isKey = !lock;
			columns = new int[this.matrix[0].length];
			for(int j = 0; j < matrix[0].length; j++) {
				int counter = 0;
				for(int i = 1; i < matrix.length-1; i++) {
					char c = matrix[i][j];
					if(c == '#') {
						counter++;
					}
				}
				columns[j] = counter;
				
			}
		}
		
		public int compareTo(Input p) {
			for(int i= 0; i < columns.length; i++) {
				int a = columns[i];
				int b = p.columns[i];
				if(a<b) {
					return -1;
				}
				if(a>b) {
					return 1;
				}
			}
			return 0;
		}
		
		public boolean fitsIn(Input p) {
			for(int i= 0; i < columns.length; i++) {
				int a = columns[i];
				int b = p.columns[i];
				int sum = a+b;
				if(sum > 5) {
					return false;
				}
			}
			return true;
		}
		
		public String toString() {
			String g = "";
			for(int c : columns) {
				g+= "," + c;
			}
			return "[" + g.substring(1) + "]";
		}
	}

}
