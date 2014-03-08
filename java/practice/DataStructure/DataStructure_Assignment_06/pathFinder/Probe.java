/*
 * Probe.java
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
public class Probe {
	  public StationSystem mother;
	  public Station[] path;
	  public int numOfVisits;  // how many stations it has been to up to now.
	  public double totalTimeDist;
	  public int totalLineDist;
	  public Station destination;
	  
	  // if this probes exceeds this limit, it terminates
	  public double timeLimit;
	  public int lineLimit;
	  
	  public boolean prepareMode;
	  
	  
	  
	  
	  /** Creates a new instance of Probe */
	  public Probe (StationSystem mother1, Station departure, Station inputDestination
			  ,double inputTimeLimit, int inputLineLimit, boolean prepareMode1) {
		    mother=mother1;
		    //increase the num of probes in the stationsystem
		    mother.numOfTotalProbes++;
		    
		    path=new Station [Subway.MAX_NUM_OF_STATIONS];
		    path[0]=departure;
		    numOfVisits=1;
		    totalTimeDist=0;
		    totalLineDist=0;
		    destination=inputDestination;
		    
		    timeLimit=inputTimeLimit;
		    lineLimit=inputLineLimit;
		    
		    prepareMode=prepareMode1;
		    
		    this.explore();
	  }
	  
	  
	  // this constructor copies the original Probe to itself...
	  public Probe(Probe original){
		    mother=original.mother;
            // increase the num of probes in the stationsystem
		    mother.numOfTotalProbes++;
		    //if(mother.numOfTotalProbes>1000){
		    //	System.out.println("probe overlfow");
		   // }
		    
		    
		    numOfVisits=original.numOfVisits;
		    path=new Station [Subway.MAX_NUM_OF_STATIONS];
		    for(int i=0 ; i<numOfVisits ; i++){
		    	this.path[i]=original.path[i];
		    }
		    totalTimeDist=original.totalTimeDist;
		    totalLineDist=original.totalLineDist;
		    destination=original.destination;
		   
		    timeLimit=original.timeLimit;
		    lineLimit=original.lineLimit;
		    prepareMode=original.prepareMode;
	  }
	  
	  
	  // order this Probe to go to the nextStation
	  public void goTo(Station nextStation){
		    
		    // double checking the connection with the nextStation
		    Station curr=path[numOfVisits-1];
		    String nameOfNext=nextStation.getName ();
		    int count=-1;   // this is used to find the index of the nextStation
		    for (int i = 0; i < curr.getNumOfLinkedStations (); i++) {
			      if (curr.getStation (i).getName ().equals (nameOfNext) ){
					count=i;
			      }
			      
		    }
		    if (count==-1){
			      System.out.println("Internal ERROR in Probe.goTo.1");
			      return;
		    }
		    
		    path[numOfVisits]=nextStation;
		    numOfVisits++;
		    totalTimeDist+=curr.getTimeDist (count);
		    totalLineDist+=curr.getLineDist (count);
		    explore();
		    
	  }
	  
	  // default recursive behavior of a Probe. 
	  // look for all linked stations and send Probes to each of them.
	  public void explore(){
		  
		  //out of game if first move is a transfer
		  if(this.numOfVisits==2 &&
				 ( this.path[0].getLocation().equals(this.path[1].getLocation())==true)){
			  mother.numOfRuinedProbes++;
		      return;
		  }
		  
		  //out of game if timelimit has been exceeded
		  if(this.totalTimeDist>timeLimit){
			  mother.numOfRuinedProbes++;
		      return;
		  }
		  //ut of game if linelimit has been exceeded
		  if(this.totalLineDist>lineLimit){
			  mother.numOfRuinedProbes++;
		      return;
		  }
		  
		  Station curr;
		  curr=path[numOfVisits-1];
		   
		  //first check how the work is going in the mother systme.
		  // if this probe is late both in time and line, it is out of concern!
		  // changeed the code(2007 5 24): to enable multiple results.
		  // The objective is to show three best results to the user
		  // So, this probe will not be ruined unless there are three better record
		
		  
		  
		  
		  
		    if(mother.isOutOfGame(this)==true){
		    	mother.numOfRuinedProbes++;
		    	//System.out.println("Ruined");
		    	return;
		    }
		  
		    
		    
		    if(this.prepareMode==false){
               // if it has reached the destination...success and record its time
			    if (curr.getName ().equals (this.destination.getName ())){
			    	int temp=this.numOfVisits;
			    	
			    	// ruined if the last move is a transfer
			    	if(this.path[temp-2].getLocation().equals(this.path[temp-1].getLocation())){
			    		mother.numOfRuinedProbes++;
				    	return;
			    	}
				    
				      mother.recordFinishedProbe(this);
				      mother.numOfFinishedProbes++;
				      //System.out.println("Finished!!!");
				      return;
			    }
			    
		    }
		    
		    
		    
		   
		    
		    if(curr.isOutOfGame(this)==true){
		    	mother.numOfRuinedProbes++;
		    	return;
		    }
		    
		    curr.recordProbe(this);
		    
		    
		    
		    int numOfLink=curr.getNumOfLinkedStations ();
		    int last=-1;
		    for (int i = 0; i < numOfLink ; i++) {
			      Station temp;
			      temp=curr.getStation (i);
			      if ( this.isVisited (temp)==false){
				   last=i;	 // So if last==-1, there is no station to go.
			      }
		    }
		    
		    // end the probe if there is nowhere new to go (a deadlock)
		    // it is ruined...
		    if (last==-1){
		    	mother.numOfRuinedProbes+=1;
		    	return;
		    }
		    for (int i = 0; i < numOfLink ; i++) {
			      Station temp;
			      temp=curr.getStation (i);
			      if ( this.isVisited (temp)==false){
				    if (i!=last){
					      Probe newProbe;
					      newProbe=new Probe(this);
					      newProbe.goTo (temp);
				    }	
				    if(i==last){
					      this.goTo (temp);
				    }
			      }
		    }
		    
		    
	  }
	  
	  
	  // find out whether a station was visited in the past.
	  private boolean isVisited (Station input){
		  
		    for (int i = 0; i < numOfVisits-1; i++) {
			      if ( input.getLocation().equals (path[i].getLocation ())){
					return(true);
			      }
		    }
		    return(false);
	  }
	  
	  
	  //print its path
	  public void printPath(){
		  for(int i=0 ; i < numOfVisits ;i++){
			  
			  Station tempStation1;
			  tempStation1=this.path[i];
			  
			  if(i==numOfVisits-1){
				  System.out.print(tempStation1.getLocation()+" ");
				 
				  continue;
			  }			  
			  
			  Station tempStation2;			  
			  tempStation2=this.path[i+1];
			  
			  //if it is a transfer...
			  if(tempStation1.getLine().equals(tempStation2.getLine())==false){
				  i++;
				  System.out.print("["+tempStation1.getLocation()+"] ");
			  }
			  else{
				  System.out.print(tempStation1.getLocation()+" ");
			  }
			  
			  
			  //System.out.printf(this.path[i].getLocation()+" ");
			  
		  }
		  
		  System.out.println("");
		  System.out.print(this.totalTimeDist+"  ");
		  System.out.println("(#of transfer: "+this.totalLineDist+")");
		  
		  
		  
	  }
	  
	  
}











