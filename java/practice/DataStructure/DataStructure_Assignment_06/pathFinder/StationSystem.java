package pathFinder;
/*
 * Starting to work on April 20th Friday, 2007
 * This is the last assignment of Data Structure but I feel interested and just started on it.

package PathFinder;

  Finished on May 24th. Added some features(multiple option showing).

/**
 *
 * @author Calvin Caulfield
 */
public class StationSystem {
	  
	  
	  
	  
	  // array of all the stations in this StationSystem
	  public Station[] stations;
	  public int numOfStations;
	  
	  
	  // used to control probes
	  // Number of total and finished probes are important, because the algorithm should
	  // not be considered complete before all the probes are finished;
	  // That is, finish if and only if when finished+ruined==total probes
	  public int numOfTotalProbes;
	  public int numOfFinishedProbes;
	  public int numOfRuinedProbes;
	  
	
	  
	  //time sorting
	  public Probe[] finishedProbes_1;
	  //line sorting
	  public Probe[] finishedProbes_2;
	  
	  
	  /** Creates a new instance of StationSystem */
	  public StationSystem (int maxNumOfStations) {
		   
		    stations=new Station[maxNumOfStations];
		    
		    numOfStations=0;
		    numOfTotalProbes=0;
		    numOfFinishedProbes=0;
		    numOfRuinedProbes=0;
		    
	  }
	  
	  
	  
	  
	  
	  
	  
	 
	  
	  
	  // name is the unique identifier of a station; usually several bits of decimal number
	  // two different Stations can not have same name. It must be unique.
	  private Station findStation (String inputName){
		    for (int i = 0; i <numOfStations; i++) {
			    if( stations[i].getName ().equals (inputName)){
				      return (stations[i]);
			    }  
		    }
		    System.out.println("Internal ERROR in StationSystem.findStation");
		    return(null);
	  }
	  
	  
	  // inputs are: name, location, line
	  public int addStation(String inputString){
		    String[] a;
		    a=inputString.split ("\\s+");
		    if ( a.length<3 && a.length>5){
			    System.out.println("StationSystem.makeStation ERROR. Input is invalid");
			    return(1);
		    }
		    if (a[0].equals ("")){
			    for (int i = 0; i < a.length-1; i++) {
				 a[i]=a[i+1];     
			    }
		    }
		    
		    //Integer temp=new Integer(a[2]);
		    //int b=temp.intValue();
		    stations[numOfStations]=new Station(a[0], a[1], a[2]);
		    
		    
		  numOfStations++;
		  return(0);	
	  }
	  
	  
	  // inspects the input and see if there are two identical 'name' s in them
	  public int inspectInput (){
		  for ( int i = 0; i < numOfStations ; i++){
			 for (int j = 0; j < numOfStations; j++) {
				   if ( (i!=j) && stations[i].getName ().equals (stations[j].getName ())){
					     System.out.println("Input is invalid. There are two identical names in it");
					     return(-1);
				   }
			 }   
		  }
		  
		return(0);    
	  }
	  
	  
	  
	  // this method operates in the middle of the input text
	  // finds the 'interchange stations' and link them one to another'
	  // find a station with the same location and like them properly
	  // line dist is assumed to be 0 because they are on the same line
	  public int findSameLocation(){
		 
		    for (int i = 0; i < numOfStations; i++) {
			      for (int j = 0; j < numOfStations; j++) {
					if ( (i!=j) && stations[i].getLocation ().equals (stations[j].getLocation () )){
						  if (stations[i].getLine ()==stations[j].getLine ()){
							    System.out.println("Input is invalid. There are two stations"+
								   "with identical location and line");
							    return(-1);
						  }
						  stations[i].setLink (stations[j],Subway.TIME_TO_CHANGE_LINE, 1);
					}
			      }
		    }
		  return(0);
	  }
	  
	  
	  
	  
	  
	  //inputs are: name1, name2 , timeDist
	  // it means we can go FROM name1 TO name2 with timeDist minutes
	  // the two stations are assumed to be on the same line
	  public int linkStation(String inputString){
		    
		  String[] a;
		  a=inputString.split ("\\s+");
		  if ( a.length<3 || a.length>5){
			    System.out.println("StationSystem.makeStation ERROR. Input is invalid");
			    return(-1);
		  }
		  
		  if (a[0].equals ("")){
			    for (int i = 0; i < a.length-1; i++) {
				 a[i]=a[i+1];     
			    }
		  }
		  
		  Double temp=new Double(a[2]);
		  double b=temp;
		  
		  if (a[0].equals (a[1])){
			    System.out.println("StationSystem.linkStation.  Input Invalid. "
				   +" identical name was used in the input.");
			    return(-1);
		  }
		  Station temp0;
		  Station temp1;
		  temp0=this.findStation (a[0]);
		  temp1=this.findStation (a[1]);	
		  
		  if (temp0==null || temp1==null ){
			  System.out.println(a[0]+" and"+a[1]+" in the input is invalid...");
			  return(-1);
		  }
		  
		  
		  if ( temp0.getLine().equals(temp1.getLine ())==false  ){
			    System.out.println("StationSystem.linkStation. Input Invalid. Can not connect two "
				   +"stations on different lines...");
			    return(1);
		  }
		  
		  if ( temp0.getLocation ().equals (temp1.getLocation ()) ){
			    System.out.println("StationSystem.linkStation. Input Invalid. Can not connect two "
				   +"stations the same location...");
			    return(-1);
		  }
		  
		  temp0.setLink (temp1, b, 0);
		   return(0);
	  }
	  
	  
	  // prints out some info...about stations and its number of link
	  public void print1 (){
		   for (int i = 0 ; i < numOfStations ; i++){
			     System.out.println(stations[i].getName ()+"  "+stations[i].getLocation ()
			     +" "+stations[i].getLine ()+"\n"+stations[i].getNumOfLinkedStations ());
		   }
			 
	  }
	  
	  

	  
	  
