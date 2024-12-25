import java.util.ArrayList;
import java.util.HashMap;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day11 extends BaseSolution {
	
	

	public Day11() {
		super(11);
	}
	

	
	@Override
	protected void solveDay() {
		long p2 = 0;
		
		long[] input = null;
		
		for(String line : super.inputfileLines) {
			DefaultSink sink = (DefaultSink) Parser.extractNumbers(line,false,-1,null);
			input = Caster.castLong(sink.toArr());
		}
		long[] p1List = input;
		for(int i = 0; i < 25; i++) {
			p1List = applyRule(p1List);
			//Caster.print(p1List, "");
			//System.out.println("=========="+i+"=========");
		}
		
		/*
		HashMap<Long, ArrayList<Long>> lookUpTable = new HashMap<Long, ArrayList<Long>>();
		...
		*/
		
		ArrayList<NumberOccurance> listp2 = new ArrayList<NumberOccurance>();
		for(long l : input) {
			listp2.add(new NumberOccurance(l, 1));
			
		}
		for(int i = 0; i < 75; i++) {
			listp2 = applyRule2(listp2);
			// now filter out duplicates. no need to to the same calculation twice. Use an HashMap to keep track over the occurrences.
			HashMap<Long, NumberOccurance> hm = new HashMap<Long, NumberOccurance>();
			ArrayList<NumberOccurance> reduced = new ArrayList<NumberOccurance>();
			for(NumberOccurance occ : listp2) {
				if(hm.containsKey(occ.nr)) {
					hm.get(occ.nr).times += occ.times;
				}else {
					hm.put(occ.nr, occ);
					reduced.add(occ);
				}
			}
			/*
			System.out.println("part 2 step " + i + " : reduced " + inp2.size() + " -> " + reduced.size());
			// After step 92 the reduced size stays at 3811. Why? This is interesting...
			for(NumberOccurance n : inp2) {
				if(n.nr < 0 || n.nr > Long.MAX_VALUE/2025) {
					System.out.println("ERROR");
					return;
				}
			}
			*/
				
			listp2 = reduced;
		}
		for(NumberOccurance occ : listp2) {
			p2 += occ.times;
		}
		
		part1(p1List.length);
		part2(p2);
	}
	
	private int digitCount(long l) {
		int ret = 0;
		while(l != 0) {
			l /= 10;
			ret ++;
		}
		return ret;
	}
	
	private long[] applyRule(long[] input) {
		ArrayList<Long> ret = new ArrayList<Long>();
		for(long l : input) {
			if(l == 0) {
				ret.add(1L);
				continue;
			}
			int digitCount = digitCount(l);
			if((digitCount & 0x1) == 0) {
				int half = digitCount >> 1;
				long divideBy = 1;
				for(int i = 0; i < half; i++) {
					divideBy *= 10;
				}
				long firstNr = l / divideBy;
				long secondNr = l - firstNr*divideBy;
				ret.add(firstNr);
				ret.add(secondNr);
			}else {
				ret.add(2024*l);
			}
		}
		long[] r = new long[ret.size()];
		int i = 0;
		for(Long l : ret) {
			r[i++] = l;
		}
		return r;
	}
	
	private ArrayList<NumberOccurance> applyRule2(ArrayList<NumberOccurance> input) {
		ArrayList<NumberOccurance> ret = new ArrayList<NumberOccurance>();
		for(NumberOccurance occ : input) {
			if(occ.nr == 0) {
				ret.add(new NumberOccurance(1, occ.times));
				continue;
			}
			int digitCount = digitCount(occ.nr);
			if((digitCount & 0x1) == 0) {
				int half = digitCount >> 1;
				long divideBy = 1;
				for(int i = 0; i < half; i++) {
					divideBy *= 10;
				}
				long firstNr = occ.nr / divideBy;
				long secondNr = occ.nr - firstNr*divideBy;
				ret.add(new NumberOccurance(firstNr, occ.times));
				ret.add(new NumberOccurance(secondNr, occ.times));
			}else {
				ret.add(new NumberOccurance(2024*occ.nr, occ.times));
			}
		}
		return ret;
	}
	
	private static class NumberOccurance{
		private long nr;
		private long times;
		private NumberOccurance(long nr, long times) {
			this.nr = nr;
			this.times = times;
		}
	}

}
