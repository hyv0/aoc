package aoclib;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.function.IntFunction;

public class Caster {

	public static <E> void print(ArrayList<E> arr, String lineEnd){
		if(arr == null || arr.size() < 1) {
			System.out.println("list " + arr+ " is empty ");
			return;
		}
		String blanks = "";
		int b = arr.size();
		while(b > 10) {
			b /= 10;
			blanks += " ";
		}
		b = 10;
		Iterator<E> it = arr.iterator();
		for(int i = 0; i < arr.size(); i++) {
			String index = blanks + i;
			System.out.println("[" + index+ "] " + it.next() + lineEnd);
			if(i == b-1 && blanks.length() > 0) {
				b*=10;
				blanks = blanks.substring(1);
			}
		}
	}
	
	public static <E> ArrayList<E> toArrayList(E[] arr) {
		if(arr == null) return null;
		ArrayList<E> list = new ArrayList<E>();
		for(E e : arr) {
			list.add(e);
		}
		return list;
	}
	
	

	public static ArrayList<Long> toArrayList(long[] arr) {
		if(arr == null) return null;
		ArrayList<Long> list = new ArrayList<Long>();
		for(long e : arr) {
			list.add(e);
		}
		return list;
	}
	
	public static void print(long[] arr, String lineEnd)
	{
		print(toArrayList(arr),lineEnd);
	}
	
	public static <E> void print(E[] arr, String lineEnd)
	{
		print(toArrayList(arr),lineEnd);
	}
	
	public static char[] combine(char[] ... arrays) {
		int length = 0;
		for(char[] array : arrays) {
			length += array.length;
		}
		char[] ret = new char[length];
		int i = 0;
		for(char[] array : arrays) {
			for(char c : array) {
				ret[i++] = c;
			}
		}
		return ret;
		
	}

	
	public static int[] castInt(String[] input) {
		if(input == null) return null;
		int[] ret = new int[input.length];
		for(int i = 0; i < input.length; i++) {
			ret[i] = Integer.parseInt(input[i].strip());
		}
		return ret;
	}
	
	public static long[] castLong(String[] input) {
		if(input == null) return null;
		long[] ret = new long[input.length];
		for(int i = 0; i < input.length; i++) {
			ret[i] = Long.parseLong(input[i].strip());
		}
		return ret;
	}
	
	public static void printMatrix(char[][] matrix) {
		String header = "##";
		String space = "   ";
		for(int i = 0;  matrix.length > 0 && i < matrix[0].length; i++) {
			header += "#";
		}
		System.out.println(space + header);
		for(int i = 0; i < matrix.length; i++) {
			System.out.print(space + "#");
			for(int j = 0; j < matrix[i].length; j++) {
				System.out.print(matrix[i][j]);
			}
			System.out.println("#");
		}
		System.out.println(space + header);
	}

	public static int[] toArray(ArrayList<Integer> patternToType) {
		if(patternToType == null) return null;
		int i = 0;
		int[] ret = new int[patternToType.size()];
		for(Integer n : patternToType) {
			ret[i++] = n;
		}
		return ret;
	}
}
