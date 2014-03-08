using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Collections;

// typedef string MAC_Add
using MACAdd = System.String;
// typedef short SigStr
using SigStren = System.Double;



namespace Server
{
  class Program
  {
    static void Main(string[] args)
    {
      // Data gathered by service provider or users
      MeasurePointTable dataTable = new MeasurePointTable();
      // Measure data created by users to calculate position
      MeasurePointTable clientTable = new MeasurePointTable();
      dataTable.readFromFile("dataset.txt");
      dataTable.readFromFile("20080407.txt");
      dataTable.readFromFile("20080411.txt");
      clientTable.readFromFile("test.txt");
      APTable apTable = new APTable(dataTable);
      apTable.convertFromMeasurePointTable();
      // Test performance
      apTable.test(clientTable);
      apTable.printToFile("ap_position.txt");
      //apTable.print();
      
      MACAdd add302_105 = "001AC13844AE";
      GPSCoordi gps302_105 = new GPSCoordi(126.9525194, 37.4489);
      /*
      if (apTable.hasAP(add302_105))
      {
        AP ap302_105 = apTable.getAP(add302_105);
        ap302_105.print();
        for (int i = 0; i < ap302_105.size(); i++)
        {
          MeasurePointKey key = ap302_105.getRelatedMeasurePointKey(i);
          MeasurePoint point = dataTable.getMeasurePoint(key);
          SigStren signal = point.getSignal(add302_105);
          double dist = Func.getMeter(
            gps302_105, point.getKey().getGPSCoordi());
          Console.WriteLine("Distance: " + dist + 
            " signal: " + signal);
        }
      }*/
      
    }
  }



  /***************** Data Structure classes ********************/

  class Date
  {
    public Date(short inYear, byte inMonth, byte inDay, byte inHour,
      byte inMin, byte inSec)
    {
      year = inYear;
      month = inMonth;
      day = inDay;
      hour = inHour;
      min = inMin;
      sec = inSec;
    }
    private short year;    // four digit (e,g 2008)
    private byte month;
    private byte day;
    private byte hour;
    private byte min;
    private byte sec;
  }

  class GPSCoordi
  {
    public GPSCoordi(double inLon, double inLat)
    {
      longitude = inLon;
      latitude = inLat;
    }
    public double getLon()
    {
      return longitude;
    }
    public double getLat()
    {
      return latitude;
    }
    // convert (degree, min, sec) to degree with fraction
    private double longitude;
    private double latitude;
  }

  // pair <GPSCooordi, SigStren>
  class PointSignalPair
  {
    public PointSignalPair(GPSCoordi inGPS, SigStren inStren)
    {
      gpsCoordi = inGPS;
      sigStren = inStren;
    }
    public GPSCoordi getGPSCoordi()
    {
      return gpsCoordi;
    }
    public SigStren getSigStren()
    {
      return sigStren;
    }
    private GPSCoordi gpsCoordi;
    private SigStren sigStren;
  }

  class AP
  {
    public AP(MACAdd inMacAdd)
    {
      macAdd = inMacAdd;
      measurePointKeyList = new List<MeasurePointKey>();
    }
    // Returns the number of associated data points
    public int size()
    {
      return measurePointKeyList.Count;
    }
    public MACAdd getMacAdd()
    {
      return macAdd;
    }
    public GPSCoordi getGPSCoordi()
    {
      return gpsCoordi;
    }
    // Check for redundancy
    public void addMeasurePoint(MeasurePoint newMeasurePoint)
    {
      if (measurePointKeyList.Contains(newMeasurePoint.getKey()))
        return;
      measurePointKeyList.Add(newMeasurePoint.getKey());
    }
    public MeasurePointKey getRelatedMeasurePointKey(int index)
    {
      return measurePointKeyList[index];
    }
    // Recalculate GPS coordinate with associated data points
    public void calculatePosition(MeasurePointTable measurePointTable)
    {
      // make an array of PointSignalPair and call subroutine
      List<PointSignalPair> pointList = new List<PointSignalPair>();
      for (int i = 0; i < measurePointKeyList.Count; i++)
      {
        MeasurePointKey key = measurePointKeyList[i];
        if (!measurePointTable.hasMeasurePoint(key))
        {
          Console.WriteLine("CalculatePosition error: no related" +
            " data point in table\n");
          return;
        }
        MeasurePoint measurePoint = measurePointTable.getMeasurePoint(key);
        SigStren sigStren = measurePoint.getSignal(this.macAdd);
        if (sigStren > 0)
        {
          Console.WriteLine("CalculatePosition error: no related" +
            " data pair in data point\n");
          return;
        }
        pointList.Add(new PointSignalPair(
          key.getGPSCoordi(), sigStren));
      }
      this.gpsCoordi = Func.calculatePosition(pointList);
      //Console.WriteLine("Calculated my position\n");
    }
    // Inspect data
    public void print()
    {
      Console.WriteLine("MAC address is " + macAdd);
      Console.WriteLine("Num of related data points are " + 
        measurePointKeyList.Count);
    }
    private MACAdd macAdd;
    private GPSCoordi gpsCoordi;
    private List<MeasurePointKey> measurePointKeyList;
    // The degree of consistency and arruracy of its GPS coordinate
    private int reliability; // need to specify range later
  }

