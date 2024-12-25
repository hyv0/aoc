import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day22 extends BaseSolution {
	
	

	public Day22() {
		super(22);
	}
	
	
	@Override
	protected void solveDay() {
		long p1 = 0;
		long p2 = 0;

		
		int[][] prices = new int[super.inputfileLines.length][2001];
		//TreeItem[] priceChanges = new TreeItem[super.inputfileLines.length];
		
		int lineNumber = 0;
		for(String line : super.inputfileLines) {
			DefaultSink sink = (DefaultSink) Parser.extractNumbers(line,false,-1,null);
			long[] numbers = Caster.castLong(sink.toArr());
			long initial = numbers[0];
			long afterSimulating = initial;
			int index = 0;
			prices[lineNumber][index++] = (int) (afterSimulating %10);
			for(int i = 2000; i > 0; i--) {
				afterSimulating = simulate(afterSimulating);
				int price = (int) (afterSimulating %10);
				prices[lineNumber][index++] = price ;
			}
			p1 += afterSimulating;
			lineNumber++;
		}
		part1(p1);
		
	
		lineNumber--;
		TreeItem root =  new TreeItemMid();
		for(; lineNumber>= 0; lineNumber--) {
			//TreeItem root =  new TreeItemMid();
			//priceChanges[lineNumber] = root;
			int[] changes = new int[4];
			int priceIndex = 0;
			int validCounter = changes.length-1;
			int prev = prices[lineNumber][priceIndex++];
			while(priceIndex != prices[lineNumber].length) {
				int price = prices[lineNumber][priceIndex++];
				int diff = price - prev;
				
				changes[0] = changes [1];
				changes[1] = changes [2];
				changes[2] = changes [3];
				changes[3] = diff;
				
				
				if(validCounter != 0) {
					validCounter--;
				}else {
					int bananas = root.registerIfNotEmpty(changes, 0, price,lineNumber);
					if(bananas > p2) {
						p2 = bananas;
					}
				}
				prev = price;
			}
		}
		
		/*
		// instead of storing an array of trees and then afterwards searching the highest banana count, it is better
		// to store one tree which has one boolean array per TreeItemEnd and sum up the sums directly. 
		// So this by factor 10 slower running code is no longer necessary:
		int[] diffArray = new int[4];
		for(int a = -9; a <= 9; a++) {
			for(int b = -9; b <= 9; b++) {
				for(int c = -9; c <= 9; c++) {
					for(int d = -9; d <= 9; d++) {
						diffArray[0] = a;
						diffArray[1] = b;
						diffArray[2] = c;
						diffArray[3] = d;
						long bananas = 0;
						for(int lineNr = super.inputfileLines.length - 1; lineNr>= 0; lineNr--) {
							int p =  priceChanges[lineNr].getCostFor(diffArray,0);
							bananas += p;
						}
						if(bananas > p2) {
							p2 = bananas;
						}
					}
				}
			}
		}
		*/
		
		part2(p2);
	}
	
	public long simulate(long secret) {
		long multiplied = secret << 6;
		secret = (secret ^ multiplied) & 0xFFFFFF;
		long div = secret >> 5;
		secret = (secret ^ div)  & 0xFFFFFF;
		long mult = secret << 11;
		secret = (secret ^ mult)  & 0xFFFFFF;
		return secret;
	}
	
	

	private static interface TreeItem{
		

		public int registerIfNotEmpty(int[] diffArray, int index, int cost, int lineNr);
		//public int getCostFor(int[] diffArray, int index);
		
	}
	
	private static class TreeItemMid implements TreeItem{
		
		private TreeItem[] childs;
		
		TreeItemMid() {
			childs = new TreeItem[19];
		}
		
		public int registerIfNotEmpty(int[] diffArray, int index, int cost, int lineNr) {
			int diff = diffArray[index];
			if(childs[diff+9] == null) {
				TreeItem n;
				if(diffArray.length == index + 1) {
					n = new TreeItemEnd();
				}else {
					n = new TreeItemMid();
				}
				childs[diff+9] = n;
				return n.registerIfNotEmpty(diffArray, index+1, cost, lineNr);
			}else {
				return childs[diff+9].registerIfNotEmpty(diffArray, index+1, cost, lineNr);
			}
		}
		/*
		public int getCostFor(int[] diffArray, int index) {
			int diff = diffArray[index];
			if(childs[diff+9] == null) {
				return 0; // not sold
			}else {
				return childs[diff+9].getCostFor(diffArray,index+1);
			}
		}
		*/
	}
	

	private static class TreeItemEnd implements TreeItem{

		private int cost = 0;
		
		private boolean[] alreadyRegistered = new boolean[2000];
		
		@Override
		public int registerIfNotEmpty(int[] diffArray, int index, int cost, int lineNr) {
			if(alreadyRegistered[lineNr]) {
				
			}else {
				alreadyRegistered[lineNr] = true;
				this.cost += cost;
			}
			return this.cost;
		}
		/*
		@Override
		public int getCostFor(int[] diffArray, int index) {
			return cost;
		}
		*/
	}

}
