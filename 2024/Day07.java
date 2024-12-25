/*
 * started at 07.12.2024: 07:20
 */
import java.util.ArrayList;
import java.util.TreeSet;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day07 extends BaseSolution {
	
	

	public Day07() {
		super(7);
	}
	

	
	@Override
	protected void solveDay() {
		long p1 = 0;
		long p2 = 0;
		for(String line : super.inputfileLines) {
			DefaultSink sink = (DefaultSink) Parser.extractNumbers(line,false,20,null);
			long[] numbers = Caster.castLong(sink.toArr());
			if(check(numbers, false)) {
				p1 += numbers[0];
				p2 += numbers[0];
			}else {
				if(check(numbers,true)) {
					p2 += numbers[0];
					//System.out.println("p2 successfully done for " + numbers[0]);
				}else {
					//System.out.println("p2 done for " + numbers[0]);
				}
				
			}
			
		}
		part1(p1);
		part2(p2);
	}
	
	/*
	private void calcConcats(ArrayList<long[]> resultList, long[] numbers, int start) {
		if(start >= numbers.length) {
			resultList.add(numbers);
			return;
		}
		
		calcConcats(resultList, numbers, start+2);
		for(int i = start; i < numbers.length-1; i ++) {
			long[] numbersNew = new long[numbers.length-1];
			int kk = 0;
			for(int k = 0; k < numbers.length; k++) {
				numbersNew[kk] = numbers[k];
				if(k != i) {
					kk++;
				}
			}
			int multFactor = 1;
			while(multFactor <= numbers[i+1]) {
				multFactor *= 10;
			}
			numbersNew[i] = numbers[i] * (multFactor) + numbers[i+1];
			
			
			// recursion:
			calcConcats(resultList, numbersNew, i);
			
		}
	}
	*/
	
	private boolean checkHelper2(long[] numbers, int index, long interim) {
		if(index >= numbers.length) {
			boolean ret = numbers[0] == interim;
			return ret;
		}
		if(interim > numbers[0] ) return false; // early abort
		long maxNr = 1;
		for(int k = index; k < numbers.length; k++) {
			long m = numbers[k];
			while(m != 0) {
				m /= 10;
				maxNr *= 10;
			}
		}
		if(maxNr  * interim < numbers[0]) {
			return false; // early abort
		}
		

		int multFactor = 1;
		while(multFactor <= interim) {
			multFactor *= 10;
		}
		multFactor /= 10;
		
		return  checkHelper2(numbers, index+1, interim + numbers[index]) || checkHelper2(numbers, index+1, interim * numbers[index]) || checkHelper2(numbers, index+1, interim * multFactor + numbers[index]);
	}
	
	private TreeSet<Long> checkHelper3(long[] numbers, int index) {
		if(index == 1) {
			TreeSet<Long> ret = new TreeSet<Long>();
			ret.add(numbers[index]);
			return ret;
		}
		TreeSet<Long> recursionResult = checkHelper3(numbers, index - 1);
		TreeSet<Long> myResult = new TreeSet<Long>();
		long myNr = numbers[index];
		for(Long l : recursionResult) {
			long addThis = myNr + l;
			if(addThis <= numbers[0]) myResult.add(addThis);
			addThis = myNr * l;
			if(addThis <= numbers[0]) myResult.add(addThis);

			//System.out.println(" test " + recursionResult.size() + "  " + l);
			int multFactor = 1;
			long copy = myNr;
			while(copy != 0) {
				multFactor *= 10;
				copy /= 10;
			}
			//System.out.println(" test " + recursionResult.size() + "  " + multFactor);
			
			addThis = l * multFactor + myNr;
			if(addThis <= numbers[0]) myResult.add(addThis);
		}
		return myResult;
	}
	
	private boolean checkHelper(long[] numbers, int index, long interim) {
		if(index >= numbers.length) {
			boolean ret = numbers[0] == interim;
			return ret;
		}
		if(interim > numbers[0] ) return false; // early abort
		return  checkHelper(numbers, index+1, interim + numbers[index]) || checkHelper(numbers, index+1, interim * numbers[index]);
	}
	
	private boolean check(long[] numbers, boolean p2) {
		if(p2) {
			for(long l : checkHelper3(numbers, numbers.length - 1)){
				if(l == numbers[0]) return true;
			}
			return false;
			
		}else {
			return checkHelper(numbers, 2, numbers[1]);
			
		}
	}

}
