package patternMatching;

import java.io.*;
import java.util.*;

public class Matching {
	
	public final static int SIZE_OF_MATCHING_TOKEN=6;
	
	
	
	public static void main(String[] args){
		//The length of the longest input String. Needed for the method positionFinder()
		int maxLengthOfInputString=0;
		int inputLineNumber=0;
		int patternNumber=0;
	
		HashTable table;
		table=new HashTable();
		
		if(args.length!=3){
			System.out.println("Wrong command arguments; 3 argument needed");
			return;
		}
		
		File inputFile, patternFile, outputFile;
		inputFile=new File(args[0]);
		patternFile=new File(args[1]);
		outputFile=new File(args[2]);
		
		
		//First fine out the number of lines in the data file
		Scanner inputFileScanner;
		try {
			inputFileScanner=new Scanner(inputFile);
			
		} catch (FileNotFoundException e) {
			System.out.println("The first argument Datafile can not be found");
			return;
		}
				
		while(inputFileScanner.hasNextLine()==true){
			inputLineNumber++;
			inputFileScanner.nextLine();
		}
		inputFileScanner.close();
		
		// Find out the number of lines in the pattern file
		Scanner patternFileScanner;
		try{
			patternFileScanner=new Scanner(patternFile);
		} catch(Exception e){
			System.out.println("The second argument Patternfile can not be found");
			return;
		}
		
		while(patternFileScanner.hasNextLine()==true){
			patternNumber++;
			patternFileScanner.nextLine();
		}
		patternFileScanner.close();
		
		
		
		
		
		
		
		//Get each line as a string
		
		String[] dataArray;
		dataArray=new String[inputLineNumber];
		try {
			inputFileScanner=new Scanner(inputFile);
		} catch (FileNotFoundException e) {
			return;
		}
		
		for(int i=0 ; i < inputLineNumber; i++){
			dataArray[i]=inputFileScanner.nextLine();
		}
		
		
		//Get each line of pattern
		
		String[] patternArray;
		patternArray=new String[patternNumber];
		try {
			patternFileScanner=new Scanner(patternFile);
		} catch (FileNotFoundException e) {
			return;
		}
		
		for(int i=0 ; i < patternNumber; i++){
			patternArray[i]=patternFileScanner.nextLine();
		}
		
		
		
		
		//System.out.println(inputLineNumber);
		
		
		/*
		String[] dataArray;
		dataArray=new String[3];
		inputLineNumber=dataArray.length;
		
		dataArray[0]="I am a boy and you are a girl";
		dataArray[1]="I am a boy and and you are are a girl are a girl";
		dataArray[2]="and you I am a boy boy and I am a";
		
		String [] patternArray;
		patternArray=new String[3];
		patternArray[0]="I am a ";
		patternArray[1]="and you";
		patternArray[2]=" a boy boy and";
		*/
		
		
		
		
		
		// inserting process
		for ( int i=0 ; i <dataArray.length ; i++){
			PieceOfString[] temp;
			if( dataArray[i].length()>maxLengthOfInputString){
				maxLengthOfInputString=dataArray[i].length();
			}
			temp=Matching.stringTokenizer(dataArray[i], i);
			for ( PieceOfString element:temp){
				table.insert(element);
			}
		}
		
		
		
		
		//write to the output File
		
		PrintWriter writer;
		FileWriter a;
		try {
			a=new FileWriter(outputFile);
		} catch (IOException e1) {
			System.out.println("The outputFile by the name "+args[2]+" " +
					"could not be made");
			return;
		}
	
		writer=new PrintWriter(a);
		
		
		for (int i=0 ; i <patternArray.length ; i++){
			ListOfPieceOfString[] temp;
			temp=Matching.patternFinder(patternArray[i], table);
			int[][]  tmp;
			tmp=Matching.positionFinder(temp, inputLineNumber, maxLengthOfInputString);
			for ( int j=0 ; j < tmp.length ; j++){
				writer.print("("+(tmp[j][0]+1)+","+(tmp[j][1]+1)+")   ");
			}
			if (tmp.length==0){
				writer.print("(0,0)");
			}
			writer.println("");
		}
		
		writer.close();
		
		
		
		
	}
	
	
	
	
	
	
	
	public static PieceOfString[] stringTokenizer(String inputString, int lineNumber){
		
		int n=inputString.length();
		int k=SIZE_OF_MATCHING_TOKEN;
		if(n<k){
			System.out.println("Illegal input; every input string's length must be equal to" +
					"or greater than 5");
			return(null);
		}
		int numOfToken=n-k+1;
		PieceOfString[] outTokens;
		outTokens=new PieceOfString[numOfToken];
		for (int i = 0; i < numOfToken; i++) {
			String temp;
			temp=new String(inputString.substring(i, i+k));
			int[] a;
			a=new int[2];
			a[0]=lineNumber;
			a[1]=i;
			outTokens[i]=new PieceOfString(temp, a);
			
		}
		return(outTokens);
		
	}
	
	
	
	
	
	
	
	
	public static ListOfPieceOfString[] patternFinder(String pattern, HashTable table){
		int n=pattern.length();
		int k=SIZE_OF_MATCHING_TOKEN;
		if(n<k){
			System.out.println("Illegal input; every input string's length must be equal to" +
					"or greater than 5");
			return(null);
		}
		int numOfToken=n-k+1;
		ListOfPieceOfString[] outTokens;
		outTokens=new ListOfPieceOfString[numOfToken];
		for (int i = 0; i < numOfToken; i++) {
			String temp;
			temp=new String(pattern.substring(i, i+k));
			outTokens[i]=	table.retrieve(temp);
			
		}	
		
		
		return(outTokens);
		
	}
	
	
	
	
	
	
	
	public static int[][] positionFinder (ListOfPieceOfString[] result, int lineNum, int maxLength){
		int length=result.length;
		
		int[][] matchList;
		matchList=new int[lineNum*maxLength][2];
		int counter=0;
		
		for(int i=0 ; i < lineNum ; i++){
			for (int j=0 ; j < maxLength ; j++){
				
				boolean temp=true;
				for ( int k =0 ; k < length ; k++){
					int[] c;
					c=new int[2];
					c[0]=i;
					c[1]=j+k;
					if (result[k]==null||result[k].isThere(c)==false){
						temp=false;
						break;
					}
				}
				if(temp==true){
					int[] c;
					c=new int[2];
					c[0]=i;
					c[1]=j;
					matchList[counter]=c;
					counter++;
				}
				
			}
		}
		int[][] out;
		out = new int[counter][2] ;
		for(int i=0 ; i < counter ; i++){
			out[i]=matchList[i];
		}
		
		return(out);
	}
	
	
	
	
	
	
	
	

}














