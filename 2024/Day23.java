import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day23 extends BaseSolution {
	

	private final int[][] DIRS = new int[][] {{0,1},{1,0},{0,-1},{-1,0}}; 

	public Day23() {
		super(23);
	}
	

	
	@Override
	protected void solveDay() {
		long p1 = 0;

		int indexCount = 0;
		HashMap<String, Integer> nameIndexMapping = new HashMap<String, Integer>();
		for(String line : super.inputfileLines) {
			String[] names = line.split("-");
			for(String g : names) {
				if(!nameIndexMapping.containsKey(g)) {
					nameIndexMapping.put(g, indexCount++);
				}
			}
		}
		int[][] graphMap = new int[nameIndexMapping.size()][nameIndexMapping.size()];
		String[] names = new String[nameIndexMapping.size()];
		for(String line : super.inputfileLines) {
			String[] namesSplit = line.split("-");
			int a = nameIndexMapping.get(namesSplit[0]);
			int b = nameIndexMapping.get(namesSplit[1]);
			graphMap[a][b] = 1;
			graphMap[b][a] = 1;
			names[a] = namesSplit[0];
			names[b] = namesSplit[1];
		}
		//System.out.println("Graph built. Size = " + nameIndexMapping.size());
		
		
		// i am sure there are super efficient algorithms for this problem. But bruteforcing works as well (If it didn't, this comment wouldn't exist)
		for(int i = 0; i < nameIndexMapping.size(); i++) {
			ArrayList<Integer> c = new ArrayList<Integer>();
			for( int j = i+1; j < nameIndexMapping.size(); j++) {
				if(j == i) continue;
				if(graphMap[i][j] != 0) {
					c.add(j);
				}
			}
			for(int k = 0; k < c.size(); k++) {
				for(int m = k+1; m < c.size(); m++) {
					int ii = c.get(k);
					int iii = c.get(m);
					if(graphMap[ii][iii] != 0) {
						if(names[i].charAt(0) == 't' || names[ii].charAt(0) == 't' || names[iii].charAt(0) == 't') {
							//System.out.println(i+"  " + ii + "  "+ iii);
							p1++;
						}
					}
				}
				
			}
		}
		part1(p1);
		
		
		
		// ====================
		// part 2
		// ====================
		

		// clique problem. It is NP-hard.
		

		// ok, i looked at the first few graphics of this paper:
		// https://arxiv.org/pdf/1503.04794
		
		int largestCompleteSubgraph = 0;
		ArrayList<Integer> largestCompleteSubgraphNodeList = null;
		for(int i = 0; i < names.length; i++) {
			for(int j = 0; j < names.length; j++) {
				if(j == i) continue;
				if(graphMap[i][j] == 0) continue;
				// i is connected to j. what is connected to these nodes
				ArrayList<Integer> subgraph = new ArrayList<Integer>();
				subgraph.add(i);
				subgraph.add(j);
				for(int k = 0; k < names.length; k++) {
					if(k==i || k == j) continue;
					// check if k can be added to the subgraph. Check if k is connected to all of the nodes in $subgraph
					if(graphMap[i][k] != 0) {
						boolean ok = true;
						for(int m : subgraph) {
							if(graphMap[k][m] == 0) {
								ok=false;
								break;
							}
						}
						if(ok) {
							subgraph.add(k);
						}
					}
				}
				/*
				String g = "";
				for(int k : subgraph) {
					g+= names[k] + ", ";
				}
				System.out.println("subgraph with size=" + subgraph.size() + "     " + g);
				*/
				if(subgraph.size() > largestCompleteSubgraph) {
					largestCompleteSubgraph = subgraph.size();
					largestCompleteSubgraphNodeList = subgraph;
				}
			}
		}
		
		ArrayList<String> nodeNamesOfCompleteSubgraph = new ArrayList<String>();
		for(int i : largestCompleteSubgraphNodeList) {
			nodeNamesOfCompleteSubgraph.add(names[i]);
		}
		nodeNamesOfCompleteSubgraph.sort(new Comparator<String>() {
			@Override
			public int compare(String o1, String o2) {
				return o1.compareTo(o2);
			}
		});
		String g = "";
		for(String name : nodeNamesOfCompleteSubgraph) {
			g+= ","+name;
		}
		part2(g.substring(1));
		
		
		
	}
	
	
	
	
	
	

}
