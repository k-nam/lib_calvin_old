package com.excelsior.prototype.vividcamera;

import android.app.Activity;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Surface;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.TextView;

import com.excelsior.prototype.R;

public class VividCamera extends Fragment {
	Button openCameraButton;
	TextView cameraInfo;
	private Camera mCamera;
	private Preview mPreview;

	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		View view = inflater.inflate(R.layout.vivid_camera, container, false);
		mCamera = getCameraInstance();
		// Create our Preview view and set it as the content of our activity.
		mPreview = new Preview(getActivity(), mCamera);
		if (mPreview == null) {
			Log.e("Preview", "mPreview was null");
		}
		FrameLayout preview = (FrameLayout) view.findViewById(R.id.vivid_camera__preview);
		preview.addView(mPreview);
		setCameraDisplayOrientation(getActivity(), 0, mCamera);
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

	// google's solution for setting appropriate rotation
	public static void setCameraDisplayOrientation(Activity activity, int cameraId, android.hardware.Camera camera) {
		android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
		android.hardware.Camera.getCameraInfo(cameraId, info);
		int rotation = activity.getWindowManager().getDefaultDisplay().getRotation();
		int degrees = 0;
		switch (rotation) {
			case Surface.ROTATION_0:
				degrees = 0;
				break;
			case Surface.ROTATION_90:
				degrees = 90;
				break;
			case Surface.ROTATION_180:
				degrees = 180;
				break;
			case Surface.ROTATION_270:
				degrees = 270;
				break;
		}
		int result;
		if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
			result = (info.orientation + degrees) % 360;
			result = (360 - result) % 360; // compensate the mirror
		} else { // back-facing
			result = (info.orientation - degrees + 360) % 360;
		}
		camera.setDisplayOrientation(result);
	}

	private void testCamera() {
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
