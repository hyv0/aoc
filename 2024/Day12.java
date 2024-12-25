import java.util.ArrayList;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day12 extends BaseSolution {
	

	int[][] DIRS = new int[][] {{-1,0},{0,1},{1,0},{0,-1}};

	public Day12() {
		super(12);
	}
	

	
	@Override
	protected void solveDay() {
		long p1 = 0;
		long p2 = 0;
		
		char[][] input = Parser.parseMatrix(super.inputfileLines);
		
		
		for(char[][] area : extractAreas(input)) {
			int areaSize = 0;
			int perimeter = 0;
			int perimeter2 = 0;
			char type = 0;
			int[][] countedEdges = new int[input.length][input[0].length];
			for(int i = 0; i < input.length; i++) {
				for(int j = 0; j < input[0].length;j++) {
					char c = area[i][j];
					if(c != 0) {
						type = c;
						areaSize++;
						for(int k = 0; k < DIRS.length; k++) {
							int ii = i + DIRS[k][0];
							int jj = j + DIRS[k][1];
							boolean border = false;
							if(Parser.isInBounds(input, ii, jj)) {
								if(area[ii][jj] == 0) {
									border = true;
								}
							}else {
								border = true;
							}
							if(border) {
								perimeter++;
								// for part 2: Don't count every perimeter. When counting a perimeter in a direction, don't count the same direction for the neighboring tiles
								if((countedEdges[i][j] & (1<<k)) == 0) {
									perimeter2++;
								}
								for(int n = 1; n < 4; n+=2) {
									int iii = i + DIRS[(k+n)%DIRS.length][0];
									int jjj = j + DIRS[(k+n)%DIRS.length][1];
									if(Parser.isInBounds(input, iii, jjj)) {
										countedEdges[iii][jjj] |= 1<<k;
									}
								}
								
							}
						}
					}
				}
			}
			p1 += perimeter * areaSize;
			p2 += perimeter2 * areaSize;
			//Caster.printMatrix(area);
			//System.out.println(type + "  size=" + areaSize + "  perimeter1=" + perimeter + "  perimeter2=" + perimeter2);
		}
		
		
		
		
		
		part1(p1);
		part2(p2);
	}
	
	private ArrayList<char[][]> extractAreas(char[][] input) {
		ArrayList<char[][]> ret = new ArrayList<char[][]>();
		boolean[][] done = new boolean[input.length][input[0].length];
		while(true) {
			int starti = 0;
			int startj = 0;
			ArrayList<Point> trunk = new ArrayList<Day12.Point>();
			boolean found = false;
			for(starti = 0; starti < input.length; starti++) {
				for(startj = 0; startj < input[0].length; startj++) {
					if(!done[starti][startj]) {
						found = true;
						break;
					}
				}
				if(found) break;
			}
			if(!found) {
				break;
			}
			trunk.add(new Point(starti,startj));
			done[starti][startj] = true;
			char type = input[starti][startj];
			char[][] area = new char[input.length][input[0].length];
			area[starti][startj] = type;
			
			while(trunk.size() != 0) {
				Point p = trunk.removeFirst();
				for(int[] dir : DIRS) {
					int ii = p.i + dir[0];
					int jj = p.j + dir[1];
					if(Parser.isInBounds(input, ii, jj)) {
						if(done[ii][jj] == false && input[ii][jj] == type) {
							trunk.add(new Point(ii,jj));
							done[ii][jj] = true;
							area[ii][jj] = type;
						}
					}
				}
			}
			ret.add(area);
		}
		return ret;
	}

	public static class Point{
		int i;
		int j;
		public Point(int i, int j) {
			this.i = i;
			this.j=j;
		}
	}

}
