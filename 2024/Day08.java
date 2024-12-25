import java.util.ArrayList;
import java.util.Arrays;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day08 extends BaseSolution { 
	

	public Day08() {
		super(8);
	}
	

	
	@Override
	protected void solveDay() {
		
		char[][] input = Parser.parseMatrix(super.inputfileLines); // \r needs to be filtered out to get the correct matrix size!!!
		long antinodesCount = 0;
		long p2 = 0;
		boolean[][] antinodes = new boolean[input.length][input[0].length];
		boolean[][] antinodes2 = new boolean[input.length][input[0].length];
		
		ArrayList<Character> usedChars = new ArrayList<Character>();
		for(int i = 0; i < input.length; i++) {
			for(int j = 0; j < input[0].length;j++) {
				char c = input[i][j];
				if((!usedChars.contains(c)) && (c != '.')) {
					usedChars.add(c);
				}
				//System.out.print(c);
			}
			//System.out.println();
		}	
		
		for(char c : usedChars) {
			ArrayList<Point> allAntennnas = new ArrayList<Point>();
			for(int i = 0; i < input.length; i++) {
				for(int j = 0; j < input[0].length;j++) {
					if(input[i][j] == c) {
						allAntennnas.add(new Point(i, j, c));
					}
				}
			}
			//System.out.println("c="+c+": " + allAntennnas .size() + " antennas");
			for(int k = 0; k < allAntennnas.size(); k++) {
				for(int kk = k+1; kk < allAntennnas.size(); kk++) {
					Point a1 = allAntennnas.get(k);
					Point a2 = allAntennnas.get(kk);
					int diffi = a2.i - a1.i;
					int diffj = a2.j - a1.j;
					int a1i = a1.i - diffi;
					int a1j = a1.j - diffj;
					int a2i = a2.i + diffi;
					int a2j = a2.j + diffj;
					if(a1i >= 0 && a1i < input.length && a1j >= 0 && a1j < input[0].length) {
						if(antinodes[a1i][a1j] == false) {
							antinodes[a1i][a1j] = true;
							antinodesCount++;
						}
					}
					
					if(a2i >= 0 && a2i < input.length && a2j >= 0 && a2j < input[0].length) {
						if(antinodes[a2i][a2j] == false) {
							antinodes[a2i][a2j] = true;
							antinodesCount++;
						}
					}
					
				}
			}
			
			// p2:
			for(int k = 0; k < allAntennnas.size(); k++) {
				for(int kk = k+1; kk < allAntennnas.size(); kk++) {
					Point a1 = allAntennnas.get(k);
					Point a2 = allAntennnas.get(kk);
					int diffi = a2.i - a1.i;
					int diffj = a2.j - a1.j;
					
					int ii = a1.i;
					int jj = a1.j;
					int state = 0;
					while(true) {
						if(Parser.isInBounds(input, ii, jj)) {
							if(antinodes2[ii][jj] == false) {
								antinodes2[ii][jj] = true;
								p2++;
							}
						}else {
							if(state == 0) {
								ii = a1.i;
								jj = a1.j;
								state = 1;
								diffi = -diffi;
								diffj = -diffj;
							}else {
								break;
							}
						}
						ii -= diffi;
						jj -= diffj;
						
					}
				}
			}
		}
		
		
		part1(antinodesCount);
		part2(p2);
	}
	
	
	public static class Point{
		int i;
		int j;
		char c;
		public Point(int i, int j, char c) {
			this.i = i;
			this.j=j;
			this.c = c;
		}
	}

}
