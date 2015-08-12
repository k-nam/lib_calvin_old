package bean;

import java.sql.Timestamp;
import java.util.Date;

public class DailyStatus {
	public DailyStatus(long date, long recordedTime, int mood, int medication) {
		this.date = date;
		this.recordedTime = recordedTime;
		this.mood = mood;
		this.medication = medication;
	}

	public long date;
	public long recordedTime;
	public int mood;
	public int medication;
}
