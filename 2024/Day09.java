import java.util.ArrayList;

import aoclib.BaseSolution;
import aoclib.Caster;
import aoclib.Parser;
import aoclib.Parser.DefaultSink;

public class Day09 extends BaseSolution {
	
	
	

	public Day09() {
		super(9);
	}
	

	
	@Override
	protected void solveDay() {
		long p1 = 0;
		long p2 = 0;
		
		ArrayList<Input> inputs = new ArrayList<Day09.Input>();
		boolean isFile = true;
		int id = 0;
		for(int i = 0; i < super.inputfileLines[0].length(); i++) {
			char c = super.inputfileLines[0].charAt(i);
			if(isFile) {
				inputs.add(new Input(id++,c-'0'));
			}else {
				inputs.add(new Input(c-'0'));
			}
			isFile = !isFile;
		}
		int[] filesystem = new int[inputs.size()*9];
		int[] filesystem2 = new int[inputs.size()*9];
		
		int filesystemSize = 0;
		for(Input p : inputs) {
			for(int k = 0; k < p.length; k++) {
				if(p.isFreeSpace) {
					filesystem[filesystemSize] = -1;
					filesystem2[filesystemSize] = -1;
				}else {
					filesystem[filesystemSize] = p.id;
					filesystem2[filesystemSize] = p.id;
				}
				filesystemSize++;
			}
		}
		
		int frontPointer = -1;
		int backPointer = filesystemSize;
		while(true) {
			do {
				frontPointer++;
			}while(filesystem[frontPointer] != -1);
			do {
				backPointer--;
			}while(filesystem[backPointer] == -1);
			if(frontPointer >= backPointer) {
				break;
			}
			filesystem[frontPointer] = filesystem[backPointer];
			filesystem[backPointer] = -1;
		}
		
		for(int i = 0; i < filesystemSize; i++) {
			int bid = filesystem[i];
			if(bid == -1) break;
			p1 += i * bid;
		}
		
		
		Input[] inputsById = new Input[id];
		for(Input p : inputs) {
			if(p.isFreeSpace) continue;
			inputsById[p.id] = p;
		}
		

		inputs.removeIf((p)->p.isFreeSpace && p.length == 0);
		
		for(int i = inputsById.length-1;  i>= 0; i--) {
			Input p = inputsById[i];
			if(p.isFreeSpace) continue;
			// try to move p
			for(int searchFreeSpaceIndex = 0; searchFreeSpaceIndex < inputs.size(); searchFreeSpaceIndex++) {
				Input checkSpace = inputs.get(searchFreeSpaceIndex);
				if(p == checkSpace) {
					break;
				}
				if(checkSpace.isFreeSpace) {
					if(checkSpace.length == p.length) {
						//inputs.remove(p);
						removeElement(inputs, p, searchFreeSpaceIndex);
						inputs.set(searchFreeSpaceIndex, p);
						break;
					}
					if(checkSpace.length > p.length) {
						
						removeElement(inputs, p, -1);
						checkSpace.length -= p.length;
						inputs.add(searchFreeSpaceIndex, p);
						break;
					}
				}
			}
			

			
			
			boolean lastt = false;
			for(Input pp : inputs) {
				if((pp.isFreeSpace == true) && lastt) {
					System.out.println("error after doing " + p.id + " at index="+inputs.indexOf(pp));
				}
				
				lastt = pp.isFreeSpace;
				
			}
			
		}
		
		int index = 0;
		boolean last = false;
		for(Input p : inputs) {
			if(p.isFreeSpace == true && last) {
				System.out.println("ERROR");
			}
			if(p.isFreeSpace) {
				index += p.length;
			}else {
				for(int i = 0; i < p.length; i++) {
					p2 += index * p.id;
					index++;
				}
			}
			last = p.isFreeSpace;
			
		}
		
		
		
		
		part1(p1);
		part2(p2);
	}
	
	
	public void removeElement(ArrayList<Input> inputs, Input p, int doNotMergeThisIndex) {
		int index = inputs.indexOf(p);
		Input newSpace = new Input(p.length);
		boolean merged = false;
		if(index > 0 && (index-1) != doNotMergeThisIndex) {
			Input before = inputs.get(index-1);
			if(before.isFreeSpace) {
				before.length += p.length;
				inputs.remove(p);
				merged = true;
				index--;	// uff... i found it. finally.
				newSpace = before;
			}
		}
		if(index < inputs.size()-1  && (index+1) != doNotMergeThisIndex ) {
			Input after = inputs.get(index+1);
			if(after.isFreeSpace) {
				inputs.remove(after);
				newSpace.length += after.length;
			}
		}
		if(!merged) {
			inputs.set(index, newSpace);
		}
		
	}

	public static class Input{
		public final boolean isFreeSpace;
		public final int id;
		public int length;
		
		public Input(int id, int length) {
			this.id = id;
			this.length = length;
			isFreeSpace = false;
		}
		public Input(int length) {
			this.id = -1;
			this.length = length;
			isFreeSpace = true;
		}
		
	}
	
}
