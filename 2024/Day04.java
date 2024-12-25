import aoclib.BaseSolution;
import aoclib.Parser;

public class Day04 extends BaseSolution {
	
	

	public Day04() {
		super(4);
	}
	
	/**
	 * 
	 * @param input
	 * @param l the length of the returned word
	 * @param i may be -1 -> return word is ""
	 * @param j may be -1 -> return word is ""
	 * @param direction 0=right, 1=right down, 2=down, 3=left down, 4=left, 5=left up, 6=up, 7=right up
	 * @return
	 */
	private String extractWord(char[][] input, int l, int i, int j, int direction) {
		if(i >= input.length || i < 0 || j >= input[0].length || j< 0) { return "";}
		String word = input[i][j] + "";
		for(int k = 0; k < l-1; k++) {
			switch(direction) {
			case 0: j++;break;
			case 1: j++; i++; break;
			case 2: i++; break;
			case 3: i++; j--; break;
			case 4: j--; break;
			case 5: i--; j--; break;
			case 6: i--; break;
			case 7: i--; j++; break;
			}

			if(i >= input.length || i < 0 || j >= input[0].length || j< 0) {
				return word;
			}
			word += input[i][j];
			
		}
		return word;
	}
	
	@Override
	protected void solveDay() {
		long p1 = 0;
		long p2 = 0;
		char[][] input = Parser.parseMatrix(super.inputfileLines);
		for(int i = 0; i < input.length; i++) {
			for(int j = 0; j < input[0].length;j++) {
				char c = input[i][j];
				for(int dir = 0; dir < 8; dir++) {
					String word = extractWord(input, 4, i, j, dir);
					if(word.equals("XMAS")) {
						p1++;
					}
				}
				
				if(extractWord(input, 3, i-1, j-1, 1).equals("MAS") || extractWord(input, 3, i+1, j+1, 5).equals("MAS")) {
					if(extractWord(input, 3, i+1, j-1, 7).equals("MAS") || extractWord(input, 3, i-1, j+1, 3).equals("MAS")) {
						p2++;
					}
					
				}
				
				
			}
		}
		part1(p1);
		part2(p2);
	}

}