  // Dictionary: MACAdd --> AP
  class APTable
  {
    // maps MAC address to the index of AP in AP_List
    public APTable(MeasurePointTable inTable)
    {
      measurePointTable = inTable;
      macToAp = new Hashtable();
      apList = new List<AP>();
    }
    // Convert measured data set into this AP data set
    public void convertFromMeasurePointTable()
    {
      int size = measurePointTable.size();
      Console.WriteLine("MeasurePointTable size is " + size);
      for (int i = 0; i < size; i++)
      {
        //Console.WriteLine("Adding data");
        MeasurePoint curPoint = measurePointTable.getMeasurePoint(i);
        int numPair = curPoint.size();
        for (int j = 0; j < numPair; j++)
        {
          MeasurePair pair = curPoint.getDataPair(j);
          MACAdd macAdd = pair.getMacAdd();
          SigStren sigStren = pair.getSigStren();
          AP ap = new AP(macAdd);
          insertAp(ap); // don't care if it exists or not
          ap = getAP(macAdd); // get relavant AP
          ap.addMeasurePoint(curPoint);
          //Console.WriteLine("Adding data");
        }
      }
      // Calculate position of all APs
      for (int i = 0; i < apList.Count; ++i)
      {
        apList[i].calculatePosition(measurePointTable);
      }
      Console.WriteLine("Total # of APs is: " + this.apList.Count);
    }
    // Get client's position
    public GPSCoordi getPosition(List<MeasurePair> measurePairList)
    {
      // Get each MAC address's AP and their GPS
      List<PointSignalPair> signalPointPairList = 
        new List<PointSignalPair>();
      for (int i = 0; i < measurePairList.Count; i++)
      {
        MeasurePair pair = measurePairList[i];
        MACAdd macAdd = pair.getMacAdd();
        SigStren sigStren = pair.getSigStren();
        // Be careful; nondetermined APs should be excluded
        if (this.hasAP(macAdd)) // skip nonexistent APs
        {
          AP ap = this.getAP(macAdd);
          GPSCoordi gps = ap.getGPSCoordi();
          if (gps.getLat() != -1) // exclude this AP
          {
            PointSignalPair newPair =
              new PointSignalPair(gps, sigStren);
            signalPointPairList.Add(newPair);
          }
        }
      }
      //Console.WriteLine("num data is " + signalPointPairList.Count);
      return Func.calculatePosition(signalPointPairList);
    }

    // Test each MeasurePoint in the table and compare results
    public void test(MeasurePointTable inTable)
    {
      List<double> errorList = new List<double>();
      for (int i = 0; i < inTable.size(); i++)
      {
        MeasurePoint point = inTable.getMeasurePoint(i);
        List<MeasurePair> measurePairList = point.getMeasurePairList();
        GPSCoordi estimated = this.getPosition(measurePairList);
        GPSCoordi real = point.getKey().getGPSCoordi();
        double error = Func.getMeter(estimated, real);
        errorList.Add(error);
        Console.WriteLine(i + " th error is " + error);
      }
      double avg = 0, deviation = 0;
      Func.getStatistics(errorList, ref avg, ref deviation);
      Console.WriteLine("AVG is " + avg + " standard deviation is " +
        deviation);
    }

