import java.util.ArrayList;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day03 extends BaseSolution {

	public Day03() {
		super(3);
	}
	
	private static class Tuple{
		private int a;
		private int b;
		private int index;
		private boolean enabled = true;
		public Tuple(int a, int b, boolean en, int index) {
			this.a=a;
			this.b=b;
			this.enabled = en;
			this.index = index;
		}
	}
	private static class LineMult{
		private ArrayList<Tuple> list= new ArrayList();

	}

	@Override
	protected void solveDay() {
		long sum = 0;
		long p2 = 0;


		
		
		
		ArrayList<LineMult> mults = new ArrayList<Day03.LineMult>();
		
		boolean enabled = true;
		for(String line : super.inputfileLines) {
			LineMult l = new LineMult();
			mults.add(l);
			int index = 0;
			
			
			while(index < line.length()) {
				
				
				int start = line.indexOf("mul(", index);
				
				if(start == -1) {
					index++;
					
					continue;
				}else {
					index = start+1;
				}
				if(start == -1) continue;
				int comma = line.indexOf(",",start);
				if(comma == -1) continue;
				int end = line.indexOf(")",comma);
				if(end==-1) continue;
				String a = line.substring(start+4, comma);
				String b = line.substring(comma+1,end);
				if(a.length() > 3 || b.length() > 3) continue;
				
				boolean ok = true;
				for(char c : a.toCharArray()) {
					if(c < '0' || c>'9') {
						ok=false;
					}
				}
				for(char c : b.toCharArray()) {
					if(c < '0' || c>'9') {
						ok=false;
					}
				}
				int aa = Integer.parseInt(a);
				int bb = Integer.parseInt(b);
				
				int moddo = line.lastIndexOf("do()",end);
				int moddont = line.lastIndexOf("don't()", end);
				if(moddo != -1 && moddont != -1) {
					if(moddo > moddont) {
						enabled = true;
					}else {
						enabled = false;
					}
				}else {
					if(moddo != -1) {
						enabled = true;
					}else {
						if(moddont != -1) {
							enabled = false;
							
						}else {
							// both -1
							//enabled = true; 
						}
					}
				}
				l.list.add(new Tuple(aa,bb,enabled,start));
				
				
				
				
				
			}
			
		}
		
		for(LineMult m : mults) {
			for(Tuple t : m.list) {
				sum += t.a * t.b;
				if(t.enabled) {
					p2 += t.a * t.b;
					
				}
			}
		}
		
		part1(sum);
		part2(p2);
	}

}
