import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day15 extends BaseSolution {
	

	int[][] DIRS = new int[][] {{0,1},{1,0},{0,-1},{-1,0}}; 

	public Day15() {
		super(15);
	}
	

	
	@Override
	protected void solveDay() {
		
		
		
		String[] inputMap = null;
		String commandos = "";
		for(int i = 0;  i < super.inputfileLines.length; i++) {
			if(super.inputfileLines[i].length() == 0 && inputMap == null) {
				inputMap = new String[i];
				for(int k = 0; k < i; k++) {
					inputMap[k] = super.inputfileLines[k];
				}
			}else {
				if(inputMap != null) {
					commandos += super.inputfileLines[i];
				}
			}
		}
		
		int robotI = 0;
		int robotJ = 0;
		char[][] matrix = Parser.parseMatrix(inputMap);
		for(int i = 0; i < matrix.length; i++) {
			for(int j = 0; j < matrix[0].length;j++) {
				char c = matrix[i][j];
				if(c == '@') {
					robotI = i;
					robotJ = j;
					matrix[i][j] = '.';
					break;
				}
			}
		}
		//Caster.printMatrix(matrix);
		//System.out.println(commandos);
		
		Robot r = new Robot(robotI, robotJ);
		for(char c : commandos.toCharArray()) {
			int dir = 0;
			switch(c) {
			case '>' : dir = 0; break;
			case 'v': dir = 1; break;
			case '<': dir = 2; break;
			case '^': dir = 3; break;
			}
			r = move(matrix, r.i,r.j,dir );

			//System.out.println("after c=" + c + "  i=" + r.i+ " j="+r.j);
			//Caster.printMatrix(matrix);
		}
		part1(calcScore(matrix));
		
		
		// ============================================================
		
		
		matrix = Parser.parseMatrix(inputMap);
		char[][] matrixB = new char[matrix.length][matrix[0].length*2];
		for(int i = 0; i < matrix.length; i++) {
			for(int j = 0; j < matrix[0].length;j++) {
				if(matrix[i][j] == '#') {
					matrixB[i][j*2] = '#';
					matrixB[i][j*2+1] = '#';
				}else if(matrix[i][j] == 'O') {
					matrixB[i][j*2] = '[';
					matrixB[i][j*2+1] = ']';
				}else {
					matrixB[i][j*2] = '.';
					matrixB[i][j*2+1] = '.';
					
				}
			}
		}
		robotJ *= 2;
		//Caster.printMatrix(matrixB); // check if the conversion is correct
		
		for(char c : commandos.toCharArray()) {
			int dir = 0;
			switch(c) {
			case '>': dir = 0; break;
			case 'v': dir = 1; break;
			case '<': dir = 2; break;
			case '^': dir = 3; break;
			}
			boolean[][] moveMatrix = new boolean[matrixB.length][matrixB[0].length];
			if(moveHelper(matrixB, robotI+ DIRS[dir][0], robotJ + DIRS[dir][1], dir, moveMatrix)) {
				robotI += DIRS[dir][0];
				robotJ += DIRS[dir][1];
				// now move the boxes by using the moveMatrix (This matrix marks all tiles that need to be moved)
				for(int i = ((dir == 1)? (matrixB.length-1):0); i < matrixB.length &&i >= 0;i+=((dir == 1) ? -1: 1) ) {
					for(int j = ((dir == 0)? (matrixB[0].length-1):0); j < matrixB[0].length &&j >= 0;j+=((dir == 0) ? -1: 1) ) {
						if(moveMatrix[i][j]) {
							matrixB[i+DIRS[dir][0]][j+DIRS[dir][1]] = matrixB[i][j]; 
							matrixB[i][j] = '.';
						}
					}
				}
			}
			//System.out.println("after c=" + c + "  i=" + robotI+ " j="+robotJ);
			//Caster.printMatrix(matrixB);
			
		}
			
		
		part2(calcScore(matrixB));
	}



	private long calcScore(char[][] matrix) {
		long ret = 0;
		for(int i = 0; i < matrix.length; i++) {
			for(int j = 0; j < matrix[0].length;j++) {
				if(matrix[i][j] == 'O' || matrix[i][j] == '[') {
					ret += i * 100 + j;
				}
			}
		}
		return ret;
	}

	private Day15.Robot move(char[][] matrix, int robotI, int robotJ, int direction) {
		int[] dir = DIRS[direction];
		
		int i = robotI;
		int j = robotJ;
		boolean moveAllowed = true;
		do{
			i += dir[0];
			j += dir[1];
		}while(matrix[i][j] == 'O');
		if(matrix[i][j] == '#') {
			moveAllowed = false;
		}
		
		if(moveAllowed) {
			if(matrix[robotI + dir[0]][robotJ+dir[1]] == 'O') {
				matrix[i][j] = 'O';
				matrix[robotI + dir[0]][robotJ+dir[1]] = '.';
			}
			return new Robot(robotI + dir[0], robotJ+dir[1]);
		}else {
			return new Robot(robotI, robotJ);
		}
		
	}
	
	/**
	 * checks if i and j are free for something (box or robot) moving here
	 */
	private boolean moveHelper(char[][] matrix, int i, int j, int direction, boolean[][] toBeMoved) {
		if(toBeMoved[i][j]) {
			// has already been processed
			return true;
		}
		if(matrix[i][j] == '.') {
			return true;
		}
		if(matrix[i][j] == '#') {
			return false;
		}		
		
		if(direction == 0|| direction == 2) {
			for(int k = 0; k < 2; k++) {
				if(matrix[i][j] == '[' || matrix[i][j] == ']') {
					toBeMoved[i][j] = true;
				}else {
					throw new RuntimeException("my bad " + matrix[i][j]);
				}
				i += DIRS[direction][0];
				j += DIRS[direction][1];
			}
			return moveHelper(matrix, i, j, direction, toBeMoved);
		}else {
			int jSecondPart = 0;
			if(matrix[i][j] == '[') {
				jSecondPart = 1;
			}else if(matrix[i][j] == ']') {
				jSecondPart = -1;
			}else {
				throw new RuntimeException("my bad ");
			}
			toBeMoved[i][j] = true;
			toBeMoved[i][j + jSecondPart] = true;
			return moveHelper(matrix, i + DIRS[direction][0], j + DIRS[direction][1], direction, toBeMoved)
					&& moveHelper(matrix, i + DIRS[direction][0], j + DIRS[direction][1] + jSecondPart, direction, toBeMoved);
		}
	}

	private static class Robot{
		int i = 0;
		int j = 0;
		public Robot(int i, int j) {
			this.i = i;
			this.j = j;
		
		}
	}
	
}
