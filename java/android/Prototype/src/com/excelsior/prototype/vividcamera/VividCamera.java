package com.excelsior.prototype.vividcamera;

import android.content.Intent;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.excelsior.prototype.R;

public class VividCamera extends Fragment {
	Button testCameraButton;
	Button openCameraButton;
	TextView cameraInfo;
	private int cameraIdToUse = 1;
	private int requestCodeForPreviewScreen = 0;
	public static String TAG = "Preview";
	public static String RESULT_FILE_PATH = "Result file path";

	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		View view = inflater.inflate(R.layout.vividcamera, container, false);
		// Create our Preview view and set it as the content of our activity.
		testCameraButton = (Button) view.findViewById(R.id.vivid_camera__test_camera_button);
		openCameraButton = (Button) view.findViewById(R.id.vivid_camera__open_camera_button);
		cameraInfo = (TextView) view.findViewById(R.id.vivid_camera__camera_info);
		setButtonListener();
		return view;
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
		output += getActivity().getWindowManager().getDefaultDisplay().getRotation();
		cameraInfo.setText(output);
	}

	private void openCamera(int cameraId) {
		Intent intent = new Intent("com.excelsior.prototype.VividCameraPreview");
		startActivityForResult(intent, requestCodeForPreviewScreen);
	}

	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		Log.v(VividCamera.TAG, "onActivityResult");
		if (requestCode == requestCodeForPreviewScreen) {
			if (resultCode == android.app.Activity.RESULT_OK) {
				String resultFilePath = data.getStringExtra(RESULT_FILE_PATH);
			}
		}
	}
}
