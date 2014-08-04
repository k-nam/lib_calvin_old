package com.excelsior.vividcamera;

import android.app.Activity;
import android.content.Intent;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class Main extends Activity {
	Button testCameraButton;
	Button openCameraButton;
	TextView cameraInfo;
	private int cameraIdToUse = 1;
	private int requestCodeForPreviewScreen = 0;
	private int requestCodeForReview = 1;
	public static String TAG = "Preview";
	public static String RESULT_FILE_PATH = "Result file path";

	public void onCreate(Bundle savedInstanceState) {
		Log.i(Main.TAG, "create 1");
		super.onCreate(savedInstanceState);
		Log.i(Main.TAG, "create 2");
		setContentView(R.layout.vividcamera__main);
		Log.i(Main.TAG, "create 3");
		testCameraButton = (Button) findViewById(R.id.vivid_camera__test_camera_button);
		Log.i(Main.TAG, "create 4");
		openCameraButton = (Button) findViewById(R.id.vivid_camera__open_camera_button);
		Log.i(Main.TAG, "create 5");
		cameraInfo = (TextView) findViewById(R.id.vivid_camera__camera_info);
		Log.i(Main.TAG, "create 6");
		setButtonListener();
		Log.i(Main.TAG, "create 7");
	}

	private void setButtonListener() {
		testCameraButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				testCamera(cameraIdToUse);
			}
		});
		openCameraButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				openCamera(cameraIdToUse);
			}
		});
	}

	private void testCamera(int cameraId) {
		CameraInfo info = new CameraInfo();
		Camera.getCameraInfo(cameraId, info);
		String output = new String();
		if (info.facing == CameraInfo.CAMERA_FACING_FRONT) {
			output += "Front";
		} else {
			output += "Rear";
		}
		output += info.orientation;
		output += " Display orientation = ";
		output += getWindowManager().getDefaultDisplay().getRotation();
		cameraInfo.setText(output);
	}

	private void openCamera(int cameraId) {
		Intent intent = new Intent("com.excelsior.vividcamera.Shooting");
		startActivityForResult(intent, requestCodeForPreviewScreen);
	}

	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		Log.v(Main.TAG, "onActivityResult");
		if (requestCode == requestCodeForPreviewScreen) {
			if (resultCode == android.app.Activity.RESULT_OK) {
				String resultFilePath = data.getStringExtra(RESULT_FILE_PATH);
				launchReviewActivity(resultFilePath);
			}
		} else if (requestCode == requestCodeForReview) {} else {
			// error
		}
	}

	private void launchReviewActivity(String resultFilePath) {
		Intent intent = new Intent("com.excelsior.vividcamera.Review");
		intent.putExtra(RESULT_FILE_PATH, resultFilePath);
		startActivityForResult(intent, requestCodeForReview);
	}
}
