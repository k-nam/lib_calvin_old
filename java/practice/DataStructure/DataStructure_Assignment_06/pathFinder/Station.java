/*
 * Station.java
 *
 * Created on April 20, 2007, 8:40 PM
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

package pathFinder;

/**
 *
 * @author Calvin Caulfield
 */
public class Station {
	  
	  
	  private String name;
	  //location is the usual name of the station ex) Sa-Dang, Sin_Do-Rim.
	  // several distinct Stations can have the same location (interchagne stations)
	  private String location;
	  private String line;
	  private int numOfLinkedStations;
	  
	  // these three are problematic. they may well be integrated into another data structure
	  // the indexes of each array must match well.
	  private Station[] nextStations;
	  private double[] timeDist;
	  private int[] lineDist;
	  
	  
	  
	  //time sorting
	  public Probe[] passedProbes_1;
	  //line sorting
	  public Probe[] passedProbes_2;
	  
	  int numOfPassedProbes;
	  
	  /** Creates a new instance of Station */
	  
	  public Station (){
		    
	  }
	  
	  public Station ( String inputName, String inputLocation, String inputLine) {
		    name=inputName;
		    location=inputLocation;
		    line=inputLine;
		    nextStations=new Station[Subway.MAX_NUM_OF_LINKED_STATIONS];
		    timeDist=new double[Subway.MAX_NUM_OF_LINKED_STATIONS];
		    lineDist=new int[Subway.MAX_NUM_OF_LINKED_STATIONS];
		    numOfLinkedStations=0;
		    
		    
		    numOfPassedProbes=0;
		    passedProbes_1=new Probe[Subway.MAX_NUM_OF_PASS];
		    passedProbes_2=new Probe[Subway.MAX_NUM_OF_PASS];
	  }
	  
	  public String getName(){
		    return (name);
	  }
	  
	  public String getLocation(){
		    return(location);
	  }
	  public String getLine(){
		    return(line);
	  }
	  public int getNumOfLinkedStations(){
		    return(numOfLinkedStations);
	  }
	  
	  
	  
	  
	  
	  public Station getStation(int index){
		    if (index>numOfLinkedStations){
			      System.out.println("Internal ERROR in Station.getStation");
			      return(null);
		    }
		    return(nextStations[index]);
	  }	  
	  public double getTimeDist(int index){
		    if (index>numOfLinkedStations){
			      System.out.println("Internal ERROR in Station.getTimeDist");
			      return(-1);
		    }
		    return(timeDist[index]);
	  }	  
	  public int getLineDist(int index){
		    if (index>numOfLinkedStations){
			      System.out.println("Internal ERROR in Station.getLineDist");
			      return(-1);
		    }
		    return(lineDist[index]);
	  }
	  
	  //do this after a certain path finding..
	  public void initialize(){
		  numOfPassedProbes=0;
		  passedProbes_1=new Probe[Subway.MAX_NUM_OF_PASS];
		  passedProbes_2=new Probe[Subway.MAX_NUM_OF_PASS];
	  }
	  
	  
	  
	  
	  
	  
	  public void setLink (Station inputStation, double inputTimeDist, int inputLineDist){
		    nextStations[numOfLinkedStations]=inputStation;
		    timeDist [numOfLinkedStations]=inputTimeDist;
		    lineDist[numOfLinkedStations]=inputLineDist;
		    numOfLinkedStations++;
		    
	  }
	  
	  
	  
	  
	  
	  
	  public void recordProbe(Probe passingProbe){
		  this.numOfPassedProbes++;
		  
		  for(int i=0 ; i <Subway.MAX_NUM_OF_PASS; i++ ){
			  if  (this.passedProbes_1[i]==null||
		     passingProbe.totalTimeDist<this.passedProbes_1[i].totalTimeDist){
				  for(int j=Subway.MAX_NUM_OF_PASS-1 ; j>i;j--){
					  this.passedProbes_1[j]=this.passedProbes_1[j-1];
				  }
				  this.passedProbes_1[i]=passingProbe;
				  break;
			  }
		  }
		  
		  for(int i=0 ; i <Subway.MAX_NUM_OF_PASS; i++ ){
			  if  (	this.passedProbes_2[i]==null|| 
					  ( passingProbe.totalLineDist<this.passedProbes_1[i].totalLineDist ||
							  (passingProbe.totalLineDist==this.passedProbes_1[i].totalLineDist &&
									  passingProbe.totalTimeDist<this.passedProbes_1[i].totalTimeDist)  )
		    ){
				  for(int j=Subway.MAX_NUM_OF_PASS-1 ; j>i;j--){
					  this.passedProbes_2[j]=this.passedProbes_1[j-1];
				  }
				  this.passedProbes_2[i]=passingProbe;
				  break;
			  }
		  }
		  
	  }
	  
	  //returns true if it is out of game already
	  public boolean isOutOfGame(Probe pathFinder){
		  if(this.numOfPassedProbes<Subway.MAX_NUM_OF_PASS){
			  return(false);
		  }
		  else{
			  if( ( pathFinder.totalTimeDist>
				  this.passedProbes_1[Subway.MAX_NUM_OF_PASS-1].totalTimeDist )&&
				  (pathFinder.totalLineDist>
				  this.passedProbes_2[Subway.MAX_NUM_OF_PASS-1].totalLineDist) )
			  {
						  
				  return(true);
			  }
			  else
				  return(false);
		  }
	  }
	  
	  
	    
	  
	  
	  
	  
	  
	  
	  
}



















