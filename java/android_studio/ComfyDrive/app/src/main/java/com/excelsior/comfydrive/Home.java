package com.excelsior.comfydrive;

import android.app.Activity;
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.widget.TextView;

public class Home extends Activity implements SensorEventListener {
	private SensorManager mSensorManager;
	private Sensor mAccelerometer;
	private float mPreviousX = 0;
	private float mPreviousY = 0;
	private float mPreviousZ = 0;
	private final float threshold = 0.5f;
	private Analyzer mAnalyzer = null;

	public Home() {
		mAnalyzer = new Analyzer();
	}

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.home);
		mSensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);
		mAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
	}

	protected void onResume() {
		super.onResume();
		mSensorManager.registerListener(this, mAccelerometer, SensorManager.SENSOR_DELAY_NORMAL);
	}

	protected void onPause() {
		super.onPause();
		mSensorManager.unregisterListener(this);
	}

	public void onAccuracyChanged(Sensor sensor, int accuracy) {
	}

	public void onSensorChanged(SensorEvent event) {
		mAnalyzer.refresh(event);
		TextView forwardAccel = (TextView)findViewById(R.id.forward_accel);
		TextView updownAccel = (TextView)findViewById(R.id.updown_accel);
		TextView forwardShock = (TextView)findViewById(R.id.forward_shock);
		TextView updownShock = (TextView)findViewById(R.id.updown_shock);
		setValue(forwardAccel, mAnalyzer.getForwardAccel());
		setValue(updownAccel, mAnalyzer.getUpdownAccel());
		setValue(forwardShock, mAnalyzer.getForwardShock());
		setValue(updownShock, mAnalyzer.getUpdownShock());
	}

	private void setValue(TextView view, float value) {
		view.setText(String.format("%1$,.2f", value));
		if (value < 0.3) {
			view.setBackgroundColor(Color.GREEN);
		} else if (value < 2) {
			view.setBackgroundColor(Color.YELLOW);
		} else if (value < 5) {
			view.setBackgroundColor(Color.MAGENTA);
		} else {
			view.setBackgroundColor(Color.RED);
		}
	}
}