    // Inspect AP data
    public void print()
    {
      int size = apList.Count;
      for (int i = 0; i < size; i++)
      {
        apList[i].print();
      }
    }
    // Write the GPS coordinates of all APs to a text file
    public void printToFile(string fileName)
    {
      TextWriter writer = new StreamWriter(fileName);
      for (int i = 0; i < this.apList.Count; i++)
      {
        AP ap = this.apList[i];
        if (ap.getGPSCoordi().getLat() > 0)
        {
          writer.WriteLine("MAC:" + ap.getMacAdd());
          writer.WriteLine("Lat:" + ap.getGPSCoordi().getLat() + 
            " Lon:" + ap.getGPSCoordi().getLon());
          writer.WriteLine("EOD");
        }
      }
      writer.Close();
    }
    public bool hasAP(MACAdd inMacAdd)
    {
      if (macToAp.ContainsKey(inMacAdd))
        return true;
      return false;
    }
    // Assuming that given MAC address exists!
    public AP getAP(MACAdd inMacAdd)
    {
      return (apList[(int)macToAp[inMacAdd]]);
    }
    private bool insertAp(AP newAP)
    {
      if (!macToAp.ContainsKey(newAP.getMacAdd())) // OK, new AP
      {
        // hash mac address to the index of this AP int apList
        macToAp.Add(newAP.getMacAdd(), apList.Count);
        apList.Add(newAP);
        return true;
      }
      else // No, existing AP
      {
        return false;
      }
    }
    private MeasurePointTable measurePointTable; // related table
    private Hashtable macToAp;
    private List<AP> apList;
  }

  // pair <MACAdd, SignalStren>
  class MeasurePair
  {
    public MeasurePair(MACAdd inMacAdd, SigStren inStren)
    {
      macAdd = inMacAdd;
      sigStren = inStren;
    }
    public MACAdd getMacAdd()
    {
      return macAdd;
    }
    public SigStren getSigStren()
    {
      return sigStren;
    }
    private MACAdd macAdd;
    private SigStren sigStren;
  }

  class MeasurePointKey
  {
    public MeasurePointKey(Date inDate, GPSCoordi inCoordi)
    {
      date = inDate;
      gpsCoordi = inCoordi;
    }
    public GPSCoordi getGPSCoordi()
    {
      return gpsCoordi;
    }
    private Date date;
    private GPSCoordi gpsCoordi;
  }

  // instance of measuring
  class MeasurePoint
  {
    public MeasurePoint()
    {
    }
    public MeasurePoint(MeasurePointKey inKey)
    {
      key = inKey;
      measurePairList = new List<MeasurePair>();
    }
    public MeasurePointKey getKey()
    {
      return key;
    }
    public int size()
    {
      return measurePairList.Count;
    }
    public List<MeasurePair> getMeasurePairList()
    {
      return this.measurePairList;
    }
    public MeasurePair getDataPair(int index)
    {
      return measurePairList[index];
    }
    // Check for redundancy
    public void addMeasurePair(MeasurePair newPair)
    {
      if (!measurePairList.Contains(newPair))
        measurePairList.Add(newPair);
    }
    // Return signal strength for given MAC address
    public SigStren getSignal(MACAdd macAdd)
    {
      for (int i = 0; i < measurePairList.Count; i++)
      {
        if (measurePairList[i].getMacAdd() == macAdd)
          return measurePairList[i].getSigStren();
      }
      return 100;
    }
    private MeasurePointKey key;
    private List<MeasurePair> measurePairList;
  }

