import java.util.ArrayDeque;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day10 extends BaseSolution {
	
	

	public Day10() {
		super(10);
	}
	

	
	@Override
	protected void solveDay() {
		long p1 = 0;
		long p2 = 0;
		
		
		char[][] input = Parser.parseMatrix(super.inputfileLines);
		for(int i = 0; i < input.length; i++) {
			for(int j = 0; j < input[0].length;j++) {
				char c = input[i][j];
				if(c == '0') {
					p1 += calcTrailheadsp1(input, i, j);
					p2 += calcTrailheadsp2(input, i, j);
				}
			}
		}
		

		
		
		part1(p1);
		part2(p2);
	}
	
	
	private int calcTrailheadsp1(char[][] input, int starti, int startj) {
		boolean[][] checked = new boolean[input.length][input[0].length];
		ArrayDeque<Point> backlog = new ArrayDeque<Day10.Point>();
		int ends = 0;
		backlog.add(new Point(starti, startj, input[starti][startj]));
		int[][] directions = new int[][] {{0,1},{0,-1},{-1,0},{1,0}};
		while(!backlog.isEmpty()) {
			Point p = backlog.removeFirst();
			if(checked[p.i][p.j]) {
				continue;
			}
			checked[p.i][p.j] = true;
			if(p.c == '9') {
				ends ++;
				continue;
			}
			for(int[] dir : directions) {
				int i = p.i + dir[0];
				int j = p.j + dir[1];

				if(Parser.isInBounds(input, i, j)) {
					char c = input[i][j];
					if(c-1 == p.c) {
						backlog.add(new Point(i, j, c));
					}
				}
			}
		}
		return ends;
 	}
	

	/**
	 * This works unexpectedly. So lets stick with this solution (It's basically the same as part1, but without the checked array)
	 */
	private int calcTrailheadsp2(char[][] input, int starti, int startj) {
		ArrayDeque<Point> backlog = new ArrayDeque<Day10.Point>();
		int ends = 0;
		backlog.add(new Point(starti, startj, input[starti][startj]));
		int[][] directions = new int[][] {{0,1},{0,-1},{-1,0},{1,0}};
		while(!backlog.isEmpty()) {
			Point p = backlog.removeFirst();
			if(p.c == '9') {
				ends ++;
				continue;
			}
			for(int[] dir : directions) {
				int i = p.i + dir[0];
				int j = p.j + dir[1];

				if(Parser.isInBounds(input, i, j)) {
					char c = input[i][j];
					if(c-1 == p.c) {
						backlog.add(new Point(i, j, c));
					}
				}
			}
		}
		return ends;
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
