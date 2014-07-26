package com.excelsior.prototype.vividcamera;

import java.lang.reflect.Method;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.os.Build;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.Surface;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.Toast;

import com.excelsior.prototype.R;

public class Preview extends Activity {
	private SimpleSurfaceView mSurfaceView;
	private Camera mCamera;
	private int cameraIdToUse = 1;
	int realWidth;
	int realHeight;
	public String VIVIDCAMERA_TAG = "Preview";
	private Context activityContext = this;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.i(VIVIDCAMERA_TAG, "onCreate");
		// requestWindowFeature(Window.FEATURE_NO_TITLE);
		// getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
		// WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setContentView(R.layout.vividcamera_preview);
	}

	@Override
	public void onStart() {
		super.onStart();
		Log.i(VIVIDCAMERA_TAG, "onStart");
		FrameLayout preview = (FrameLayout) findViewById(R.id.vividcamera__preview);
		mCamera = VividCamera.getCameraInstance(cameraIdToUse);
		mSurfaceView = new SimpleSurfaceView(this, mCamera);
		preview.addView(mSurfaceView);
		if (mSurfaceView == null) {
			Log.e(VIVIDCAMERA_TAG, "mPreview was null");
		}
		Log.v(VIVIDCAMERA_TAG, "OS Version is" + Build.VERSION.SDK_INT);
		setCameraDisplayOrientation(this, cameraIdToUse, mCamera);
		checkResolution();
		setScreenDimension();
		adjustPreviewAspect();
		showInstructionDialog();
	}

	@Override
	public void onRestart() {
		Log.i(VIVIDCAMERA_TAG, "onRestart");
		super.onRestart();
	}

	@Override
	public void onStop() {
		super.onStop();
		Log.i(VIVIDCAMERA_TAG, "onStop");
		mCamera.setPreviewCallback(null);
		mSurfaceView.getHolder().removeCallback(mSurfaceView);
		mCamera.release();
	}

	@Override
	public void onPause() {
		Log.i(VIVIDCAMERA_TAG, "onPause");
		super.onPause();
	}

	@Override
	public void onResume() {
		Log.i(VIVIDCAMERA_TAG, "onResume");
		super.onResume();
	}

	@Override
	public void onDestroy() {
		Log.i(VIVIDCAMERA_TAG, "onDestroy");
		super.onDestroy();
	}

	private void checkResolution() {
		Camera.Parameters params = mCamera.getParameters();
		for (Size size : params.getSupportedPictureSizes()) {
			if (size.width > 1000) {
				Log.v(VIVIDCAMERA_TAG, "Available picture: " + size.width + " " + size.height + " ratio: " + (float) size.width
						/ size.height);
			}
		}
		for (Size size : params.getSupportedPreviewSizes()) {
			if (size.width > 1000) {
				Log.v(VIVIDCAMERA_TAG, "Available preview: " + size.width + " " + size.height + " ratio: " + (float) size.width
						/ size.height);
			}
		}
		// params.setPictureSize(1920, 1080);
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
				Log.v("Display Info", "Couldn't use reflection to get the real display metrics.");
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
		// Log.v(VIVIDCAMERA_TAG, "screen size is " + realWidth + ", " +
		// realHeight);
		// Log.v(VIVIDCAMERA_TAG, "framelayout size is " + preview.getWidth() + ", "
		// + preview.getHeight());
		Log.v(VIVIDCAMERA_TAG, "preview size is " + size.width + ", " + size.height);
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
		Log.v(VIVIDCAMERA_TAG, "result size is " + new_width + ", " + new_height + " ratio is " + ratio);
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

	private class InstructionDialog extends DialogFragment {
		public Dialog onCreateDialog(Bundle savedInstanceState) {
			Builder builder = new AlertDialog.Builder(activityContext);
			builder.setIcon(R.drawable.ic_launcher);
			builder.setTitle("Be ready!");
			builder.setMessage("Now pose, and take the first selfy");
			builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {
					Toast.makeText(activityContext, "OK clicked", Toast.LENGTH_SHORT).show();
				}
			});
			return builder.create();
		}
	}

	private void showInstructionDialog() {
		new InstructionDialog().show(getFragmentManager(), "aa");
	}
}