  // Dictionary: DatePointKey --> MeasurePoint
  class MeasurePointTable
  {
    // Initialize to empty table
    public MeasurePointTable()
    {
      keyToMeasurePoint = new Hashtable();
      measurePointList = new List<MeasurePoint>();
    }
    public int size()
    {
      return measurePointList.Count;
    }
    public bool hasMeasurePoint(MeasurePointKey key)
    {
      if (keyToMeasurePoint.ContainsKey(key))
        return true;
      else
        return false;
    }
    // Assuming valid index!
    public MeasurePoint getMeasurePoint(int index)
    {
      return measurePointList[index];
    }
    // Assuming there exists given key!
    public MeasurePoint getMeasurePoint(MeasurePointKey key)
    {
      int index = (int)(keyToMeasurePoint[key]);
      return measurePointList[index];
    }
    // Read measure data from a text file
    public bool readFromFile(string inputFileName)
    {
      TextReader reader = new StreamReader(inputFileName);
      string curLine;
      MeasurePoint curMeasurePoint = new MeasurePoint();
      // true if processing in a data point, false otherwise
      bool status = false;
      while (true)
      {
        curLine = reader.ReadLine();
        if (curLine == null) // end of file
        {
          break;
        }
        // Add data pair to current MeasurePoint
        if (curLine.StartsWith("MAC"))
        {
          if (!status)
          {
            Console.WriteLine("input data format error\n");
            return false;
          }
          try
          {
            curMeasurePoint.addMeasurePair(Func.parseMeasurePair(curLine));
          }
          catch (Exception)
          {
          }
          //Console.WriteLine("Added one line");
          continue;
        }
        // Create new MeasurePoint
        if (curLine.StartsWith("[")) // make new data point
        {
          if (status)
          {
            //Console.WriteLine("input data format error\n");
          }
          status = true; // going into curMeasurePoint
          // Move to next line to get GPS cooridnate
          MeasurePointKey key = new MeasurePointKey(Func.parseDate(curLine),
            Func.parseGPSCoordi(curLine = reader.ReadLine()));
          curMeasurePoint = new MeasurePoint(key);
          insertMeasurePoint(curMeasurePoint);
          continue;
        }

        if (curLine.StartsWith("EOD"))
        {
          status = false; // going out of curMeasurePoint
          continue;
        }

        continue; // ignore other cases
      }
      reader.Close();
      Console.WriteLine("# of extracted data: " + measurePointList.Count);
      return true;

           
    }
    // Add new data point 
    private bool insertMeasurePoint(MeasurePoint newData)
    {
      // OK, new data
      if (!keyToMeasurePoint.ContainsKey(newData.getKey()))
      {
        // hash the key to the index of the data in the array
        keyToMeasurePoint.Add(newData.getKey(), measurePointList.Count);
        measurePointList.Add(newData);
        return true;
      }
      else // No, already existing data
      {
        return false;
      }
    }        
    /*******************************
     ***** MeasurePointTable data *****
     *******************************/
    private Hashtable keyToMeasurePoint;
    private List<MeasurePoint> measurePointList;
  }

  static class Func
  {
    // Small step size in positioning algorithms: unit in degree
    // 0.0001 = 1.7m
    static double delta = 0.0001;
    // Large step size
    static double interval = 0.0003;
    // Parse MAC:00:00: .... Signal:-90
    static public MeasurePair parseMeasurePair(string inString)
    {
      MACAdd macAdd;
      SigStren signalStren;
      string temp = inString.ToString();
      temp = temp.Replace(":", ""); // remove ":"

      int macIndex = temp.IndexOf("MAC");

      int signalIndex = temp.IndexOf("Signal");

      string macAddString = temp.Substring(macIndex + 3, 12);
      //Console.WriteLine(temp);
      // Readthrough to the end of string
      string signalString = temp.Substring(signalIndex + 6,
        temp.Length - (signalIndex + 6));
      //Console.WriteLine("" + macIndex + " " + signalIndex);
      //macAdd = Int64.Parse(macAddString,
      //  System.Globalization.NumberStyles.HexNumber);
      macAdd = macAddString;
      signalStren = Int16.Parse(signalString);

      MeasurePair dataPair = new MeasurePair(macAdd, signalStren);
      return dataPair;
    }

    static public Date parseDate(string inString)
    {
      short year;
      byte month, day, hour, min, sec;
      string temp = inString.ToString();
      temp = temp.Replace("[", "");
      year = short.Parse(temp.Substring(0, 4));
      month = byte.Parse(temp.Substring(4, 2));
      day = byte.Parse(temp.Substring(6, 2));
      hour = byte.Parse(temp.Substring(8, 2));
      min = byte.Parse(temp.Substring(10, 2));
      sec = byte.Parse(temp.Substring(12, 2));
      Date date = new Date(year, month, day, hour, min, sec);
      return date;
    }

    static public GPSCoordi parseGPSCoordi(string inString)
    {
      double lon, lat;
      int latIndex = inString.IndexOf("Lat");
      int lonIndex = inString.IndexOf("Lon");
      lat = double.Parse(inString.Substring(latIndex + 4, 7));
      lon = double.Parse(inString.Substring(lonIndex + 4, 7));
      GPSCoordi coordi = new GPSCoordi(lon, lat);
      return coordi;
    }

    // Take two coordinates and returns the distance in meter
    static public double getMeter(GPSCoordi a, GPSCoordi b)
    {
      double meterPerDegree = 17666.66;
      double xDistance = (a.getLon() - b.getLon()) * meterPerDegree;
      double yDistance = (a.getLat() - b.getLat()) * meterPerDegree;
      double distance = Math.Sqrt(xDistance * xDistance + 
        yDistance * yDistance);
      return distance;
    }

