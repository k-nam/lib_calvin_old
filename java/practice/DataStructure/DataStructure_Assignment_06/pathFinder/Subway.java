/*
 * Main.java
 *
 * Created on April 20, 2007, 8:40 PM
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

package pathFinder;

import java.io.*;
/**
 *
 * @author Calvin Caulfield
 */
public class Subway {
	  
	  // setting some constants; these can be changed when needed
	  
	  // time needed for one to change the line
	  public static final double TIME_TO_CHANGE_LINE=5;
	  
	  public static final int MAX_NUM_OF_STATIONS=1000;
	  
	  // because there exist triple interchange in Seoul subway, this value must be at least 5
	  public static final int MAX_NUM_OF_LINKED_STATIONS=10;
	  
	  // the number of total probes...
	  public static final int MAX_NUM_OF_PROBES=5;
	  
	  // the number of probes passing throug a station
	  public static final int MAX_NUM_OF_PASS=1;
	  
	  /** Creates a new instance of Main */
	  public Subway () {
	  }
	  
	  static public void main(String args[]) throws IOException{
		  
		    StationSystem a;
		    a=new StationSystem(MAX_NUM_OF_STATIONS);
		    
		    
		  
		    // file input processing....    
		    File file1;
		    // if the argument in not specified, end the program
		    try{		    
		    	file1=new File (args[0]);
		    }catch(ArrayIndexOutOfBoundsException e){
		    	System.out.println("You must enter an Input file name");
		    	return;
		    }
		    
		    if (file1==null || !(file1.exists ())  ){
			      System.out.println("File name "+args[0]+" does not exist.");
			      return;
		    }
		    
		    FileReader fileReader1;
	
	        fileReader1=new FileReader(file1);
		
		    BufferedReader bufferedReader1;
		    bufferedReader1=new BufferedReader(fileReader1);
		    
		    
		    // make each station
		  int status=0; 
		  while ( status==0  ){
			   String temp;
			   temp=bufferedReader1.readLine ();
			   if (temp.equals ("")){
				     status=1;
				     continue;
			   }
			    a.addStation (temp);   
		  }
		  
		  
		  
		  if(a.inspectInput()==-1){
			  return;
		  }
		  
		  
		  // link the interchange stations...
		  if(a.findSameLocation ()==-1){
			  return;
		  }
		  
		  
		  
		  // link the stations on the same line
		  while (status==1){
			    String temp;
			    temp=bufferedReader1.readLine ();
			    if(temp==null){
				      status=2;
				      continue;
			    }
			    if(temp.equals("")){
			    	continue;
			    }
			    
			    int i=a.linkStation (temp);
			    if(i==-1){
			    	return;
			    }
		  }
		    
		  //close file
		  fileReader1.close();
		  
		  
		 //a.print1 ();
		  
		 File file2;
		 try{	 
			 file2=new File(args[1]);
		 }catch(Exception e){
			 System.out.println("Command file name invalid");
			 return;
		 }
		 
		 if (file2==null || !(file2.exists ())  ){
		      System.out.println("File name "+args[1]+" does not exist.");
		      return;
	    }
		 FileReader fileReader2;
		 fileReader2=new FileReader(file2);
		 
		 
		  
		  BufferedReader b=new BufferedReader(fileReader2);
		  
		  int status1=0; 
		  
		  
		  
		  do{
			  
			  boolean leastExchange=false;    // option -ex
			  boolean multipleShow=false;    //option -m
			  
			  
			  String input0=b.readLine();
			  if(input0==null){
				  
				  status1=-1;
				  continue;
			  }
			  if(input0.equalsIgnoreCase("")){
				  continue;
			  }
			  String[] c;
			  c=input0.split("(\\s)+");
			  
			  if (c[0].equals("")){
				  for(int i=1; i<c.length ; i++){
					  c[i-1]=c[i];
				  }				
			  }
			  
			  int temp=-1;
			  for(int i=2 ; i<c.length ; i++){
				  if( c[i].equalsIgnoreCase("-ex")){
					  leastExchange=true;
				  }
				  if( c[i].equals("-m")==true){
					  multipleShow=true;
					  temp=i;
				  }
				  
			  }
			  
			  int num=Subway.MAX_NUM_OF_PROBES;
			  /*
			  if(multipleShow==true){
				  try{
					  String tempString=c[temp].substring(2);
					  Integer num1;
					  num1=new Integer(tempString);
					  num=num1;
				  }
				  catch(Exception e){
					  num=3;
				  }
				 
				 
			  }*/
			  
			  
			 
		
			  switch (a.findPath(c[0], c[1])){
				  case -1: {
					  System.out.println("The input station names does not exist.\n");
					  break;
				  }
				  case -2:{
					  System.out.println("There is no possible path between the input stations "
							  +c[0]+" and "+c[1]+"\n");
					  break;
				  }
				  case 0: {
					  if(leastExchange==true){
						  if(multipleShow==true){
								 
							  if(Subway.MAX_NUM_OF_PROBES<num){
								  num=Subway.MAX_NUM_OF_PROBES;
							  }
							  if(a.numOfFinishedProbes<num){
								  num=a.numOfFinishedProbes;
							  }
							  System.out.printf("(LX) ");
							  for(int i=0 ; i < num; i++){
								  System.out.printf(i+1+"'th: ");
								  a.finishedProbes_2[i].printPath();
							  }
							  System.out.println("");
						  }
						  else{
							  System.out.printf("(LX) ");
							  a.finishedProbes_2[0].printPath();
							  System.out.println("");
						  }
					  }
					  
					  
					  else{
						  if(multipleShow==true){
								 
							  if(a.numOfFinishedProbes<num){
								  num=a.numOfFinishedProbes;
							  }
							  if(a.numOfFinishedProbes<num){
								  num=a.numOfFinishedProbes;
							  }
							  
							  
							  for(int i=0 ; i < num; i++){
								  System.out.printf(i+1+"'th: ");
								  a.finishedProbes_1[i].printPath();
							  }
							  System.out.println("");
						  }
						  else{
							  a.finishedProbes_1[0].printPath();
							  System.out.println("");
						  }
					  }
					  
					  
						 
					  
				  }
				  
			  }
			  
			  a.initialize();	  
			  
			// System.out.println("total "+a.numOfTotalProbes);
			 // System.out.println("ruined "+a.numOfRuinedProbes);
			 // System.out.println("finished "+a.numOfFinishedProbes);
			  //a.print2();
			 
		  }
		  while(status1==0);		  
		  
		  
		    
	  }
	  
}






























