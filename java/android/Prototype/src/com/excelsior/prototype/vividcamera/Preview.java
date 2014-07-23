package com.excelsior.prototype.vividcamera;

import java.lang.reflect.Method;
import java.util.List;

import android.app.Activity;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.os.Build;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.Surface;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

import com.excelsior.prototype.R;

public class Preview extends Activity {
	private SimpleSurfaceView mSurfaceView;
	private Camera mCamera;
	private int cameraIdToUse = 1;
	int realWidth;
	int realHeight;
	public String VIVIDCAMERA_TAG = "Preview";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.vividcamera_preview);
		FrameLayout preview = (FrameLayout) findViewById(R.id.vividcamera__preview);
		mCamera = VividCamera.getCameraInstance(cameraIdToUse);
		setResolution();
		setCameraDisplayOrientation(this, cameraIdToUse, mCamera);
		mSurfaceView = new SimpleSurfaceView(this, mCamera);
		mSurfaceView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
				| View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide
																																											// nav
																																											// bar
				| View.SYSTEM_UI_FLAG_FULLSCREEN // hide status bar
				| View.SYSTEM_UI_FLAG_IMMERSIVE);
		preview.addView(mSurfaceView);
		if (mSurfaceView == null) {
			Log.e(VIVIDCAMERA_TAG, "mPreview was null");
		}
	}

	@Override
	public void onStart() {
		super.onStart();
		setScreenDimension();
		adjustPreviewAspect();
	}

	private void setResolution() {
		Camera.Parameters params = mCamera.getParameters();
		List<Size> sizes = params.getSupportedPictureSizes();
		// Iterate through all available resolutions and choose one.
		// The chosen resolution will be stored in mSize.
		Size mSize;
		for (Size size : sizes) {
			Log.e(VIVIDCAMERA_TAG, "Available resolution: " + size.width + " " + size.height);
		}
		params.setPictureSize(1920, 1080);
		params.setPreviewSize(1920, 1080);
		mCamera.setParameters(params);
	}

	private void setScreenDimension() {
		Display display = getWindowManager().getDefaultDisplay();
		if (Build.VERSION.SDK_INT >= 17) {
			// new pleasant way to get real metrics
			DisplayMetrics realMetrics = new DisplayMetrics();
			display.getRealMetrics(realMetrics);
			realWidth = realMetrics.widthPixels;
			realHeight = realMetrics.heightPixels;
		} else if (Build.VERSION.SDK_INT >= 14) {
			// reflection for this weird in-between time
			try {
				Method mGetRawH = Display.class.getMethod("getRawHeight");
				Method mGetRawW = Display.class.getMethod("getRawWidth");
				realWidth = (Integer) mGetRawW.invoke(display);
				realHeight = (Integer) mGetRawH.invoke(display);
			} catch (Exception e) {
				// this may not be 100% accurate, but it's all we've got
				realWidth = display.getWidth();
				realHeight = display.getHeight();
				Log.e("Display Info", "Couldn't use reflection to get the real display metrics.");
			}
		} else {
			// This should be close, as lower API devices should not have window
			// navigation bars
			realWidth = display.getWidth();
			realHeight = display.getHeight();
		}
	}

	private void adjustPreviewAspect() {
		FrameLayout preview = (FrameLayout) findViewById(R.id.vividcamera__preview);
		Camera.Size size = mCamera.getParameters().getPreviewSize();
		Log.e(VIVIDCAMERA_TAG, "screen size is " + realWidth + ", " + realHeight);
		Log.e(VIVIDCAMERA_TAG, "framelayout size is " + preview.getWidth() + ", " + preview.getHeight());
		Log.e(VIVIDCAMERA_TAG, "preview size is " + size.width + ", " + size.height);
		// landscape
		float ratio = (float) size.width / size.height;
		int rotation = getWindowManager().getDefaultDisplay().getRotation();
		if (rotation == Surface.ROTATION_0 || rotation == Surface.ROTATION_180) {
			ratio = (float) size.height / size.width;
		}
		// portrait
		// float ratio = (float)size.height/size.width;
		int new_width = 0, new_height = 0;
		if (realWidth / realHeight > ratio) {
			new_width = Math.round(realHeight * ratio);
			new_height = realHeight;
		} else {
			new_width = realWidth;
			new_height = Math.round(realWidth / ratio);
		}
		Log.e(VIVIDCAMERA_TAG, "result size is " + new_width + ", " + new_height + "ratio is " + ratio);
		preview.setLayoutParams(new LinearLayout.LayoutParams(new_width, new_height));
	}

	// google's solution for setting appropriate rotation
	private static void setCameraDisplayOrientation(Activity activity, int cameraId, android.hardware.Camera camera) {
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
}
