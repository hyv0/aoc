import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day06 extends BaseSolution {
	
	

	public Day06() {
		super(6);
	}
	

	
	@Override
	protected void solveDay() {
		long p2 = 0;
		char[][] input = Parser.parseMatrix(super.inputfileLines);
		int x = 0;
		int y = 0;
		int visitedCount = 0;
		for(int i = 0; i < input.length; i++) {
			for(int j = 0; j < input[0].length;j++) {
				if(input[i][j] == '^') {
					x = j;
					y = i;
				}
			}
		}
		visitedCount = checkPath(input, x, y, 0);
		
		// not sure if there is an elegant solution. so let's do the trivial approach
		for(int i = 0; i < input.length; i++) {
			for(int j = 0; j < input[0].length;j++) {
				if(input[i][j] != '.') {
					continue;
				}
				input[i][j] = '#';
				if(checkPath(input, x, y, 0) == -1) {
					p2++;
				}
				input[i][j] = '.';
			}
		}
		

		part1(visitedCount);
		part2(p2);
	}



	private int checkPath(char[][] input, int x, int y, int direction) {
		int[][] visited = new int[input.length][input[0].length];
		boolean mapLeft=false;
		int visitedCount = 0;
		while(!mapLeft) {
			if(visited[y][x] == 0) {
				visitedCount++;
			}
			if(visited[y][x] == (1 << direction)) {
				return -1;// loop detected
			}
			visited[y][x] = (1 << direction);
			Point next;
			do {
				next = move(x,y,direction);
				if(next.x < 0 || next.y < 0 || next.x >= input[0].length || next.y >= input.length) {
					mapLeft = true;
					break;
				}
				if(input[next.y][next.x] == '#') {
					direction++;
					if(direction == 4) {
						direction = 0;
					}
				}else {
					break;
				}
			}while(true);
			
			
			x = next.x;
			y = next.y;
			
		}
		return visitedCount;
	}

	static class Point{
		int x;
		int y;
		public Point(int x, int y ) {
			this.x = x;
			this.y = y;
		}
		
	}
	static Point move(int x, int y, int direction) {
		switch(direction) {
		case 0: return new Point(x,y-1);
		case 1: return new Point(x+1,y);
		case 2: return new Point(x,y+1);
		case 3: return new Point(x-1,y);
		}
		return null;
	}
	
	
	

}
