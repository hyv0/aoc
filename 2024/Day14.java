import java.util.ArrayList;
import java.util.Scanner;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day14 extends BaseSolution {
	
	

	public Day14() {
		super(14);
	}
	

	
	@Override
	protected void solveDay() {
		long p2 = 0;
		
		ArrayList<Robot> robots = new ArrayList<Day14.Robot>();
		for(String line : super.inputfileLines) {
			DefaultSink sink = (DefaultSink) Parser.extractNumbers(line,true,3,null);
			long[] numbers = Caster.castLong(sink.toArr());
			Robot r = new Robot(numbers[0],numbers[1], numbers[2], numbers[3]);
			robots.add(r);
		}
		
		int boundsx = 101;
		int boundsy = 103;
		long[][] quadrants = new long[2][2];
		for(Robot r : robots) {
			long x = r.x ;
			long y = r.y;
			
			x +=r. vx * 100;
			y += r.vy * 100;
			while(x < 0) {
				x+= boundsx;
			}
			while(y < 0) {
				y+= boundsy;
			}
			x = x % boundsx;
			y = y % boundsy;
			if(x != boundsx/2 && y != boundsy/2) {
				quadrants[(int) (x/ ((boundsx+1)/2))][(int) (y / ((boundsy+1)/2))] ++;
			}
			
		}
		
		part1(quadrants[0][0]*quadrants[1][0]*quadrants[0][1]*quadrants[1][1]);
		
		/*
		for(Robot r : robots) {
			for(Robot k : robots) {
				if(k==r) continue;
				if(r.vx == k.vx && r.vy == k.vy) {
					System.out.println(r.vx + " "+k.vy);
				}
			}
		}
		*/
		
		
		while(true) {
			char[][] map = new char[boundsy][boundsx];	
			for(Robot r : robots) {
				long x = r.x;
				long y = r.y;
				
				x +=r.vx;
				y += r.vy;
				while(x < 0) {
					x+= boundsx;
				}
				while(y < 0) {
					y+= boundsy;
				}
				r.x = x % boundsx;
				r.y = y % boundsy;
				map[(int) r.y][(int) r.x]='+';
				
			}

			p2++;
			
			/*
			// turns out, it is not symmetric....
			int symmetryScore = 0;
			
			
			for(int i = 0; i < boundsy; i++) {
				for(int j = 0; j < boundsx/2; j++) {
					if(map[i][j] == map[i][boundsx-1-j]) {
						symmetryScore++;
					}
				}
			}

			if(symmetryScore > boundsx * boundsy * 0.7) {
				Caster.printMatrix(map);
				break;
			}
			
			*/
			
			// calculate surrounding area
			ArrayList<Point> trunk = new ArrayList<Day14.Point>();
			boolean[][] trunkMap = new boolean[boundsy][boundsx];
			trunk.add(new Point(0,0));
			trunk.add(new Point(boundsy-1,0));
			trunk.add(new Point(boundsy-1,boundsx-1));
			trunk.add(new Point(0,boundsx-1));

			trunkMap[0][0] = true;
			trunkMap[boundsy-1][0] = true;
			trunkMap[boundsy-1][boundsx-1] = true;
			trunkMap[0][boundsx-1] = true;
			
			
			
			
			int sizeOfOuterSpace = 4;
			while(!trunk.isEmpty()) {
				Point p = trunk.removeFirst();
				for(int[] dir : new int[][] {{0,1},{1,0},{0,-1},{-1,0}}) {
					int x = p.j + dir[1];
					int y = p.i + dir[0];
					
					if(y >= 0 && y < trunkMap.length && x >= 0 && x < trunkMap[0].length) {
						Point pp = new Point(y,x);
						
						if(trunkMap[y][x] == false && map[y][x] == 0) {
							trunk.add(pp);
							trunkMap[pp.i][pp.j] = true;
							
							sizeOfOuterSpace++;
						}
					}
					
				}
			}
			
			
			
			
			if(sizeOfOuterSpace < map.length * map[0].length-1111) {
				//Caster.printMatrix(map); // check if the tree can be seen here
				break;
			}
		}
		part2(p2);
	}
	
	
	
	private static class Point{
		private int i;
		private int j;
		public Point(int i, int j) {
			this.i = i;
			this.j = j;
		}
	}
	
	private static class Robot{
		long x = 0;
		long y = 0;
		long vx = 0;
		long vy = 0;
		public Robot(long x, long y, long vx, long vy) {
			this.x = x;
			this.y = y;
			this.vx = vx;
			this.vy = vy;
		
		}
	}
	
	

}
