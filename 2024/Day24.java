import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;

import aoclib.BaseSolution;

public class Day24 extends BaseSolution {
	
	

	public Day24() {
		super(24);
	}
	

	
	@Override
	protected void solveDay() {
		ArrayList<InputNode> inputNodes = new ArrayList<Day24.InputNode>();
		ArrayList<CalcNode> calcNodes = new ArrayList<Day24.CalcNode>();
		HashMap<String, Node> allNodes = new HashMap<String, Day24.Node>();
		ArrayList<Node> outputNodes = new ArrayList<Day24.Node>();
		
		parseInput(inputNodes, calcNodes, allNodes);
		
		for(Node n : calcNodes) {
			if(n.name.startsWith("z") ) {
				outputNodes.add(n);
			}
		}
		outputNodes.sort(new Comparator<Node>() {
			@Override
			public int compare(Day24.Node o1, Day24.Node o2) {
				return o2.name.compareTo(o1.name);
			}
		});
		
		for(Node n : outputNodes) {
			n.calc();
		}
		long value = 0;
		for(Node n : outputNodes) {
			value <<= 1;
			if(n.value) {
				value ++;
			}
		}
		
		part1(value);
		
		
		// check if i understood the task correctly:
		/*
		HashSet<Node> shouldContain = new HashSet<Day24.Node>();
		for(int i = outputNodes.size()-1; i>=0; i--) {
			String number = String.format("%02d", outputNodes.size() - i-1);
			shouldContain.add(allNodes.get("x" + number));
			shouldContain.add(allNodes.get("y" + number));
			Node n = outputNodes.get(i);
			boolean containsAll = n.involvedInputNodes.containsAll(shouldContain);
			boolean sizeOk = n.involvedInputNodes.size() == n.involvedInputNodes.size();
			if(containsAll == false || sizeOk == false) {
				System.out.println("error at " + i);
			}
		}
		*/
		
		ArrayList<String> swapped = new ArrayList<String>();
		do {
		
			
			CalcNode initHelper = (Day24.CalcNode) allNodes.get("z01");
			CalcNode carry = (((Day24.CalcNode) initHelper.a).operation == OPERATION.AND )? (Day24.CalcNode) (initHelper).a: (Day24.CalcNode) (initHelper).b ;
			
			
			Node[] plsSwap = null;
			
			List<Day24.Node> outputNodesReversed = outputNodes.reversed();
			outer:
			for(int i = 1; i < outputNodesReversed.size()-1; i++ ) {
				String number = String.format("%02d", i);
				Node x = allNodes.get("x" + number);
				Node y = allNodes.get("y" + number);
				CalcNode z =  (Day24.CalcNode) allNodes.get("z" + number);
				
				if(z.operation != OPERATION.XOR) {
					for(CalcNode n : x.successors) {
						if((n.a == y || n.b == y) && n.operation == OPERATION.XOR) {
							for(CalcNode nn : n.successors) {
								if((nn.a == n || nn.b == n) && nn.operation == OPERATION.XOR) {
									plsSwap = new Node[] {nn,z};
									break outer;
								}
							}
							
						}
					}
					throw new RuntimeException("TODO: write code that proposes a swap for this error. i=" + i);
				}
	
				Node za =  z.a;
				Node zb =  z.b;
				
				
				
				
				boolean nodeAsingleBitXor = false;
				if(za instanceof CalcNode) {
					CalcNode za_ = (Day24.CalcNode) za;
					if(za_.operation == OPERATION.XOR && (za_.a == x || za_.a == y) && (za_.b == x || za_.b == y)) {
						// ok
						nodeAsingleBitXor = true;
					}
				}
				boolean nodeBsingleBitXor = false;
				if(zb instanceof CalcNode) {
					CalcNode zb_ = (Day24.CalcNode) zb;
					if(zb_.operation == OPERATION.XOR && (zb_.a == x || zb_.a == y) && (zb_.b == x || zb_.b == y)) {
						// ok
						nodeBsingleBitXor = true;
					}
				}
				if(!nodeAsingleBitXor && !nodeBsingleBitXor) {
					for(CalcNode n : x.successors) {
						if((n.a == y || n.b == y) && n.operation == OPERATION.XOR) {
							if(za == carry) {
								plsSwap = new Node[] {zb,n};
							}else {
								plsSwap = new Node[] {za,n};
							}
							break outer;
						}
					}
					throw new RuntimeException("TODO: write code that proposes a swap for this error. i=" + i);
				}else {
					
					Node usedCarry = nodeAsingleBitXor ? zb : za;
					if(usedCarry != carry) {
						throw new RuntimeException("TODO: write code that proposes a swap for this error. i=" + i);
					}
					
					Node thisXor = nodeAsingleBitXor ? za : zb;

					 
					
					CalcNode carryInfluence = null;
					for(CalcNode n : carry.successors) {
						if((n.a == thisXor || n.b == thisXor) && n.operation == OPERATION.AND) {
							carryInfluence = n;
						}
					}

					
					if(carryInfluence.operation != OPERATION.AND) {
						System.out.println("error");
					}
					if(carryInfluence.successors.size() != 1) {
						throw new RuntimeException("TODO: write code that proposes a swap for this error. i=" + i);
					}

					carry = carryInfluence.successors.toArray(CalcNode[]::new)[0];
					if(carry.operation != OPERATION.OR) {
						throw new RuntimeException("TODO: write code that proposes a swap for this error. i=" + i);
					}
					Node othercarryInfluence = carry.a == carryInfluence ? carry.b : carry.a;
					if(othercarryInfluence instanceof CalcNode == false) {
						throw new RuntimeException("TODO: write code that proposes a swap for this error. i=" + i);
					}else {
						CalcNode othercarryInfluence_ = (Day24.CalcNode) othercarryInfluence;
						if(((othercarryInfluence_.a == x && othercarryInfluence_.b == y) || (othercarryInfluence_.a == y && othercarryInfluence_.b == x)) && othercarryInfluence_.operation == OPERATION.AND){
							
						}else {
							throw new RuntimeException("TODO: write code that proposes a swap for this error. i=" + i);
						}
					}
					
				}
			}
			if(plsSwap == null) {
				break;
			}else {
				CalcNode a = (Day24.CalcNode) plsSwap[0];
				CalcNode b = (Day24.CalcNode) plsSwap[1];
				//System.out.println("Swapping " + a.name + " and " + b.name);

				a.a.successors.remove(a);
				a.b.successors.remove(a);
				b.a.successors.remove(b);
				b.b.successors.remove(b);
				a.a.successors.add(b);
				a.b.successors.add(b);
				b.a.successors.add(a);
				b.b.successors.add(a);
				Node aa = a.a;
				Node ab = a.b;
				OPERATION ao = a.operation;
				a.a = b.a;
				a.b = b.b;
				a.operation = b.operation;
				b.a = aa;
				b.b = ab;
				b.operation = ao;
				
				swapped.add(a.name);
				swapped.add(b.name);
				
			}
		}while(true);
		swapped.sort(new Comparator<String>() {
			public int compare(String o1, String o2) {
				return o1.compareTo(o2);
			}
		});
		String p2 = "";
		for(String g : swapped) {
			p2+= "," + g;
		}
	
		part2(p2.substring(1));
	}



