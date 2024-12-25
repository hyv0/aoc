import java.util.ArrayList;
import java.util.Comparator;
import java.util.concurrent.ArrayBlockingQueue;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day02 extends BaseSolution {

	public Day02() {
		super(2);
	}

	@Override
	protected void solveDay() {
		long p1=0;
		long p2 = 0;
		
		for(String line : super.inputfileLines) {
			DefaultSink sink = (DefaultSink) Parser.extractNumbers(line,false,5,null);
			long[] numbers = Caster.castLong(sink.toArr());
			
			if(isok(numbers, 3)) {
				p1++;
			}
			
			if(isok(numbers, 3)) {
				p2++;	
			}else {
				long[] numbers2 = new long[numbers.length-1];
				for(int i = 0; i < numbers.length; i++) {
					int k = 0;
					for(int j = 0; j < numbers.length; j++) {
						if(i == j) {
							continue;
						}
						numbers2[k] = numbers[j];
						k++;
						
						
					}
					if(isok(numbers2,3)) {
						p2++;
						break;
					}
				}
			}

			
			
			
		}
		
		part1(p1);
		part2(p2);
	}

	private boolean isok(long[] numbers, int savedistance) {
		long prev = numbers[0];
		boolean ascending = numbers[1] > numbers[0];
		boolean ok = true;
		for(int i = 1; i < numbers.length; i++) {
			long diff = numbers[i] - prev;
			if(ascending) {
				if(diff < 1) {
					ok = false;
				}
				if(diff > savedistance) {
					ok = false;
				}
				
			}else {
				if(diff > -1) {
					ok = false;
				}
				if(diff < -savedistance) {
					ok = false;
				}
				
			}
			prev = numbers[i];
		}
		return ok;
	}

}
