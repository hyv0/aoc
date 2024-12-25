import java.util.Comparator;
import java.util.PriorityQueue;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day18 extends BaseSolution {
	
	

	public Day18() {
		super(18);
	}
	

	int[][] DIRS = new int[][] {{0,1},{1,0},{0,-1},{-1,0}}; 
	
	
	@Override
	protected void solveDay() {
		long p1 = 0;
		
		
		char[][] input = new char[71][71];
		
		int byteNr = 0;
		for(String line : super.inputfileLines) {
			DefaultSink sink = (DefaultSink) Parser.extractNumbers(line,false,2,null);
			long[] numbers = Caster.castLong(sink.toArr());
			input[(int) numbers[1]][(int) numbers[0]] = '#';
			byteNr++;
			if(byteNr == 1024) {
				break;
			}
			
		}
		
		for(int i = 0; i < input.length; i++) {
			for(int j = 0; j < input[0].length;j++) {
				char c = input[i][j];
				if(c != '#') {
					input[i][j]=' ';
				}
			}
		}
		//Caster.printMatrix(input);;
		
		p1 = findPath(input);
		
		part1(p1);
		
		/*
		while(true) {
			String line = super.inputfileLines[byteNr++];
			DefaultSink sink = (DefaultSink) Parser.extractNumbers(line,false,2,null);
			long[] numbers = Caster.castLong(sink.toArr());
			input[(int) numbers[1]][(int) numbers[0]] = '#';
			
			if(findPath(input) == -1) {
				part2(line);
				break;
			}
			
			
		}
		*/
		
		int minimum = 1024-1;
		int maximum = super.inputfileLines.length;
		
		while(true) {
			//System.out.println("minimum="+ minimum + " maximum=" + maximum + " mid=" +((maximum + minimum) / 2) );
			if(minimum >= maximum - 1) {
				part2(super.inputfileLines[maximum]);
				break;
			}
			int mid = (maximum + minimum) / 2;
			input = new char[71][71];
			byteNr = 0;
			for(String line : super.inputfileLines) {
				DefaultSink sink = (DefaultSink) Parser.extractNumbers(line,false,2,null);
				long[] numbers = Caster.castLong(sink.toArr());
				input[(int) numbers[1]][(int) numbers[0]] = '#';
				if(byteNr == mid) {
					break;
				}
				byteNr++;
			}
			if(findPath(input) == -1) {
				maximum = mid;
			}else {
				minimum = mid;
			}
			
			
		}
		



		
		
	}



	private long findPath(char[][] input) {
		final int endI = 70;
		final int endJ = 70;

		PriorityQueue<SearchItem> trunk = new PriorityQueue<SearchItem>(new Comparator<SearchItem>() {

			@Override
			public int compare(SearchItem o1, SearchItem o2) {
				int a = Math.abs(o1.i - endI) + Math.abs(o1.j - endJ) + o1.previousScore;
				int b = Math.abs(o2.i - endI) + Math.abs(o2.j - endJ) + o2.previousScore;
				return a-b;
			}
		});
		SearchItem startItem = new SearchItem(0, 0, 0, 0);
		trunk.add(startItem);
		
		boolean[][] markedTiles = new boolean[71][71];
		
		while(!trunk.isEmpty()) {
			SearchItem item = trunk.remove();
			if(item.i == endI && item.j == endJ) {
				return item.previousScore;
			}
			
			
			if(markedTiles[item.i][item.j]) {
				continue; // no need to calculate a tile twice. the first time the score was better anyway.
			}else {
				markedTiles[item.i][item.j] = true;
			}
			

			for(int dir = 0; dir < 4; dir++) {
				int newI = item.i + DIRS[dir][0];
				int newJ = item.j + DIRS[dir][1];
				if(Parser.isInBounds(input, newI, newJ)) {
					if(input[newI][newJ] != '#') {
						trunk.add(new SearchItem(item.previousScore+1, newI, newJ, dir , item));
					}
				}
			}
		}
		return -1;
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
