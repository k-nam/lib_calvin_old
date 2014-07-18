package com.excelsior.prototype.vividcamera;

import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.excelsior.prototype.R;

public class VividCamera extends Fragment {
	Button openCameraButton;
	TextView cameraInfo;

	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		View view = inflater.inflate(R.layout.vivid_camera, container, false);
		this.openCameraButton = (Button) view.findViewById(R.id.vivid_camera__open_camera_button);
		this.cameraInfo = (TextView) view.findViewById(R.id.vivid_camera__camera_info);
		this.setButtonListener();
		return view;
	}

	private void setButtonListener() {
		this.openCameraButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				testCamera();
			}
		});
	}

	private void testCamera() {
		Camera camera = this.getCameraInstance();
		CameraInfo info = new CameraInfo();
		Camera.getCameraInfo(0, info);
		String output = new String();
		if (info.facing == CameraInfo.CAMERA_FACING_FRONT) {
			output += "Front";
		} else {
			output += "Rear";
		}
		output += info.orientation;
		this.cameraInfo.setText(output);
	}

	/** A safe way to get an instance of the Camera object. */
	public static Camera getCameraInstance() {
		Camera c = null;
		try {
			c = Camera.open(); // attempt to get a Camera instance
		} catch (Exception e) {
			// Camera is not available (in use or does not exist)
		}
		return c; // returns null if camera is unavailable
	}
}