	  // return 0 when finished normally
	  // -1 when input is wrong
	  // -2 when the path can not be found
	  public int findPath (String departure, String destination){
		    
		 
		    this.finishedProbes_1=new Probe[Subway.MAX_NUM_OF_PROBES];
		    this.finishedProbes_2=new Probe[Subway.MAX_NUM_OF_PROBES];
		    
		   
		    // if status==0, the input is invalid (there is not a station by the given name)
		    int status=0;
		    
		    
		    
		    // using a heuristic method here. It is very probable that an optimal solution
		    // contains only one or two exchanges.
		    for(int k=0 ; k<2 ; k++) {
		    	boolean prepareMode;
		    	if(k==0){
		    		prepareMode=true;
		    	}
		    	else{
		    		prepareMode=false;
		    	}
		    	
		    	//initialize record
		    	this.finishedProbes_1=new Probe[Subway.MAX_NUM_OF_PROBES];
			    this.finishedProbes_2=new Probe[Subway.MAX_NUM_OF_PROBES];
			    this.numOfFinishedProbes=0;
				this.numOfRuinedProbes=0;
				this.numOfTotalProbes=0;
			    
			    
		    	for (int i = 0; i < numOfStations; i++) {
				      if (stations[i].getLocation ().equals (departure)){
						for (int j = 0; j < numOfStations; j++) {
							   if (stations[j].getLocation ().equals (destination) ){
								    new Probe (this, stations[i], stations[j], 150,4,prepareMode );
								    status=1;
						      }
				        }
			         }
		         }
			    
			    if (status==0){
			    	return(-1);  // input does not match the existing stations...
			    }
			    
			    //Just wait if there are unfinished Probes.
			    if (this.numOfTotalProbes!=(this.numOfRuinedProbes+
			    		this.numOfFinishedProbes)  ){	
			    	System.out.println("Internal Error in StationSystem.findPath");
			    	
			    }
			    
			    
			   
		    }
		    
		    if (this.numOfFinishedProbes==0){
		    	return(-2); // the path can not be found at all
		    }
		    
		    
		    return(0); // path found
		    
	  }
	  
	  public void initialize (){
		  this.numOfFinishedProbes=0;
		  this.numOfRuinedProbes=0;
		  this.numOfTotalProbes=0;
		  for(int i=0 ;  i< this.numOfStations ; i++){
			  this.stations[i].initialize();
		  }
		  
	  }
	  
	  // keeps only the best probes that finished. (in order)
	  // so, if any probe's record gets beyond the 100'th record, it terminates.
	  public void recordFinishedProbe(Probe finishedProbe){
		  for(int i=0 ; i <Subway.MAX_NUM_OF_PROBES; i++ ){
			  if  (this.finishedProbes_1[i]==null||
		     finishedProbe.totalTimeDist<this.finishedProbes_1[i].totalTimeDist){
				  for(int j=Subway.MAX_NUM_OF_PROBES-1 ; j>i;j--){
					  this.finishedProbes_1[j]=this.finishedProbes_1[j-1];
				  }
				  this.finishedProbes_1[i]=finishedProbe;
				  break;
			  }
		  }
		  
		  for(int i=0 ; i <Subway.MAX_NUM_OF_PROBES; i++ ){
			  if  (	this.finishedProbes_2[i]==null|| 
		    ( finishedProbe.totalLineDist<this.finishedProbes_2[i].totalLineDist ||
		    (finishedProbe.totalLineDist==this.finishedProbes_2[i].totalLineDist &&
		    finishedProbe.totalTimeDist<this.finishedProbes_2[i].totalTimeDist)  )
		    ){
				  for(int j=Subway.MAX_NUM_OF_PROBES-1 ; j>i;j--){
					  this.finishedProbes_2[j]=this.finishedProbes_2[j-1];
				  }
				  this.finishedProbes_2[i]=finishedProbe;
				  break;
			  }
		  }
		  
	  }
	  
	  //returns true if it is out of game already
	  public boolean isOutOfGame(Probe pathFinder){
		  if(this.numOfFinishedProbes<Subway.MAX_NUM_OF_PROBES){
			  return(false);
		  }
		  else{
			  if( ( pathFinder.totalTimeDist>
				  this.finishedProbes_1[Subway.MAX_NUM_OF_PROBES-1].totalTimeDist )&&
				  (pathFinder.totalLineDist>
				  this.finishedProbes_2[Subway.MAX_NUM_OF_PROBES-1].totalLineDist) )
			  {
						  
				  return(true);
			  }
			  else
				  return(false);
		  }
	  }
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
	  
}

























