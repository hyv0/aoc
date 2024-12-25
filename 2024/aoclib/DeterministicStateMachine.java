package aoclib;

import java.util.ArrayList;

/***
 * -1 is reserved for internal use for states, transitions, inputs
 */
public class DeterministicStateMachine<EndStateResult> {
	
	private int stateCount;
	private int[][] transitions;
	private int maxInputChar;
	private int start;
	private Object[] end;
	
	private int currentState;
	
	
	public DeterministicStateMachine(int maxState, int maxInputChar) {
		this.stateCount = maxState + 1;
		this.maxInputChar = maxInputChar;
		transitions = new int[stateCount][maxInputChar+1];
		end = new Object[stateCount];
		for(int i = stateCount-1; i >= 0; i--) {
			for(int j = maxInputChar; j >= 0; j--) {
				transitions[i][j] = -1;
			}
		}
		start = -1;
		currentState = -1;
		
	}
	
	public int getStateCount() {
		return stateCount;
	}
	
	public void setStart(int startState) {
		this.start = startState;
		this.currentState = start;
	}

	public void addEnd(int endStateNr, EndStateResult linkedResult) {
		end[endStateNr] = linkedResult;
	}
	
	public boolean isEnd(int stateNr) {
		return end[stateNr] != null;
	}
	
	public void addTransition(int stateFrom, int inputChar, int stateTo) {
		transitions[stateFrom][inputChar] = stateTo;
	}
	
	public boolean isTransitionRegistered(int stateFrom, int inputChar) {
		return transitions[stateFrom][inputChar] != -1;
	}
	
	public int getRegisteredTransition(int stateFrom, int inputChar) {
		return transitions[stateFrom][inputChar];
	}
	
	public void setCurrentState(int currentState) {
		this.currentState = currentState;
	}
	
	public void accept(int inputChar) {
		if(currentState == -1) return;
		int nextState = transitions[currentState][inputChar];
		currentState = nextState;
	}
	
	public EndStateResult isEndStateReached() {
		if(currentState == -1) return null;
		@SuppressWarnings("unchecked")
		EndStateResult result = (EndStateResult) end[currentState];
		if(result != null) {
			return result;
		}else {
			return null;
		}
	}
	
	public int getState() {
		return currentState;
	}
	
	public void print(boolean printTreeLike) {
		System.out.println("========= PRINTING DEA " + toString() +"with " + stateCount + "states =========");
		if(printTreeLike) {
			System.out.printf("%3d", start);
			printHelper("  ", start, new boolean[stateCount]);
		}else {
		
			for(int state = 0; state < stateCount; state++) {
				if(end[state] == null) {
					System.out.printf("   %3d  ----+\n", state);
				}else {
	
					System.out.printf("  (%3d) ----+\n", state);
				}
				for(int c = 0; c < maxInputChar; c++) {
					if(transitions[state][c] != -1) {
						System.out.printf("            +----[%c]----> %d\n",(char)c, transitions[state][c] );
					}
				}
	
				System.out.println();
			}
		}
	}
	
	private void printHelper(String prefix, int state, boolean[] printedStates) {
		int count = 0;
		int singleC = 0;
		if(printedStates[state] == true) {
			System.out.println();
			return;
		}
		printedStates[state] = true;
		for(int c =  0; c < maxInputChar; c++) {
			if(transitions[state][c] != -1) {
				count++;
				singleC = c;
			}
		}
		if(count == 0) {
			// nothing to do
		}else if(count == 1) {
			System.out.printf(" --[%c]--> %3d", (char)singleC, transitions[state][singleC]);
			printHelper(prefix + "             ", transitions[state][singleC], printedStates);
		}else {
			System.out.println();
			for(int c =  0; c < maxInputChar; c++) {
				if(transitions[state][c] != -1) {
					System.out.printf(prefix + "+--[%c]--> %3d", (char)c, transitions[state][c]);
					printHelper(prefix + "|           ", transitions[state][c], printedStates);
					
				}
			}
		}
		
	}
	
/*
	public DeterministicStateMachine<EndStateResult> minimize() {
1 Eingabe: ein deterministischer, endlicher Automat A = (Q;sigma,delta,q0; F);
2 Markiere alle Paare von Zustanden {p; q}, von denen genau einer ein akzeptierender Zustand ist;
3 solange es noch ein Paar {p; q} von Zustanden und ein Zeichen a element sigma gibt,
4 sodass {delta(p; a); delta(q; a)} bereits markiert ist tue
5 markiere {p; q};
6 Ende
7 Ausgabe: Die nicht-markierten Zustandspaare bilden Paare aquivalenter Zustande.
	}
*/
	
	
}
