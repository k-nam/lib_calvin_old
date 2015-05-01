package com.excelsior.comfydrive;

import android.app.Activity;
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

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.home);
		mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
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
		float xValue = event.values[0];
		float yValue = event.values[1];
		float zValue = event.values[2];
		float xDelta = xValue - mPreviousX;
		float yDelta = xValue - mPreviousY;
		float zDelta = xValue - mPreviousZ;
		if (xDelta > threshold || -xDelta > threshold || yDelta > threshold || -yDelta > threshold || zDelta > threshold
				|| -zDelta > threshold) {
			TextView xAxis = (TextView) findViewById(R.id.x_axis_value);
			TextView yAxis = (TextView) findViewById(R.id.y_axis_value);
			TextView zAxis = (TextView) findViewById(R.id.z_axis_value);
			xAxis.setText(String.valueOf(xValue).substring(0, 4));
			yAxis.setText(String.valueOf(yValue).substring(0, 4));
			zAxis.setText(String.valueOf(zValue).substring(0, 4));
			mPreviousX = xValue;
			mPreviousY = yValue;
			mPreviousZ = zValue;
		}
	}
}
