/*
 * started at 1.12.2024: 08:10
 */
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day01 extends BaseSolution {
	
	public Day01() {
		super(1);
	}

	@Override
	protected void solveDay() {
		long sum = 0;
		
		ArrayList<Integer> firstList = new ArrayList<Integer>();
		ArrayList<Integer> secondList = new ArrayList<Integer>();
		
		
		for(String line : super.inputfileLines) {
			DefaultSink sink = (DefaultSink) Parser.extractNumbers(line,false,5,null);
			long[] numbers = Caster.castLong(sink.toArr());
			firstList.add((int) numbers[0]);
			secondList.add((int) numbers[1]);
			
			
		}
		firstList.sort(new Comparator<Integer>() {
			public int compare(Integer o1, Integer o2) {
				return o1 - o2;
			}
		});
		secondList.sort(new Comparator<Integer>() {
			public int compare(Integer o1, Integer o2) {
				return o1 - o2;
			}
		});
		int size = firstList.size();
		for(int i = 0; i < size; i++) {
			int diff = firstList.get(i) - secondList.get(i);
			if(diff < 0) diff = -diff;
			sum += diff;
		}
		part1(sum);
		
		int lastValue = -1;
		int count = 0;
		HashMap<Integer, Integer> counts = new HashMap<Integer, Integer>();
		for(int i = 0; i < size; i++) {
			int val = secondList.get(i);
			if(val == lastValue) {
				count++;
			}else {
				counts.put(lastValue, count);
				lastValue = val;
				count = 1;
			}
		}
		long p2 = 0;
		for(int value : firstList) {
			if(counts.containsKey(value)) {
				p2 += value * counts.get(value);
			}
		}
		part2(p2);
		
		
	}
	
}
