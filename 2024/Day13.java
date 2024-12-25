import java.math.BigInteger;
import java.util.ArrayList;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day13 extends BaseSolution {
	
	

	public Day13() {
		super(13);
	}
	

	
	@Override
	protected void solveDay() {
		long p1 = 0;
		long p2 = 0;
		
		
		ArrayList<Machine> machines = new ArrayList<Machine>();
		Machine m = new Machine();
		for(String line : super.inputfileLines) {
			DefaultSink sink = (DefaultSink) Parser.extractNumbers(line,false,-1,null);
			long[] numbers = Caster.castLong(sink.toArr());
			
			if(line.contains("Button A")) {
				m.xa = numbers[0];
				m.ya = numbers[1];
			}
			if(line.contains("Button B")) {
				m.xb = numbers[0];
				m.yb = numbers[1];
			}
			if(line.contains("Prize")) {
				m.xprice = numbers[0];
				m.yprice = numbers[1];
				machines.add(m);
				m = new Machine();
			}
		}
		
		for(Machine machine : machines) {
			for(int p = 0; p < 2; p++) {
				long a = 0;
				long b = 0;
				long div = (machine.xb * machine.ya - machine.yb * machine.xa);
				if(div != 0) {
					b = (machine.xprice * machine.ya - machine.xa * machine.yprice) /  div;
					a  = (machine.yprice - machine.yb * b) /  machine.ya;
				}else {
					b = machine.xprice / machine.xb;
				}
				//System.out.println("a= " +a + "  b=" + b);
				//System.out.println(machine.xa  + "*"+ a+ "+"+ machine.xb +"*"+ b +"=" +  (machine.xa * a + machine.xb * b) + " =?= "+machine.xprice );
				//System.out.println(machine.ya  + "*"+ a+ "+"+ machine.yb +"*"+ b +"=" +  (machine.ya * a + machine.yb * b) + "=?="+machine.yprice );
				
				if(machine.xa * a + machine.xb * b == machine.xprice ) {
					if(machine.ya * a + machine.yb * b == machine.yprice) {
						if(a >= 0 && b >= 0) {
							//System.out.println("adding "+(a*3 + b));
							if(p == 0) {
								p1 += a* 3 + b;
							}else {
								p2 += a* 3 + b;
							}
						}
					
					}
					
				}

				machine.xprice += 10000000000000L;
				machine.yprice += 10000000000000L;
			}
			
			/*
			// old p2:
			// Because at first div was calculated like this double div = (machine.xb - machine.yb * machine.xa / (double) machine.ya) the code did not work.
			// To get rid of the negative numbers, BigInteger has been used. For this to work the division also needed to be eliminated.
			// Afterwards it became clear, that only getting rid of the division is sufficient. 
			System.out.println("\n\n  p2\n\n");
			
			machine.xprice += 10000000000000L;
			machine.yprice += 10000000000000L;
			long p2_diff = machine.xb * machine.ya - machine.yb * machine.xa;
			if(div != 0) {
				b = BigInteger.valueOf(machine.xprice).multiply(BigInteger.valueOf(machine.ya)).subtract(BigInteger.valueOf(machine.xa).multiply(BigInteger.valueOf(machine.yprice))).divide(BigInteger.valueOf(p2_diff)).longValue();
				a = BigInteger.valueOf(machine.yprice).subtract(BigInteger.valueOf(machine.yb).multiply(BigInteger.valueOf(b))).divide(BigInteger.valueOf(machine.ya)).longValue();
			}else {
				b = machine.xprice / machine.xb;
				a = 0;
			}
			System.out.println(machine.xa  + "*"+ a+ "+"+ machine.xb +"*"+ b +"=" +  (machine.xa * a + machine.xb * b) + " =?= "+machine.xprice );
			System.out.println(machine.ya  + "*"+ a+ "+"+ machine.yb +"*"+ b +"=" +  (machine.ya * a + machine.yb * b) + "=?="+machine.yprice );
			if(machine.xa * a + machine.xb * b == machine.xprice ) {
				if(machine.ya * a + machine.yb * b == machine.yprice) {
					if(a >= 0 && b >= 0) {
						p2 += a* 3 + b;
						
					}
				}
			}
			*/
		}
		
		part1(p1);
		part2(p2);
	}

	private static class Machine{
		private long xa;
		private long ya;
		private long xb;
		private long yb;
		private long xprice;
		private long yprice;
	}
	
}
