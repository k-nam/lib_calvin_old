package bean;

import java.sql.Timestamp;
import java.util.Date;

public class DailyStatus {
	//public DailyStatus(Date date, Timestamp recordedTime, int mood, int medication) {
	public DailyStatus(long date, long recordedTime) {
		this.date = date;
		this.recordedTime = recordedTime;
		//this.mood = mood;
		//this.medication = medication;
		
	}

	
	public long date;
	public long recordedTime;
	//private int mood;
	//private int medication;
}
