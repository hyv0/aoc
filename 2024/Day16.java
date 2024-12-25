import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.TreeSet;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day16 extends BaseSolution {
	
	

	public Day16() {
		super(16);
	}
	

	int[][] DIRS = new int[][] {{0,1},{1,0},{0,-1},{-1,0}}; 
	
	char[][] input;
	int startI = 0;
	int startJ = 0;
	int endI = 0;
	int endJ = 0;
	int startDir = 0;
	
	
	
	@Override
	protected void solveDay() {
		long p1 = 0;
		long p2 = 0;
		
		
		input = Parser.parseMatrix(super.inputfileLines);
		for(int i = 0; i < input.length; i++) {
			for(int j = 0; j < input[0].length;j++) {
				char c = input[i][j];
				if(c == 'E') {
					endI = i;
					endJ = j;
				}
				if(c=='S') {
					startI = i;
					startJ = j;
				}
			}
		}
		
		// ================
		//       A*
		//=================
		
		PriorityQueue<SearchItem> trunk = new PriorityQueue<Day16.SearchItem>(new Comparator<SearchItem>() {

			@Override
			public int compare(Day16.SearchItem o1, Day16.SearchItem o2) {
				int a = Math.abs(o1.i - endI) + Math.abs(o1.j - endJ) + o1.previousScore;
				int b = Math.abs(o2.i - endI) + Math.abs(o2.j - endJ) + o2.previousScore;
				return a-b;
			}
		});
		SearchItem startItem = new SearchItem(0, startI, startJ, startDir);
		trunk.add(startItem);
		
		boolean[][][] markedTiles = new boolean[input.length][input[0].length][4];
		while(!trunk.isEmpty()) {
			SearchItem item = trunk.remove();
			if(item.i == endI && item.j == endJ) {
				p1 = item.previousScore;
				break;
			}
			
			
			if(markedTiles[item.i][item.j][item.dir]) {
				continue; // no need to calculate a tile twice. the first time the score was better anyway.
			}else {
				markedTiles[item.i][item.j][item.dir] = true;
			}
			

			trunk.add(new SearchItem(item.previousScore+1000, item.i, item.j, (item.dir + 1) %4,item.prev)); // linking to item.prev is not really necessary. i thought it might help for the part2 but it doesn't
			trunk.add(new SearchItem(item.previousScore+1000, item.i, item.j, (item.dir + 3) %4,item.prev));
			int newI = item.i + DIRS[item.dir][0];
			int newJ = item.j + DIRS[item.dir][1];
			if(Parser.isInBounds(input, newI, newJ) && input[newI][newJ] != '#') {
				trunk.add(new SearchItem(item.previousScore+1, newI, newJ, item.dir, item));
			}
			
		}
		
		part1(p1);
		


		// ================
		//    dijkstra
		//=================
		int[][] lowestScores = new int[input.length][input[0].length];
		SearchItem[][] lowestScoreItems = new SearchItem[input.length][input[0].length];
		for(int ii = 0; ii < input.length; ii++) {
			for(int jj = 0; jj < input[0].length; jj++ ) {
				lowestScores[ii][jj] = Integer.MAX_VALUE;
			}
		}
		trunk.clear();
		trunk.add(new SearchItem(0, startI, startJ, startDir));
		lowestScores[startI][startJ] = 0;
		
		while(!trunk.isEmpty()) {
			SearchItem item = trunk.remove();
			if(item.i == endI && item.j == endJ) break;
			for(int k = 0; k < DIRS.length; k++) {
				int newI = item.i + DIRS[k][0];
				int newJ = item.j + DIRS[k][1];
				if(Parser.isInBounds(input, newI, newJ) && input[newI][newJ] != '#') {
					int newScore = calcNewScore(item, k);
					if(lowestScores[newI][newJ] > newScore) {
						lowestScores[newI][newJ] = newScore;
						SearchItem previousItem = lowestScoreItems[newI][newJ];
						SearchItem newItem = new SearchItem(newScore, newI, newJ, k,item);
						if(previousItem != null) {
							trunk.remove(previousItem);
						}
						trunk.add(newItem);
						lowestScoreItems[newI][newJ] = newItem;
					}
				}
			}
		}		
		
		
		boolean[][] markedTilesForP2 = new boolean[input.length][input[0].length];
		ArrayList<SearchItem> markedTilesTrunk = new ArrayList<SearchItem>();
		markedTilesTrunk.add(lowestScoreItems[endI][endJ]);
		while(!markedTilesTrunk.isEmpty()) {
			SearchItem item = markedTilesTrunk.removeFirst();
			
			while(item != null) {
				if(markedTilesForP2[item.i][item.j]) {
					break;
				}
				markedTilesForP2[item.i][item.j] = true;

				// check if there is an alternative path:
				for(int k = 0; k < DIRS.length; k++) {
					int newI = item.i + DIRS[k][0];
					int newJ = item.j + DIRS[k][1];
					if(Parser.isInBounds(input, newI, newJ)) {
						SearchItem kItem = lowestScoreItems[newI][newJ];
						if(kItem != null && kItem != item.prev) {
							//System.out.println("checking " + newI + "  " + newJ);
							boolean alternateWay = false;
							if(calcNewScore(kItem, (k+2)%4) == item.previousScore) {
								alternateWay = true;
							}else {
								// maybe kItem can be a predecessor although it is 1001 lower
								int nextDir = (k + 2) %4;
								int nextI = item.i + DIRS[nextDir][0];
								int nextJ = item.j + DIRS[nextDir][1];
								if(Parser.isInBounds(input, nextI, nextJ) && markedTilesForP2[nextI][nextJ]) {
									if(kItem.dir == nextDir && kItem.previousScore+2 == lowestScores[nextI][nextJ]){
										alternateWay = true;
									}
								}
								
							}

							if(alternateWay){
								markedTilesTrunk.add(kItem);
							}
						}
					}
					
				}
				item = item.prev;
			}
		}
		for(int ii = 0; ii < input.length; ii++) {
			for(int jj = 0; jj < input[0].length; jj++ ) {
				if(markedTilesForP2[ii][jj]) {
					p2++;
				}
			}
		}
		part2(p2);
		
		
		
		boolean printGoodLooking = false;
		if(printGoodLooking) {
			for(int ii = 0; ii < input.length; ii++) {
				for(int jj = 0; jj < input[0].length; jj++ ) {
					if(input[ii][jj] == '#') {
						System.out.print("########");
					}
					else if(lowestScores[ii][jj] == Integer.MAX_VALUE) {
						System.out.print("        ");
						
					}else if(markedTilesForP2[ii][jj]) {
						System.out.format("(%5d) ", lowestScores[ii][jj]);
					}else {
						System.out.format(" %5d  ", lowestScores[ii][jj]);
						
					}
					
				}
				System.out.println();
			}
		}
		/*
		System.out.println();
		for(int ii = 0; ii < input.length; ii++) {
			for(int jj = 0; jj < input[0].length; jj++ ) {
				if(input[ii][jj] == '#') {
					System.out.print("#");
				}else if(markedTilesForP2[ii][jj]) {
					System.out.print("X");
				}else {
					System.out.print(" ");
				}
			}
			System.out.println();
		}
		*/
		
		
	}

	private int calcNewScore(SearchItem item, int dir) {
		int newScore;
		int diffDir = item.dir - dir;
		if(diffDir == 0) {
			newScore = item.previousScore + 1;
		}else if(Math.abs(diffDir)==2) {
			newScore = item.previousScore + 2001;
		}else {
			newScore = item.previousScore + 1001;
		}
		return newScore;
	}
	

	
	

	public static class SearchItem{
		int previousScore = 0;
		int i;
		int j;
		int dir;
		
		SearchItem prev = null;
		
		public SearchItem(int score, int i, int j , int dir,  SearchItem prev) {
			this(score,i,j,dir);
			this.prev = prev;
		}
		
		
		public SearchItem(int score, int i, int j , int dir) {
			this.dir = dir;
			this.i = i;
			this.j = j;
			this.previousScore = score;
		}
	}

}
