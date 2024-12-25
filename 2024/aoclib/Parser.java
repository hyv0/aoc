package aoclib;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.ListIterator;
import java.util.Map.Entry;
import java.util.function.BiConsumer;
import java.util.function.Consumer;
import java.util.function.Supplier;
import java.util.stream.Stream;

import javax.management.RuntimeErrorException;

public class Parser {
	
	public static final char[] ALPHABET_LOWERCASE = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
	public static final char[] ALPHABET_UPPERCASE = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
	public static final char[] ALPHABET_NUMBERS = {'0','1','2','3','4','5','6','7','8','9'};
	public static final char[] ALPHABET_SIGNS = {'.',',',';','\'','"','`','#','-','+','*','/','~','$','?','!','=','%','°','^','&','|','(',')'};
	public static final char[] ALPHABET_BLANKS = {' ','	'};
	
	
	public static final char[] ALPHABET_ALL = Caster.combine(ALPHABET_LOWERCASE,ALPHABET_UPPERCASE,ALPHABET_NUMBERS,ALPHABET_SIGNS,ALPHABET_BLANKS);
	
	
	public static class Match {
		public final String txt;
		public final int indexStart;
		public final int indexEnd;
		public final int length;
		public Match(String txt, int indexStart, int length) {
			this.txt = txt;
			this.indexStart = indexStart;
			this.length = length;
			this.indexEnd = indexStart + length;
		}
	}
	
	public static interface Sink {
		/**
		 * length = indexEnd - indexStart
		 * @param indexStart
		 * @param indexEnd exclusive.
		 */
		public void match(int indexStart, int indexEnd);
	}

	public static class DefaultSink implements Sink{
		final String input;
		final ArrayList<Match> matches;
		
		DefaultSink(String input) {
			this.input = input;
			this.matches = new ArrayList<Parser.Match>();
		}
		
		public void match(int indexStart, int indexEnd){
			this.matches.add(new Match(input.substring(indexStart, indexEnd),indexStart,indexEnd - indexStart));
		}
		
		public String[] toArr() {
			String[] ret = new String[matches.size()];
			int i = 0;
			for(Match m : matches) {
				ret[i++] = m.txt;
			}
			return ret;
		}
		public List<Match> get() {
			return Collections.unmodifiableList(matches);
		}
		public int length() {
			return matches.size();
		}
	}
	
	/**
	 * Parses a input string and returns all numbers. E.g. "abcd123as56" returns ["123","56"]. 
	 * 
	 * @param input the input string
	 * @param negative are negative numbers allowed
	 * @param maxDigitsPerNumbers specifies how long each number is allowed to be. Default = -1 = no limit. e.g. maxDigitsPerNumbers = 2 -> "abcd123as56" returns ["12", "3","56"]
	 * @param sink. may be null. Then a new Sink is created
	 * @return Array of Strings. not null
	 */
	public static Sink extractNumbers(String input, boolean negative, int maxDigitsPerNumbers, Sink sink) {
		if(sink == null) {
			sink = new DefaultSink(input);
		}
		if(input == null || input.length() == 0) {
			return sink;
		}
		int start = -1;
		for(int i = 0; i < input.length(); i++) {
			char c = input.charAt(i);
			boolean split = false;
			if(start != -1 && maxDigitsPerNumbers > 0) {
				if(i - start == (maxDigitsPerNumbers + (input.charAt(start) == '-' ? 1 : 0))) { // let a '-' not count to max number length
					split = true;
				}
			}
			if(c < '0' || c> '9') {
				if(start != -1) {
					split = true;
				}
			}
			
			if(split) {
				if(input.charAt(start) == '-' && (i - start == 1)) {
					// only "-" matched!
				}else {
					sink.match(start, i);
				}
				start = -1;
			}
			if(((c >= '0' && c<= '9') || (c == '-' && negative)) && start == -1) {
				start = i;
			}
		}
		if(start != -1) {
			sink.match(start, input.length());
		}
		return sink;
	}
	


	public static Sink extractWords(String input, String[] searchForTheseWords, Sink sink) {
		
		
		if(sink == null) {
			sink = new DefaultSink(input);
		}
		class Result{
			String string;
			int index;
			public Result(String string, int index) {
				this.string = string;
				this.index = index;
			}
		}
		int stateMax = 0;
		int maxChar = 0;
		boolean[] chars = new boolean[255];
		for(String g : searchForTheseWords) {
			stateMax += g.length();
			for(char c : g.toCharArray()) {
				if(c > maxChar) {
					maxChar = c;
				}
				chars[c] = true;
			}
		}
		for(char c : input.toCharArray()) {
			if(c > maxChar)
				maxChar = c;
			chars[c] = true;
		}
		NonDeterministicStateMachine<Result> nea = new NonDeterministicStateMachine<Result>(stateMax+2, maxChar+1);
		nea.addStart(0);
		for(int i = 0; i < chars.length; i++) {
			if(chars[i]) {
				nea.addTransition(0, i, 0);
			}
		}
		int lastState = 0;
		int index = 0;
		for(String g : searchForTheseWords) {
			boolean first = true;
			for(char c : g.toCharArray()) {
				nea.addTransition(first ? 0 : lastState, c, ++lastState);
				first = false;
			}
			nea.addEnd(lastState, new Result(g, index++));	
		}
		
		//nea.print();
		
		long starttime = System.nanoTime();	
		DeterministicStateMachine<List<Result>> dea = nea.makeDeterministic();
		//dea.print(false);
		long endtime = System.nanoTime();
//		System.out.println("statecount = " + dea.getStateCount());
		for(int i = 0; i < input.length(); i++) {
			char c = input.charAt(i);
			int prev = dea.getState();
			dea.accept(c);
//			System.out.println(prev + " -["+(char)c+"]-> " + dea.getState() + " endcount="+dea.isEndStateReached());
			List<Result> end = dea.isEndStateReached();
			if(end != null) {
				for(Result r : end ) {
					sink.match(i+1 - r.string.length(), i+1);
//					System.out.println(" end for word " + r.index + ": " +r.string);
				}
				
			}
		}

		
//		System.out.println(" >>> " + (endtime-starttime)/1000000 + "ms");
		

		return sink;
	}
	
	public static char[][] parseMatrix(String[] lines) {
		return parseMatrix(lines, 0, lines.length);
	}
	
	public static char[][] parseMatrix(String[] lines, int startLineIndex, int length) {
		if(lines == null || lines.length == 0 || length == 0) {
			return new char[0][0];
		}
		int linel = lines[startLineIndex].length();
		if(lines[startLineIndex].charAt(linel-1) == '\n') linel--;
		char[][] ret = new char[length][linel];
		int li = 0;
		for(int i = startLineIndex; i < startLineIndex+length; i++) {
			int ri = 0;
			for(char c : lines[i].toCharArray()) {
				if(ri < linel) {
					ret[li][ri++] = c;
				}
			}
			li++;
		}
		return ret;
	}
	public static boolean isInBounds(char[][] input, int i, int j) {
		return i >= 0 && i < input.length && j >= 0 && j < input[0].length;
	}
	
	
	
	
	
	
	
}
