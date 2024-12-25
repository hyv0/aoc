import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashSet;
import java.util.Random;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day17 extends BaseSolution {

	public Day17() {
		super(17);
	}
	
	
	@Override
	protected void solveDay() {
		
		long registerAInit = 0;
		long registerBInit = 0;
		long registerCInit = 0;
		long[] programInit = null;
		
		for(String line : super.inputfileLines) {
			DefaultSink sink = (DefaultSink) Parser.extractNumbers(line,false,-1,null);
			long[] numbers = Caster.castLong(sink.toArr());
			if(line.contains("Register A")) {
				registerAInit = numbers[0];
			}else if(line.contains("Register B")) {
				registerBInit = numbers[0];
			} if(line.contains("Register C")) {
				registerCInit = numbers[0];
			} if(line.contains("Program")) {
				programInit = numbers;
			}
		}
		
		ArrayList<Long> output = runProgram(registerAInit, registerBInit, registerCInit, programInit);
		part1(commaSeparatedString(output));
		
		
		
		// I have no idea how to properly solve day17 p2. So let's just use find and use pattern and do some clever bruteforcing
		
		// Three consecutive bits of registerA are determining one output number. But there are exceptions. 
		// first find a baseline
		int[] single3BitsParts = new int[programInit.length];
		int remaining = findOneTrippletAfterAnother(single3BitsParts,registerBInit, registerCInit,programInit).size();
		
		// then try to minimize the remaining digits 
		boolean again = true;
		while(again) {
			again = false;
			outer:
			for(int i = 0; i < single3BitsParts.length; i++) {
				for(int k = 0; k < 8; k++) {
					int[] copy = Arrays.copyOf(single3BitsParts, single3BitsParts.length);
					copy[i] = k;
					int remainingDiffs = findOneTrippletAfterAnother(copy,registerBInit, registerCInit,programInit).size();
					if(remainingDiffs < remaining) {
						// lets hope it is not a local minimum but the global, which we are tracking here...
						single3BitsParts = copy;
						again = true;

						remaining = remainingDiffs;
						break outer;
					}
					if(remainingDiffs == 0) {
						again = false;
						single3BitsParts = copy;
						remaining = 0;
						break outer;
					}
				}
			}
		}
		if(remaining == 0) {
			part2(concatTripplets(single3BitsParts));// if this is no the correct value you are sill very close.... 
		}else {
			throw new RuntimeException("Random::nextInt(8) is your friend. Add another outer loop around everything and change one value. simulated annealing.");
		}
		
		/*
		for(int i = 0; i < single3BitsParts.length; i++) {
			System.out.print(single3BitsParts[i] + ",");
		}
		System.out.println();
		*/
		
		
		// here are some impressions of my way to the solution above:
		/*
		long start = pow(8,15)-1;
		start += 0x10000000000L;
		
		int[] single3BitsParts = new int[]{3,0,0,4,5,3,6,7,5,6,1,7,1,4,1,1};
		single3BitsParts[0] = 1;
		
		Random random = new Random();
		
		ArrayList<Long> possibleResults = new ArrayList<Long>();
		while(true) {

			long number = 0;
			for(int i = single3BitsParts.length - 1; i >= 0; i--) {
				int previously = single3BitsParts[single3BitsParts.length-1-i];
				single3BitsParts[single3BitsParts.length-1-i] = random.nextInt(8);
				while(true) {
					number = 0;
					for(int k = 0; k < single3BitsParts.length; k++) {
						number = (number << 3) + (single3BitsParts[k] & 0x7);
					}
					output = runProgram(number, registerBInit, registerCInit, programInit);
					boolean digitOk = true;
					if(output.size() != programInit.length) {
						digitOk = false;
					}else {
						if(output.get(i) != programInit[i]) {
							digitOk = false;
						}
					}
					if(digitOk) {
//						System.out.println("test " + number);
						break;
					}else {
						single3BitsParts[single3BitsParts.length-1-i]++;
						if(single3BitsParts[single3BitsParts.length-1-i] > 7) {
							single3BitsParts[single3BitsParts.length-1-i] = previously;
//							System.out.println("digit " + i + " is not ok");
							break;
						}
					}
				}
			}
			
			for(int i = 0; i < single3BitsParts.length; i++) {
				System.out.print(single3BitsParts[i] + ",");
			}
			System.out.println("\n number=" + number);
			
			boolean done = true;
			
			if(output.size() != programInit.length) {
				done = false;
			}else {
				for(int i = 0; i < programInit.length; i++) {
					if(programInit[i] != output.get(i)) {
						done = false;
					}
				}
			}
			if(done) {
				possibleResults.add(number);
				if(possibleResults.size() > 50) {
					possibleResults.sort(new Comparator<Long>() {
	
						@Override
						public int compare(Long o1, Long o2) {
							if(o1 > o2) {
								return 1;
							}
							if(o1 < o2) {
								return -1;
							}
							return 0;
						}
					});
					System.out.println(possibleResults.get(0));
					System.out.println(possibleResults.get(possibleResults.size()-1));
					break;
				}
			}
		}
		
		// dump bruteforce:
		start = 105875099939593L;
		while(true) {
			System.out.println("running " + start);
			output = runProgram(start, registerBInit, registerCInit, programInit);
			//output = runMyProgram(start, registerBInit, registerCInit);
			if(output.size() == programInit.length) {
				boolean same = true;
				for(int i = 0; i < programInit.length; i++) {
					if(output.get(i) != programInit[i]) {
						same = false;
						break;
					}
				}
				if(same) {
					System.out.println(start);
					break;
				}
			}
			start += 0x100000;
		}
		*/
	}

	private ArrayList<Integer> findOneTrippletAfterAnother(int[] single3BitsParts, long regB, long regC, long[] program) {
		ArrayList<Integer> notSolveableTripplets = new ArrayList<Integer>();
		
		for(int i = single3BitsParts.length - 1; i >= 0; i--) {
			ArrayList<Long> output = runProgram(concatTripplets(single3BitsParts), regB, regC, program);
			boolean isRegAOk = output.size() == program.length && output.get(i) == program[i];
			if(isRegAOk) {
				continue;
			}
			
			int previousValue = single3BitsParts[single3BitsParts.length-1-i];
			single3BitsParts[single3BitsParts.length-1-i] = 0;
			while(true) {
				output = runProgram(concatTripplets(single3BitsParts), regB, regC, program);
				isRegAOk = output.size() == program.length && output.get(i) == program[i];
				if(isRegAOk) {
					break;
				}else {
					single3BitsParts[single3BitsParts.length-1-i]++;
					if(single3BitsParts[single3BitsParts.length-1-i] > 7) {
						single3BitsParts[single3BitsParts.length-1-i] = previousValue;
						notSolveableTripplets.add(i); // postponed. will be solved later. hopefully
						break;
					}
				}
			}
		}
		return notSolveableTripplets;
	}


	private String commaSeparatedString(ArrayList<Long> output) {
		String concatOutput = "";
		for(long l : output) {
			concatOutput += l+",";
		}
		if(concatOutput.length() > 0) {
			concatOutput = concatOutput.substring(0,concatOutput.length()-1);
		}
		return concatOutput;
	}


	private long concatTripplets(int[] single3BitsParts) {
		long regA;
		regA = 0;
		for(int k = 0; k < single3BitsParts.length; k++) {
			regA = (regA << 3) + (single3BitsParts[k] & 0x7);
		}
		return regA;
	}
	
	private static long pow(long basis, long exp) {
		long res = 1;
		if(basis == 1) {
			return 1;
		}
		if(exp == 0) {
			return 1;
		}
		while(exp>0) {
			res *= basis;
			exp--;
		}
		return res;
	}
	
	private static ArrayList<Long> runProgram(long regA, long regB, long regC, long[] program){
		ArrayList<Long> output = new ArrayList<Long>();
		//String debug = "regA: " + String.format("%x", regA) + " ";
		
		for(int i = 0; i < program.length; i+=2) {
			int opCode = (int) program[i];
			int operand = (int) program[i+1];
			long operandValue;
			switch(operand) {
			case 4: operandValue = regA; break;
			case 5: operandValue = regB; break;
			case 6: operandValue = regC; break;
			default: operandValue = operand; // 0-3
			}
			
			switch(opCode) {
			case 0:
				regA = regA / pow(2,operandValue);
				break;
			case 1: 
				regB = regB ^ operand;
				break;
			case 2: 
				regB = operandValue  & 0b111;
				break;
			case 3: 
				if(regA != 0) {
					//debug += " " + regA;
					i = operand -2;
				}
				break;
			case 4:
				regB = regB^regC;
				break;
			case 5:
				output.add(operandValue & 0b111);
				//debug += " " + (operandValue & 0b111);
				break;
			case 6:
				regB = regA / pow(2,operandValue);
				break;
			case 7:
				regC = regA / pow(2,operandValue);
				break;
			default:
				throw new RuntimeException("opcode = " + opCode);
			}
		}
		//System.out.println(debug);
		return output;
	}

}
