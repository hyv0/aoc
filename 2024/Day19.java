import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.DeterministicStateMachine;
import aoclib.NonDeterministicStateMachine;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day19 extends BaseSolution {
	
	

	public Day19() {
		super(19);
	}
	

	
	@Override
	protected void solveDay() {
		long p1 = 0;
		long p2 = 0;
		
		ArrayList<Towel> towels= new ArrayList<Towel>();
		for(String pattern : super.inputfileLines[0].split(", ")) {
			Towel t = new Towel();
			for(char c : pattern.toCharArray()) {
				t.add(COLOR.fromChar(c));
			}
			
			if(t.colors.size() > 0) {
				towels.add(t);
			}
		}
		//System.out.println("total towel count: " + towels.size());
		
		NonDeterministicStateMachine<Towel> neaP1= new NonDeterministicStateMachine<Towel>(towels.size()*7, 6);
		NonDeterministicStateMachine<Towel> neaP2= new NonDeterministicStateMachine<Towel>(towels.size()*7, 6);
		int nextFreeState = 1;
		int nextFreeStateP2 = 1;
		for(Towel t : towels) {
			int prevState = 0;
			for(int i = 0; i < t.colors.size()-1; i++) {
				neaP1.addTransition(prevState, t.colors.get(i).ordinal(), nextFreeState);
				prevState = nextFreeState++;
			}
			neaP1.addTransition(prevState, t.colors.get(t.colors.size()-1).ordinal(), 0);
			
			prevState = 0;
			for(COLOR c : t.colors) {
				neaP2.addTransition(prevState, c.ordinal(), nextFreeStateP2);
				prevState = nextFreeStateP2++;
			}
			neaP2.addEnd(nextFreeStateP2-1, t);
			
		}
		neaP1.addStart(0);
		neaP2.addStart(0);
		DeterministicStateMachine<List<Towel>> dea = neaP1.makeDeterministic();
		DeterministicStateMachine<List<Towel>> dea2 = neaP2.makeDeterministic();

		for(int i = 2; i < super.inputfileLines.length; i++) {
			dea.setCurrentState(0);
			String line = super.inputfileLines[i];
			for(char c : line.toCharArray()) {
				COLOR value = COLOR.fromChar(c);
				dea.accept(value.ordinal());
			}
			if(dea.getState() != -1) {
				p1++;
			}
			p2 += doPart2(dea2,line);
		}
		part1(p1);
		
		
		part2(p2);
	}
	
	
	private static enum COLOR{
		WHITE,BLUE, BLACK, RED,GREEN;
		
		public static COLOR fromChar(char c) {
			switch(c) {
			case 'w': return COLOR.WHITE;
			case 'u': return COLOR.BLUE;
			case 'b': return COLOR.BLACK;
			case 'r': return COLOR.RED;
			case 'g': return COLOR.GREEN;
			default: return null;
			}
		}
	}
	private static class Towel {
		
		private ArrayList<COLOR> colors;
		public Towel() {
			colors = new ArrayList<COLOR>();
		}
		public void add(COLOR c) {
			colors.add(c);
		}
	}
	

	HashMap<String, Long> cache = new HashMap<String, Long>();// is this called dynamic programming when i simply use introduce this cache?
	private long doPart2(DeterministicStateMachine<List<Towel>> deaForSingleTowel, String line) {
		long possibilities = recursion(deaForSingleTowel, line);
		
		//System.out.println("Cach size has grown to " + cache.size()); // The cache size is actually not that big.. Ca. 10k at the end.
		return possibilities;
	}
	
	private long recursion(DeterministicStateMachine<List<Towel>> deaForSingleTowel, String str) {
		if(cache.containsKey(str)) {
			return cache.get(str);
		}
		
		long count = 0;
		deaForSingleTowel.setCurrentState(0);
		for(int i = 0; i < str.length(); i++) {
			deaForSingleTowel.accept(COLOR.fromChar(str.charAt(i)).ordinal());
			if(deaForSingleTowel.getState() == -1) {
				break;
			}
			List<Towel> endStates = deaForSingleTowel.isEndStateReached();
			if(endStates != null) {
				if(endStates.size() > 1) {
					throw new RuntimeException("There are towels which are equal. Count this as one?");
				}
				if(i != str.length()-1) {
					int state = deaForSingleTowel.getState();
					count += recursion(deaForSingleTowel, str.substring(i+1));
					deaForSingleTowel.setCurrentState(state);
				}else {
					count += 1;
				}
			}
		}
		cache.put(str, count);
		return count;
		
		
	}
	
}