    // Takes distance in meter and return estimated signal strength
    // This functions should be adjusted to meet our goal
    // 2008-04-27: Starting with 0.016(x-50)^2 - 100
    // 0 -> -60, 50 -> -100
    static public SigStren getSigStren(double x)
    {
      if (x > 50)
        return (SigStren)(-x - 50);
      return (SigStren)(0.016 * (x - 50) * (x - 50) - 100); 
      //return (-70 - x);
    }
    
    // Get error value for current estimation
    static public double getError(GPSCoordi curPosition, 
      List<PointSignalPair> pointList) 
    {
      double totalError = 0; // to be RMS
      for (int i = 0; i < pointList.Count; i++)
      {
        double distance = getMeter(pointList[i].getGPSCoordi(), 
          curPosition);
        SigStren estimated = getSigStren(distance);
        double error = Math.Pow (
          estimated - pointList[i].getSigStren(), 2);
        totalError += error;
      }
      totalError /= pointList.Count;
      totalError = Math.Sqrt(totalError);
      return totalError;
    }

    // Calculate GPS position from surrounding points
    static public GPSCoordi calculatePosition(
      List<PointSignalPair> pointList)
    {
      
      // Can not judge if too few PointSignalPair
      if (pointList.Count < 3)
      {
        //Console.WriteLine("Too few data: can not calculate poisition");
        return new GPSCoordi(-1, -1);
      }
      // Start with arithmetic avg and move to optimize
      double lon = 0, lat = 0;
      for (int i = 0; i < pointList.Count; i++)
      {     
        lon += pointList[i].getGPSCoordi().getLon();
        lat += pointList[i].getGPSCoordi().getLat();
      }
      lon /= pointList.Count;
      lat /= pointList.Count;
      // Delta degree per each time
      int count = 0;
      while (count++ < 100) // prevent infinite loop
      {
        //Console.WriteLine("count is " + count);
        double[] error; 
        error = new double[9];
        // Getting to local minimum by small step
        error[0] = Func.getError(new GPSCoordi(lon, lat), pointList);
        error[1] = Func.getError(
          new GPSCoordi(lon + delta, lat), pointList);
        error[2] = Func.getError(
          new GPSCoordi(lon - delta, lat), pointList);
        error[3] = Func.getError(
          new GPSCoordi(lon, lat + delta), pointList);
        error[4] = Func.getError(
          new GPSCoordi(lon, lat - delta), pointList);
        // Looking a little bit further to escape local optimum
        error[5] = Func.getError(
          new GPSCoordi(lon + interval, lat), pointList);
        error[6] = Func.getError(
          new GPSCoordi(lon - interval, lat), pointList);
        error[7] = Func.getError(
          new GPSCoordi(lon, lat + interval), pointList);
        error[8] = Func.getError(
          new GPSCoordi(lon, lat - interval), pointList);
        int minimum = 0;
        for (int i = 1; i < 9; ++i)
        {
          if (error[i] < error[minimum])
            minimum = i;
        }
        bool finished = false;
        switch (minimum)
        {
          case 0:
            finished = true;
            break;
          case 1:
            lon += delta;
            break;
          case 2:
            lon -= delta;
            break;
          case 3:
            lat += delta;
            break;
          case 4:
            lat -= delta;
            break;
          case 5:
            lon += interval;
            break;
          case 6:
            lon -= interval;
            break;
          case 7:
            lat += interval;
            break;
          case 8:
            lat -= interval;
            break;
        }
        if (finished)
          break;       
      }
      //Console.WriteLine("done");
      if (lon <= 0 || lat <= 0)
        Console.WriteLine("calcaultePosition internal error.");
      return new GPSCoordi(lon, lat);
    }
    
    // Calculate average and standard deviation 
    static public void getStatistics(List<double> inTable, ref double outAvg,
      ref double outDeviation)
    {
      double total = 0, avg, variation = 0;
      for (int i = 0; i < inTable.Count; i++)
      {
        total += inTable[i];
      }
      avg = total / inTable.Count;
      outAvg = avg;
      for (int i = 0; i < inTable.Count; i++)
      {
        variation += Math.Pow(inTable[i] - avg, 2);
      }
      variation /= inTable.Count; // avg of squares of deviation
      variation = Math.Sqrt(variation); // taking square root
      outDeviation = variation;
    }
  }
}