	private void parseInput(ArrayList<InputNode> inputNodes, ArrayList<CalcNode> calcNodes, HashMap<String, Node> allNodes) {
		for(String line : super.inputfileLines) {
			if(line.isBlank()) continue;
			if(line.contains(":")) {
				int sepIndex = line.indexOf(':');
				InputNode node = new InputNode();
				node.name = line.substring(0,sepIndex);
				node.value = line.substring(sepIndex).contains("1");
				inputNodes.add(node);
				allNodes.put(node.name, node);
			}else {
				int state = 0;
				CalcNode node = new CalcNode();
				int secondNameStart = 0;
				loop:
				for(int i = 0; i < line.length(); i++) {
					char c = line.charAt(i);
					switch(state) {
					case 0:
						if(c== ' ') {
							node.aName = line.substring(0,i);
							state = 1;
						}
						break;
					case 1:
						if(c=='O') {
							node.operation = OPERATION.OR;
						}else if(c=='A') {
							node.operation = OPERATION.AND;
						}else if(c=='X') {
							node.operation = OPERATION.XOR;
						}else {
							throw new RuntimeException("invalid input");
						}
						state = 2;
						break;
					case 2:
						if(c==' ') {
							state = 3;
							secondNameStart = i+1;
						}
						break;
					case 3:
						if(c==' ') {
							node.bName = line.substring(secondNameStart, i);
							state = 4;
						}
						break;
					case 4:
						if(c=='-') {
							state = 5;
						}
						break;
					case 5:
						if(c=='>') {
							state = 6;
						}
						break;
					case 6:
						if(c==' ') {
							state = 7;
						}
						break;
					case 7:
						node.name = line.substring(i);
						state = 8;
						break;
					case 8:
						break loop;
					default:
						throw new RuntimeException("ops");
					}
				}
				if(state != 8) {
					throw new RuntimeException("invalid input for string \"" + line+"\"");
				}
				calcNodes.add(node);
				allNodes.put(node.name, node);
			}
			
		}
		for(CalcNode n : calcNodes) {
			n.a = allNodes.get(n.aName);
			n.b = allNodes.get(n.bName);
			n.a.successors.add(n);
			n.b.successors.add(n);
		}
	}
	
	private static abstract class Node{
		String name;
		boolean value;
		boolean valueValid;
		//HashSet<InputNode> involvedInputNodes = new HashSet<Day24.InputNode>();
		HashSet<CalcNode> successors = new HashSet<Day24.CalcNode>();
		protected abstract void calc();
		
		abstract CalcNode getSuccessor(Node withThisSecondOperand);
	}
	
	private static class InputNode extends Node{

		@Override
		protected void calc() {
			if(valueValid == false) {
				valueValid = true;
				//involvedInputNodes.add(this);
			}
		}

		@Override
		Day24.CalcNode getSuccessor(Day24.Node withThisSecondOperand) {
			return null;
		}
	}
	
	private static class CalcNode extends Node{
		Node a;
		Node b;
		String aName;
		String bName;
		OPERATION operation;
		@Override
		protected void calc() {
			if(valueValid)
				return;
			
			if(!a.valueValid) {
				a.calc();
			}
			if(!b.valueValid) {
				b.calc();
			}
			switch (operation) {
			case AND: 
				value = a.value && b.value;break;
			case OR: 
				value = a.value || b.value;break;
			case XOR: 
				value = a.value ^ b.value;break;
			}
			valueValid = true;
			//involvedInputNodes.addAll(a.involvedInputNodes);
			//involvedInputNodes.addAll(b.involvedInputNodes);
		}
		@Override
		Day24.CalcNode getSuccessor(Day24.Node withThisSecondOperand) {
			for(CalcNode n : successors) {
				if(n.b == withThisSecondOperand || (n.a == withThisSecondOperand)){
					return n;
				}
			}
			return null;
		}
	}
	
	private static enum OPERATION{
		AND, OR, XOR;
	}

}
