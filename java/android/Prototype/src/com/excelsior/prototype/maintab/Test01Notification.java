package com.excelsior.prototype.maintab;

import android.app.Activity;
import android.app.NotificationManager;
import android.os.Bundle;
import android.view.Menu;
import android.widget.Toast;

import com.excelsior.prototype.R;

public class Test01Notification extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.test_01_notification);
		NotificationManager nm = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
		Toast.makeText(this, Integer.toString(getIntent().getExtras().getInt("notificationId")), Toast.LENGTH_SHORT).show();
		nm.cancel(getIntent().getExtras().getInt("notificationId"));
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.test01_notification, menu);
		return true;
	}

}
