import java.util.ArrayList;
import java.util.Comparator;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day05 extends BaseSolution {
	
	

	public Day05() {
		super(5);
	}
	

	private static class Rule{
		int numberA;
		int numberB;
		
		Rule(int first, int second) {
			numberA = first;
			numberB = second;
		}
	}
	
	private static class GraphElement{
		ArrayList<GraphElement> successors = new ArrayList<GraphElement>();
		ArrayList<GraphElement> predecessors = new ArrayList<GraphElement>();
		boolean[] allPredecessors = new boolean[100];
		int allPredecessorsCount = 0;
		final int index;
		public GraphElement(int index) {
			this.index = index;
		}
		public void calcAllPredecessors() {
			for(GraphElement p  : predecessors) {
				p.calcAllPredecessorsHelper(allPredecessors,this);
			}
		}
		private void calcAllPredecessorsHelper(boolean[] registerHere, GraphElement counter) {
			if(registerHere[index]== false) {
				registerHere[index] = true;
				counter.allPredecessorsCount++;
				for(GraphElement p : predecessors) {
					p.calcAllPredecessorsHelper(registerHere, counter);
				}
			}
		}
		

	}

	
	@Override
	protected void solveDay() {

		long p1=0;
		long p2 = 0;
		
		ArrayList<Rule> rules = new ArrayList<Rule>();
		
		GraphElement[] allElements = null;
		
		for(String line : super.inputfileLines) {
			DefaultSink sink = (DefaultSink) Parser.extractNumbers(line,false,2,null);
			long[] numbers = Caster.castLong(sink.toArr());
			if(numbers.length == 0) continue;
			boolean isRule = line.contains("|");
			if(isRule) {
				rules.add(new Rule((int)numbers[0],(int) numbers[1]));
			}else {
				boolean p1ok = isOk(rules, numbers);
				if(p1ok) {
					if(numbers.length == 0) System.out.println("test" + line+"|");
					p1 += numbers[numbers.length / 2];
				}
				
				// ====================== part 2 ================================
				
				if(allElements == null) {
					// INIT for part2:
					allElements = new GraphElement[100];
					for(int i = 0; i < allElements.length;i++) {
						allElements[i] = new GraphElement(i);
					}
					for(Rule r : rules) {
						int a = r.numberA;
						int b = r.numberB;
						allElements[a].successors.add(allElements[b]);
						allElements[b].predecessors.add(allElements[a]);
					}
				}
				
				// init done now
				
				GraphElement[] subtree = new GraphElement[100];
				ArrayList<GraphElement> subtreeList = new ArrayList<GraphElement>();
				for(long l : numbers) {
					int i = (int) l;
					subtree[i] = new GraphElement(i);
					subtreeList.add(subtree[i]);
				}
				for(long l : numbers) {
					int i = (int) l;
					GraphElement t = subtree[i];
					for(GraphElement p : allElements[i].predecessors) {
						if(subtree[p.index] != null) {
							t.predecessors.add(subtree[p.index]);
						}
					}
					for(GraphElement p : allElements[i].successors) {
						if(subtree[p.index] != null) {
							t.successors.add(subtree[p.index]);
						}
					}
				}

				for(GraphElement p : subtreeList) {
					p.calcAllPredecessors();
				}
				subtreeList.sort(new Comparator<GraphElement>() {
					public int compare(GraphElement o1, GraphElement o2) {
						return o1.allPredecessorsCount - o2.allPredecessorsCount;
					}
				});
				
				if(!p1ok) {
					
					p2 += subtreeList.get(subtreeList.size()/2).index;
				}
				
				/*

				for(GraphElement p : subtreeList) {
					p.calcAllPredecessors();
					String g = "";
					for(int i = 0; i < 100; i++) {
						if(p.allPredecessors[i]) {
							g+="["+i+"] ";
						}
					}
					System.out.println(p.index + " : " + g);
				}
				System.out.print("======\n");
				*/
			}
			
			
		}
		part1(p1);
		part2(p2);
	}


	private boolean isOk(ArrayList<Rule> rules, long[] numbers) {
		// ok let's bruteforce part1 and see what part2 is about
		boolean ok = true;
		for(int i = 0; i < numbers.length; i++) {
			for(int j = i+1; j < numbers.length; j++) {
				int a = (int) numbers[i];
				int b = (int) numbers[j];
				for(Rule r : rules) {
					if(r.numberA == b && r.numberB == a) {
						ok=false;
					}
				}
			}
		}
		return ok;
	}

}
