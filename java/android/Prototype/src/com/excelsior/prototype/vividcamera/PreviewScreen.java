package com.excelsior.prototype.vividcamera;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.text.SimpleDateFormat;
import java.util.Collections;
import java.util.Date;
import java.util.List;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.SurfaceTexture;
import android.graphics.YuvImage;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
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
import android.view.TextureView;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

import com.excelsior.prototype.R;

public class PreviewScreen extends Activity {
	private FrameLayout mPreview;
	private TextureView mTextureView;
	private SimpleSurfaceView mSurfaceView;
	private Button mTakePictureButton;
	private Button mTakeScreenshotButton;
	private LinearLayout mButtonContainer;
	private Camera mCamera;
	private int cameraIdToUse = 1;
	int realWidth;
	int realHeight;
	public static String VIVIDCAMERA_TAG = "Preview";
	private Context activityContext = this;
	private boolean isShutterCallbackBusy = false;
	private boolean isJpegCallbackBusy = false;
	private boolean isDataProcessingBusy = false;
	private boolean shouldCapturePreview = false;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.i(VIVIDCAMERA_TAG, "onCreate");
		// requestWindowFeature(Window.FEATURE_NO_TITLE);
		// getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
		// WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setContentView(R.layout.vividcamera__preview_screen);
		getScreenDimension();
		Log.w(VIVIDCAMERA_TAG, "Screen width = " + realWidth + " height is: " + realHeight);
		mPreview = (FrameLayout) findViewById(R.id.vividcamera__preview);
		// mTakePictureButton = (Button)
		// findViewById(R.id.vividcamera__take_picture_button);
		// mTakeScreenshotButton = (Button)
		// findViewById(R.id.vividcamera__take_screenshot_button);
		mButtonContainer = (LinearLayout) findViewById(R.id.vividcamera__buttons);
		// mTextureView = new TextureView(this);
		// mTextureView.setSurfaceTextureListener(this);
		// setContentView(mTextureView);
		setClickListener();
	}

	public void setPreviewCallback() {
		mCamera.setPreviewCallback(mPreviewCallback);
	}

	@Override
	public void onStart() {
		super.onStart();
		Log.i(VIVIDCAMERA_TAG, "onStart");
		mCamera = VividCamera.getCameraInstance(cameraIdToUse);
		mSurfaceView = new SimpleSurfaceView(this, mCamera); // mCamera.startPreview()
																													// was called here
		mPreview.addView(mSurfaceView);
		mButtonContainer.bringToFront();
		if (mSurfaceView == null) {
			Log.e(VIVIDCAMERA_TAG, "mPreview was null");
		}
		Log.v(VIVIDCAMERA_TAG, "OS Version is" + Build.VERSION.SDK_INT);
		setCameraDisplayOrientation(this, cameraIdToUse, mCamera);
		adjustPictureAndPreviewSize();
		adjustAspect();
		// showInstructionDialog();
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
				restartPreview();
			}
		});
	}

	public void onClickTakePictureButton(View v) {
		takePicture();
	}

	public void onClickTakeScreenshotButton(View v) {
		takeScreenshot();
	}

	private void restartPreview() {
		mCamera.stopPreview();
		try {
			Thread.sleep(10);
		} catch (Exception e) {}
		setPreviewCallback();
		mCamera.startPreview();
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
		mCamera.takePicture(mShutterCallback, null, mPictureCallback);
		Log.w(VIVIDCAMERA_TAG, "Picture taken2");
		try {
			Thread.sleep(100);
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

	public void takeScreenshot() {
		shouldCapturePreview = true;
	}

	private ShutterCallback mShutterCallback = new ShutterCallback() {
		public void onShutter() {
			Log.w(VIVIDCAMERA_TAG, "ShutterCallback");
			isShutterCallbackBusy = false;
		}
	};
	private PictureCallback mPictureCallback = new PictureCallback() {
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
		// Log.e(VIVIDCAMERA_TAG, "External storage: " +
		// Environment.getExternalStorageState());
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
		// Log.w(VIVIDCAMERA_TAG, "get outputfile " + mediaStorageDir.getPath() +
		// File.separator + "IMG_" + timeStamp + ".jpg");
		return mediaFile;
	}

	private Size[] getBestPictureAndPreviewSizes(List<Size> pictureSizes, List<Size> previewSizes) {
		// We want biggest preview except for square type. It will be best if it
		// fits screen.
		for (Size size : sortSizeList(previewSizes)) {
			if (getRatio(size) > 1.3) {
				Size pictureSize = getBestPictureSize(pictureSizes, size);
				if (pictureSize != null) {
					return new Size[] { pictureSize, size };
				}
			}
		}
		return null;
	}

	private List<Size> sortSizeList(List<Size> list) {
		for (int i = 1; i < list.size(); i++) {
			for (int j = i - 1; j >= 0; j--) {
				if (less(list.get(j), list.get(j + 1))) { // sort in REVERSE order
					Collections.swap(list, j, j + 1);
				} else {
					break;
				}
			}
		}
		return list;
	}

	private boolean less(Size a, Size b) {
		return a.width < b.width || (a.width == b.width && a.height < b.height);
	}

	private Size getBestPictureSize(List<Size> pictureSizes, Size previewSize) {
		// Returns null if there is no close answer
		for (Size size : sortSizeList(pictureSizes)) {
			if (isSimiliarRatio(size, previewSize)) {
				return size;
			}
		}
		return null;
	}

	private boolean isSimiliarRatio(Size a, Size b) {
		return isSimiliar(getRatio(a), getRatio(b));
	}

	private float getRatio(Size size) {
		return (float) size.width / size.height;
	}

	private boolean isSimiliar(float a, float b) {
		float diff = a - b;
		return diff < 0.1 && diff > -0.1;
	}

	private void adjustPictureAndPreviewSize() {
		Camera.Parameters params = mCamera.getParameters();
		// pick largest resolution (except square type)
		for (Size size : sortSizeList(params.getSupportedPictureSizes())) {
			if (size.width > 1000) {
				Log.v(VIVIDCAMERA_TAG, "Available picture: " + size.width + " " + size.height + " ratio: " + getRatio(size));
			}
		}
		// for (int[] fps : params.getSupportedPreviewFpsRange()) {
		// Log.v(VIVIDCAMERA_TAG, "Supported FPS: " + fps[0] + " " + fps[1]);
		// }
		for (Size size : sortSizeList(params.getSupportedPreviewSizes())) {
			if (size.width > 1000) {
				Log.v(VIVIDCAMERA_TAG, "Available preview: " + size.width + " " + size.height + " ratio: " + getRatio(size));
			}
		}
		Size[] pictureAndPreviewSize = this.getBestPictureAndPreviewSizes(params.getSupportedPictureSizes(),
				params.getSupportedPreviewSizes());
		if (pictureAndPreviewSize != null) {
			params.setPictureSize(pictureAndPreviewSize[0].width, pictureAndPreviewSize[0].height);
			params.setPreviewSize(pictureAndPreviewSize[1].width, pictureAndPreviewSize[1].height);
		}
		mCamera.setParameters(params);
		Log.v(VIVIDCAMERA_TAG, "Set picture size to: " + params.getPictureSize().width + " "
				+ params.getPictureSize().height);
		Log.v(VIVIDCAMERA_TAG, "Set preview size to: " + params.getPreviewSize().width + " "
				+ params.getPreviewSize().height);
	}

	private void getScreenDimension() {
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

	private void adjustAspect() {
		Camera.Size previewSize = mCamera.getParameters().getPreviewSize();
		Camera.Size pictureSize = mCamera.getParameters().getPictureSize();
		// Log.v(VIVIDCAMERA_TAG, "screen size is " + realWidth + ", " +
		// realHeight);
		// Log.v(VIVIDCAMERA_TAG, "framelayout size is " + preview.getWidth() + ", "
		// + preview.getHeight());
		// Log.v(VIVIDCAMERA_TAG, "picture size was " + pictureSize.width + ", " +
		// pictureSize.height);
		// Log.v(VIVIDCAMERA_TAG, "preview size was " + previewSize.width + ", " +
		// previewSize.height);
		// landscape
		float ratio = getRatio(previewSize);
		int rotation = getWindowManager().getDefaultDisplay().getRotation();
		if (rotation == Surface.ROTATION_0 || rotation == Surface.ROTATION_180) {
			ratio = (float) previewSize.height / previewSize.width;
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
		Log.v(VIVIDCAMERA_TAG, "result mPreview size is " + new_width + ", " + new_height + " ratio is " + ratio);
		mPreview.setLayoutParams(new LinearLayout.LayoutParams(new_width, new_height));
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

	private Camera.PreviewCallback mPreviewCallback = new Camera.PreviewCallback() {
		public void onPreviewFrame(byte[] data, Camera camera) {
			// Log.v(VIVIDCAMERA_TAG, "Preview callback!");
			if (shouldCapturePreview == false) {
				return;
			}
			Parameters params = camera.getParameters();
			Size previewSize = params.getPreviewSize();
			int imageFormat = params.getPreviewFormat();
			if (imageFormat == ImageFormat.NV21) {
				Rect rect = new Rect(0, 0, previewSize.width, previewSize.height);
				YuvImage img = new YuvImage(data, ImageFormat.NV21, previewSize.width, previewSize.height, null);
				OutputStream outStream = null;
				File file = getOutputMediaFile(MEDIA_TYPE_IMAGE);
				try {
					outStream = new FileOutputStream(file);
					img.compressToJpeg(rect, 100, outStream);
					outStream.flush();
					outStream.close();
				} catch (FileNotFoundException e) {
					e.printStackTrace();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			Log.e(VIVIDCAMERA_TAG, "Preview captured!");
			shouldCapturePreview = false;
		}
	};

	public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
		Log.v(VIVIDCAMERA_TAG, "TextureView ready");
		try {
			mCamera.setPreviewTexture(surface);
			// mCamera.startPreview();
		} catch (IOException ioe) {
			// Something bad happened
		}
	}

	public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
		// Ignored, Camera does all the work for us
	}

	public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
		return true;
	}

	public void onSurfaceTextureUpdated(SurfaceTexture surface) {
		// Invoked every time there's a new Camera preview frame
	}
}
