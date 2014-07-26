package com.excelsior.prototype.vividcamera;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.lang.reflect.Method;
import java.text.SimpleDateFormat;
import java.util.Date;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.ShutterCallback;
import android.hardware.Camera.Size;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.Surface;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

import com.excelsior.prototype.R;

public class Preview extends Activity {
	private FrameLayout mPreview;
	private SimpleSurfaceView mSurfaceView;
	private Camera mCamera;
	private int cameraIdToUse = 1;
	int realWidth;
	int realHeight;
	public static String VIVIDCAMERA_TAG = "Preview";
	private Context activityContext = this;
	private boolean isShutterCallbackBusy = false;
	private boolean isJpegCallbackBusy = false;
	private boolean isDataProcessingBusy = false;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.i(VIVIDCAMERA_TAG, "onCreate");
		// requestWindowFeature(Window.FEATURE_NO_TITLE);
		// getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
		// WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setContentView(R.layout.vividcamera_preview);
		mPreview = (FrameLayout) findViewById(R.id.vividcamera__preview);
		setClickListener();
	}

	@Override
	public void onStart() {
		super.onStart();
		Log.i(VIVIDCAMERA_TAG, "onStart");
		mCamera = VividCamera.getCameraInstance(cameraIdToUse);
		mSurfaceView = new SimpleSurfaceView(this, mCamera);
		mPreview.addView(mSurfaceView);
		mCamera.startPreview();
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
		mPreview.removeAllViews();
		mCamera.stopPreview();
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

	private void setClickListener() {
		mPreview.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				takePicture();
			}
		});
	}

	private boolean isCameraBusy() {
		return isShutterCallbackBusy || isJpegCallbackBusy || isDataProcessingBusy;
	}

	private void setCameraBusy() {
		isShutterCallbackBusy = true;
		isJpegCallbackBusy = true;
		isDataProcessingBusy = true;
	}

	public void takePicture() {
		if (isCameraBusy()) {
			Log.e(VIVIDCAMERA_TAG, "Camera busy");
			return;
		} else {
			setCameraBusy();
		}
		Log.w(VIVIDCAMERA_TAG, "Picture taken");
		mCamera.takePicture(mShutter, null, mPicture);
		Log.w(VIVIDCAMERA_TAG, "Picture taken2");
		try {
			Thread.sleep(400);
			while (true) {
				try {
					mCamera.startPreview();
					break;
				} catch (Exception e) {
					e.printStackTrace();
					Thread.sleep(100);
					Log.w(VIVIDCAMERA_TAG, "Waiting loop");
				}
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		isDataProcessingBusy = false;
		Log.w(VIVIDCAMERA_TAG, "Restarted preview");
	}

	private ShutterCallback mShutter = new ShutterCallback() {
		public void onShutter() {
			Log.w(VIVIDCAMERA_TAG, "ShutterCallback");
			isShutterCallbackBusy = false;
		}
	};
	private PictureCallback mPicture = new PictureCallback() {
		@Override
		public void onPictureTaken(byte[] data, Camera camera) {
			File pictureFile = getOutputMediaFile(MEDIA_TYPE_IMAGE);
			if (pictureFile == null) {
				Log.d(VIVIDCAMERA_TAG, "Error creating media file, check storage permission");
				return;
			}
			try {
				FileOutputStream fos = new FileOutputStream(pictureFile);
				fos.write(data);
				fos.close();
			} catch (FileNotFoundException e) {
				Log.d(VIVIDCAMERA_TAG, "File not found: " + e.getMessage());
			} catch (IOException e) {
				Log.d(VIVIDCAMERA_TAG, "Error accessing file: " + e.getMessage());
			}
			isJpegCallbackBusy = false;
			Log.w(VIVIDCAMERA_TAG, "PictureCallback finished");
		}
	};
	public static final int MEDIA_TYPE_IMAGE = 1;
	public static final int MEDIA_TYPE_VIDEO = 2;

	/** Create a file Uri for saving an image or video */
	private static Uri getOutputMediaFileUri(int type) {
		return Uri.fromFile(getOutputMediaFile(type));
	}

	/** Create a File for saving an image or video */
	private static File getOutputMediaFile(int type) {
		// To be safe, you should check that the SDCard is mounted
		// using Environment.getExternalStorageState() before doing this.
		File mediaStorageDir = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES),
				"MyCameraApp");
		// This location works best if you want the created images to be shared
		// between applications and persist after your app has been uninstalled.
		// Create the storage directory if it does not exist
		if (!mediaStorageDir.exists()) {
			if (!mediaStorageDir.mkdirs()) {
				Log.d(VIVIDCAMERA_TAG, "failed to create directory");
				return null;
			}
		}
		// Create a media file name
		String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
		File mediaFile;
		if (type == MEDIA_TYPE_IMAGE) {
			mediaFile = new File(mediaStorageDir.getPath() + File.separator + "IMG_" + timeStamp + ".jpg");
		} else if (type == MEDIA_TYPE_VIDEO) {
			mediaFile = new File(mediaStorageDir.getPath() + File.separator + "VID_" + timeStamp + ".mp4");
		} else {
			return null;
		}
		Log.w(VIVIDCAMERA_TAG, "get outputfile " + mediaStorageDir.getPath() + File.separator + "IMG_" + timeStamp + ".jpg");
		return mediaFile;
	}

	private void checkResolution() {
		Camera.Parameters params = mCamera.getParameters();
		for (Size size : params.getSupportedPictureSizes()) {
			if (size.width > 1000) {
				// Log.v(VIVIDCAMERA_TAG, "Available picture: " + size.width + " " +
				// size.height + " ratio: " + (float) size.width
				// / size.height);
			}
		}
		for (Size size : params.getSupportedPreviewSizes()) {
			if (size.width > 1000) {
				// Log.v(VIVIDCAMERA_TAG, "Available preview: " + size.width + " " +
				// size.height + " ratio: " + (float) size.width
				// / size.height);
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
				}
			});
			return builder.create();
		}
	}

	private void showInstructionDialog() {
		new InstructionDialog().show(getFragmentManager(), "aa");
	}
}
