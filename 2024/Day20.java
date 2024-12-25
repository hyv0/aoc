import java.util.ArrayList;
import java.util.Comparator;
import java.util.PriorityQueue;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day20 extends BaseSolution {
	
	

	public Day20() {
		super(20);
	}

	static final int[][] DIRS = new int[][] {{0,1},{1,0},{0,-1},{-1,0}}; 
	
	int iStart;
	int iEnd;
	int jStart;
	int jEnd;
	
	@Override
	protected void solveDay() {
		long p1 = 0;
		long p2 = 0;
		
		
		
		char[][] input = Parser.parseMatrix(super.inputfileLines);
		for(int i = 0; i < input.length; i++) {
			for(int j = 0; j < input[0].length;j++) {
				char c = input[i][j];
				if(c == 'S') {
					input[i][j] = '.';
					iStart = i;
					jStart = j;
				}
				if(c == 'E') {
					input[i][j] = '.';
					iEnd = i;
					jEnd = j;
				}
			}
		}
		
/*
		int[][] allShortestPaths = new int[input.length][input[0].length];
		for(int i = 0; i < input.length; i++) {
			for(int j = 0; j < input[0].length;j++) {
				//int[][] m = findWay(input, i, j, iEnd, jEnd);
				//allShortestPaths[i][j] = m[iEnd][jEnd];
			}
		}
*/	
		int[][] allShortestPathsFromEnd = findWay(input, iEnd, jEnd, iStart, jStart);
		int[][] lowestCostsFromStart = findWay(input, iStart, jStart, iEnd, jEnd);
		
		int shortestPath = allShortestPathsFromEnd[iStart][jStart];
		
		for(int i = 0; i < input.length; i++) {
			for(int j = 0; j < input[0].length;j++) {
				if(input[i][j] != '#') {
					for(int dir = 0; dir < 4; dir++) {
						if(input[i + DIRS[dir][0]][j + DIRS[dir][1]] == '#') {
							int pathLength = calcPathWithRemovedWall(input,  i,j ,dir ,lowestCostsFromStart,allShortestPathsFromEnd);
							int diff = shortestPath - pathLength;
							if(diff >= 100) {
								p1++;
							}
						}
					}
				}
			}
		}
		part1(p1);
		
		
		//long[] test = new long[shortestPath+2];
		int[][] factors = new int[][] {{1,1},{-1,-1},{1,-1},{-1,1}};
		for(int i = 0; i < input.length; i++) {
			for(int j = 0; j < input[0].length;j++) {
				if(input[i][j] != '#') {

					
					for(int iE = 0; iE < 21; iE++) {
						for(int jE = 0; jE < (21 - iE); jE++) {
							for(int k = 0; k <( (iE==0 || jE==0)?2:4); k++) {
								int iCalc = i + iE * factors[k][0];
								int jCalc = j + jE * factors[k][1];
								if(Parser.isInBounds(input, iCalc, jCalc)) {
									// wow. this nested loops are still running within milliseconds. 
									if(input[iCalc][jCalc] != '#') {
										//System.out.println(i+" "+j+"   ->   " + iCalc + " " + jCalc);

										int scoreCheatStart = lowestCostsFromStart[i][j];
										int pathLengthCheatEnd = allShortestPathsFromEnd[iCalc][jCalc];
										int newPathLength = scoreCheatStart + pathLengthCheatEnd + iE + jE;
										int diff = shortestPath - newPathLength;
										//if(diff > 49) {
										//	test[diff]++;
										//}
										if(diff >= 100) {
											p2++;
										}
									}
									
								}
							}
						}
					}
					
				}
			}
		}
		//Caster.print(test, "");
		
		
		part2(p2);
		
		
		
	}
	



	private static int[][] findWay(char[][] input, int iS, int jS, int iE, int jE) {
		PriorityQueue<SearchItem> trunk = new PriorityQueue<SearchItem>(new Comparator<SearchItem>() {

			@Override
			public int compare(SearchItem o1, SearchItem o2) {
				int a = Math.abs(o1.i - iE) + Math.abs(o1.j - jE) + o1.previousScore;
				int b = Math.abs(o2.i - iE) + Math.abs(o2.j - jE) + o2.previousScore;
				return a-b;
			}
		});
		int[][] lowestScores = new int[input.length][input[0].length];
		SearchItem[][] lowestScoreItems = new SearchItem[input.length][input[0].length];
		for(int ii = 0; ii < input.length; ii++) {
			for(int jj = 0; jj < input[0].length; jj++ ) {
				lowestScores[ii][jj] = Integer.MAX_VALUE;
			}
		}
		SearchItem startItem = new SearchItem(0, iS, jS, null);
		trunk.add(startItem);
		lowestScores[iS][jS] = 0;
		
		while(!trunk.isEmpty()) {
			SearchItem item = trunk.remove();
			for(int k = 0; k < DIRS.length; k++) {
				int newI = item.i + DIRS[k][0];
				int newJ = item.j + DIRS[k][1];
				if(Parser.isInBounds(input, newI, newJ) && input[newI][newJ] != '#') {
					int newScore = item.previousScore + 1;
					if(lowestScores[newI][newJ] > newScore) {
						lowestScores[newI][newJ] = newScore;
						SearchItem previousItem = lowestScoreItems[newI][newJ];
						SearchItem newItem = new SearchItem(newScore, newI, newJ,item);
						if(previousItem != null) {
							trunk.remove(previousItem);
						}
						trunk.add(newItem);
						lowestScoreItems[newI][newJ] = newItem;
					}
				}
			}
		}		
		return lowestScores;
	}

	private int calcPathWithRemovedWall(char[][] input, int iS, int jS, int dir, int[][] lowestScores, int[][] allShortestPaths) {
		int jumpWalls = 0;
		int icheatEnd = -1;
		int jcheatEnd = -1;
		for(int step = 1; step < 4; step++) {
			int newI = iS + DIRS[dir][0] * step;
			int newJ = jS + DIRS[dir][1] * step;
			
			if(Parser.isInBounds(input, newI, newJ)) {
				char c = input[newI][newJ];
				if(c == '#') {
					jumpWalls++;
				}else {
					icheatEnd = newI;
					jcheatEnd = newJ;
					break;
				}
			}else {
				//if(step ==1 || step == 2) {
				return lowestScores[iEnd][jEnd];
				//}
			}
		}
		switch(jumpWalls) {
		case 3:
			return lowestScores[iEnd][jEnd];
		case 2:
		case 1:
			break;
		case 0: throw new RuntimeException("a wall was expected");
		}
		
		int scoreCheatStart = lowestScores[iS][jS];
		int pathLengthCheatEnd = allShortestPaths[icheatEnd][jcheatEnd];
		
		
		
		return scoreCheatStart + pathLengthCheatEnd + jumpWalls + 1;
		
	}






	public static class SearchItem{
		int previousScore = 0;
		int i;
		int j;
		
		SearchItem prev = null;
		
		public SearchItem(int score, int i, int j ,  SearchItem prev) {
			this.i = i;
			this.j = j;
			this.prev = prev;
			this.previousScore = score;
		}
	}


}
