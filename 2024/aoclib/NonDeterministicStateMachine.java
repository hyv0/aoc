package aoclib;

import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.TreeSet;

public class NonDeterministicStateMachine<EndStateResult> {
	
	private int stateCount;
	private List<Integer>[][] transitions;
	private int maxInputChar;
	private boolean[] start;
	private Object[] end;
	
	
	
	public NonDeterministicStateMachine(int maxState, int maxInputChar) {
		this.stateCount = maxState + 1;
		this.maxInputChar = maxInputChar;
		transitions = new ArrayList[stateCount][maxInputChar+1];
		end = new Object[stateCount];
		start = new boolean[stateCount];
		for(int i = stateCount-1; i >= 0; i--) {
			for(int j = maxInputChar; j >= 0; j--) {
				transitions[i][j] = new ArrayList<Integer>();
			}
			start[i] = false;
		}
		
	}
	
	public void addStart(int startState) {
		this.start[startState] = true;
	}

	public void addEnd(int endState, EndStateResult linkedResult) {
		end[endState] = linkedResult;
	}
	
	public void addTransition(int stateFrom, int inputChar, int stateTo) {
		transitions[stateFrom][inputChar].add(stateTo);
	}
	
	public boolean isTransitionRegistered(int stateFrom, int inputChar) {
		return transitions[stateFrom][inputChar].size() > 0;
	}
	
	public void print() {
		for(int state = 0; state < stateCount; state++) {
			if(end[state] == null) {
				System.out.printf("   %3d  ----+\n", state);
			}else {

				System.out.printf("  (%3d) ----+\n", state);
			}
			for(int c = 0; c < maxInputChar; c++) {
				if(transitions[state][c].size() > 0) {
					String f = "";
					for(int n : transitions[state][c]) {
						f += ", "+n;
					}
					f = f.substring(2);
					System.out.printf("            +----[%c]----> " + f + "\n",(char)c);
				}
			}

			System.out.println();
		}
	}
	
	public DeterministicStateMachine<List<EndStateResult>> makeDeterministic() {
		
		LinkedList<Integer> states = new LinkedList<Integer>();
		
		//int requiredArrays = stateCount / 63;
		//if(requiredArrays * 63 < stateCount) {
		//	requiredArrays++;
		//}
		//long[] arrays = new long[requiredArrays];
		//long mask = 1;
		//int arrayIndex = 0;
		for(int i = 0; i < stateCount; i++) {
			if(start[i]) {
		//		arrays[arrayIndex] |= mask;
				states.add(i);
			}
		//	if(mask == 1<<62) {
		//		mask = 1;
		//		arrayIndex++;
		//	}else {
		//		mask = mask << 1;
		//	}
		}
		
		TreeSet<StateSuperposition> tree = new TreeSet<StateSuperposition>();
		ArrayList<StateSuperposition> trunk = new ArrayList<StateSuperposition>();
		
		int stateNr = 0;
		StateSuperposition startState = new StateSuperposition(states, maxInputChar, stateCount, stateNr++);
		tree.add(startState);
		trunk.add(startState);
		
		
		
		
		while(!trunk.isEmpty()) {
			StateSuperposition state = trunk.removeFirst();
			
			// calculate follow up states
			
			
			
			LinkedList<Integer> followStates = new LinkedList<Integer>();
			for(int input = maxInputChar; input >= 0; input--) {
//				String debug = "";
				for(int s : state.states) {
					for(int transition : transitions[s][input]) {
						followStates.add(transition);
//						debug += "[" + (char)input +"]:" + transition + "  ";
						
					}
				}
				
				
				if(followStates.size() != 0) {
					// add new followupsuperstate:
					StateSuperposition followSuperState = new StateSuperposition(followStates, maxInputChar, stateCount, stateNr);
					if(!tree.contains(followSuperState)) {
						tree.add(followSuperState);
						trunk.add(followSuperState);
						stateNr++;
					}else {
						followSuperState = tree.ceiling(followSuperState);
					}
					followStates = new LinkedList<Integer>();
					state.transitions[input] = followSuperState;
//					System.out.println("adding transition from superstate " + state.stateNr + " to " + followSuperState.stateNr + " char=" + (char)input + "    "+debug);
				}
				
			}
			
		}

		DeterministicStateMachine<List<EndStateResult>> dea = new DeterministicStateMachine<>(tree.size(), maxInputChar);
		dea.setStart(startState.stateNr);
		for(StateSuperposition superstate : tree) {
//			System.out.println(" superstate " + superstate.stateNr);
			ArrayList<EndStateResult> list = new ArrayList<>();
			for(int state : superstate.states) {
				@SuppressWarnings("unchecked")
				EndStateResult result = (EndStateResult) end[state];
				if(result != null) {
					list.add(result);
				}
//				System.out.println("       contains state: " + state);
			}
			if(list.size() > 0) {
				dea.addEnd(superstate.stateNr, list);
			}
			for(int inputC = 0; inputC < superstate.transitions.length; inputC++) {
				StateSuperposition ss = superstate.transitions[inputC];
				if(ss != null) {
					dea.addTransition(superstate.stateNr, inputC, ss.stateNr);
					//System.out.println("               -["+(char)inputC+"]-> " + ss.stateNr);
				}
			}
		}
		
		
		
		return dea;
	}
	
	
	private static class StateSuperposition implements Comparable<StateSuperposition>{
		
		
		private long[] markedStates; // fehler irgendwo hier drin
		
		private StateSuperposition[] transitions;
		private List<Integer> states;
		private int stateNr;
		
		
		public StateSuperposition( List<Integer> states, int maxInputChar, int stateCount, int stateNr) {
			this.states = states;
			this.transitions = new StateSuperposition[maxInputChar+1];
			this.stateNr = stateNr;
			
			markedStates= new long[(stateCount / 63) + 1];
			for(int i : states) {
				int arayIndex = i / 63;
				markedStates[arayIndex] |= (1L << (i - arayIndex * 63));
			}
		}

		@Override
		public int compareTo(StateSuperposition o) {
			for(int i = 0; i < markedStates.length; i++) {
				long a = markedStates[i];
				long b = o.markedStates[i];
				if(a < b) {
					return 1;
				}
				if(a>b) {
					return -1;
				}
			}
			return 0;
		}
		
		
		
		
	}
}
