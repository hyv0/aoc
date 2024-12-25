/**
 * Main class for advent of code 2024
 * The Code of the aoclib and all DayXY.java files have been written by hyv0.
 */
import java.util.Scanner;
import java.util.TreeSet;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.DeterministicStateMachine;
import aoclib.NonDeterministicStateMachine;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Main {

	public static final int FIXED_DAY = -1; // -1
	public static final boolean SOLVE_ALL = false;
	
	
	
	public static void main(String[] args) {
		System.out.println("AOC 2024");
		
		if(!SOLVE_ALL) {
			int day = FIXED_DAY;
			Scanner in = new Scanner(System.in);
			while(day < 1 || day > 25) {
				try {
					System.out.println("load solution for day: ");
					day = in.nextInt();
				}catch(Exception e) {
					
				}
			}
			in.close();
			System.out.println("==========================");
			System.out.println("||       D A Y  " +(day<10 ? "0" : "")+day+"      ||");
			System.out.println("==========================");
			


			BaseSolution solution = getSolution(day);
			if(solution != null) {
				solution.solve();
			}
		}else {
			long startTime = System.nanoTime();
			for(int i = 1; i<= 25; i++) {
				BaseSolution solution = getSolution(i);
				if(solution != null) {
					solution.solve();
				}
			}
			long endTime = System.nanoTime();
			System.out.println("==========================");
			System.out.println("||    done in "+(endTime - startTime)/1000000000+" sec    ||");
			System.out.println("==========================");
			
			
		}
		
		
		
		
		
		
	}



	private static BaseSolution getSolution(int day) {
		BaseSolution solution = null;
		switch(day) {
		case  1: solution = new Day01(); break;
		case  2: solution = new Day02(); break;
		case  3: solution = new Day03(); break;
		case  4: solution = new Day04(); break;
		case  5: solution = new Day05(); break;
		case  6: solution = new Day06(); break;
		case  7: solution = new Day07(); break;
		case  8: solution = new Day08(); break;
		case  9: solution = new Day09(); break;
		case 10: solution = new Day10(); break;
		case 11: solution = new Day11(); break;
		case 12: solution = new Day12(); break;
		case 13: solution = new Day13(); break;
		case 14: solution = new Day14(); break;
		case 15: solution = new Day15(); break;
		case 16: solution = new Day16(); break;
		case 17: solution = new Day17(); break;
		case 18: solution = new Day18(); break;
		case 19: solution = new Day19(); break;
		case 20: solution = new Day20(); break;
		case 21: solution = new Day21(); break;
		case 22: solution = new Day22(); break;
		case 23: solution = new Day23(); break;
		case 24: solution = new Day24(); break;
		case 25: solution = new Day25(); break;
		default: 
			System.err.println("Day " + day + " is not implemented!");
			return null;
		}
		return solution;
	}
}
