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
import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.SurfaceTexture;
import android.graphics.YuvImage;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.ShutterCallback;
import android.hardware.Camera.Size;
import android.media.ExifInterface;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.excelsior.prototype.R;

public class PreviewScreen extends Activity {
	private FrameLayout mFrame;
	private TextureView mTextureView;
	private SimpleSurfaceView mSurfaceView;
	private ImageView mRedCircle;
	private Button mTakePictureButton;
	private Button mTakeScreenshotButton;
	private LinearLayout mButtonContainer;
	private TextView mInstructionTextView;
	private FrameLayout mDarkScreen;
	private Camera mCamera;
	private int cameraIdToUse = 1;
	int realWidth;
	int realHeight;
	private Context activityContext = this;
	private boolean isShutterCallbackBusy = false;
	private boolean isJpegCallbackBusy = false;
	private boolean isDataProcessingBusy = false;
	private boolean shouldCapturePreview = false;
	private Handler mHandler = new Handler();
	private String mPictureFileName1;
	private String mPictureFileName2;
	private Bitmap mBitmap1;
	private Bitmap mBitmap2;
	public static final int MEDIA_TYPE_IMAGE = 1;
	public static final int MEDIA_TYPE_VIDEO = 2;
	// should be 0 or positive when calculated later
	private int exifImageRotation = -1;

	private enum Stage {
		Instruction, FirstShot, Instruction2, SecondShot, Completed, ShowResult
	};

	private enum Position {
		Left, Right
	};

	private final Runnable mTryEnterFirstShot = new Runnable() {
		public void run() {
			// Log.v(VividCamera.TAG, "runnable: mTryEnterFirstShot");
			tryEnterStage(Stage.FirstShot);
		}
	};
	private final Runnable mTryEnterSecondShot = new Runnable() {
		public void run() {
			// Log.v(VividCamera.TAG, "runnable: mTryEnterSecondShot");
			tryEnterStage(Stage.SecondShot);
		}
	};
	private final Runnable mTryEnterShowResult = new Runnable() {
		public void run() {
			// Log.v(VividCamera.TAG, "runnable: mCheckBitmaps");
			if (mBitmap1 != null && mBitmap2 != null) { // OK
				enterShowResultStage();
			} else {
				Log.i(VividCamera.TAG, "mTryEnterShowResult loop");
				tryEnterShowResult();
			}
		}
	};

	private void enterStageDelayed(final Stage stage, int millisec) {
		Timer myTimer = new Timer();
		myTimer.schedule(new TimerTask() {
			@Override
			public void run() {
				if (stage == Stage.FirstShot) {
					mHandler.post(mTryEnterFirstShot);
				} else if (stage == Stage.SecondShot) {
					mHandler.post(mTryEnterSecondShot);
				} else {}
			}
		}, millisec);
	}

	private void tryEnterShowResult() {
		new Timer().schedule(new TimerTask() {
			public void run() {
				mHandler.post(mTryEnterShowResult);
			}
		}, 300);
	}

	private Stage mCurrentStage;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.i(VividCamera.TAG, "onCreate");
		// requestWindowFeature(Window.FEATURE_NO_TITLE);
		// getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
		// WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setContentView(R.layout.vividcamera__preview_screen);
		getScreenDimension();
		Log.w(VividCamera.TAG, "Screen width = " + realWidth + " height is: " + realHeight);
		mFrame = (FrameLayout) findViewById(R.id.vividcamera__preview);
		mRedCircle = (ImageView) findViewById(R.id.vividcamera__redcircle);
		mButtonContainer = (LinearLayout) findViewById(R.id.vividcamera__buttons);
		mDarkScreen = (FrameLayout) findViewById(R.id.vividcamera__dark);
		mDarkScreen.setBackgroundColor(0xFFFFFFFF);
		mInstructionTextView = (TextView) findViewById(R.id.vividcamera__instruction_text);
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
		Log.i(VividCamera.TAG, "onStart");
		// showInstructionDialog();
	}

	@Override
	public void onRestart() {
		Log.i(VividCamera.TAG, "onRestart");
		super.onRestart();
	}

	@Override
	public void onStop() {
		super.onStop();
		Log.i(VividCamera.TAG, "onStop");
	}

	@Override
	public void onPause() {
		Log.i(VividCamera.TAG, "onPause");
		super.onPause();
		mFrame.removeView(mSurfaceView);
		mCamera.stopPreview();
		mCamera.setPreviewCallback(null);
		mSurfaceView.getHolder().removeCallback(mSurfaceView);
		mCamera.release();
	}

	@Override
	public void onResume() {
		Log.i(VividCamera.TAG, "onResume");
		super.onResume();
		mCamera = VividCamera.getCameraInstance(cameraIdToUse);
		setPictureOrientation();
		setCameraDisplayOrientation(this, cameraIdToUse, mCamera);
		adjustPictureAndPreviewSize();
		adjustFrameSize();
		// mCamera.startPreview() is called here
		mSurfaceView = new SimpleSurfaceView(this, mCamera);
		// adjust subviews order. I don't change their order anywhere else;
		// use alpha value to mimic the change of the order of child views
		mFrame.addView(mSurfaceView);
		mRedCircle.bringToFront();
		mButtonContainer.bringToFront();
		mButtonContainer.setAlpha(0.0f);
		mDarkScreen.bringToFront();
		mInstructionTextView.bringToFront();
		enterInstructionStage();
		mBitmap1 = null;
		mBitmap2 = null;
	}

	@Override
	public void onDestroy() {
		Log.i(VividCamera.TAG, "onDestroy");
		super.onDestroy();
	}

	private void setClickListener() {
		mFrame.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				onClickScreen();
			}
		});
	}

	private void onClickScreen() {
		switch (mCurrentStage) {
			case Instruction:
				enterFirstShotStage();
				break;
			case FirstShot:
				takePicture();
				enterInstructionStage2();
				break;
			case SecondShot:
				takePicture();
				enterCompletedStage();
				break;
			default:
				break;
		}
	}

	private void tryEnterStage(Stage stage) {
		switch (stage) {
			case FirstShot:
				if (mCurrentStage == Stage.Instruction) {
					enterFirstShotStage();
					// possible case due to delayed timer call
				} else {}
				break;
			case SecondShot:
				if (mCurrentStage == Stage.Instruction2) {
					enterSecondShotStage();
					// possible case due to delayed timer call
				} else { // serious error
					Log.e(VividCamera.TAG, "Stage order error.");
					finish();
				}
				break;
			default:
				break;
		}
	}

	private void enterInstructionStage() {
		Log.v(VividCamera.TAG, "enterInstructionStage");
		mCurrentStage = Stage.Instruction;
		hideRedCircle();
		showInstruction("Direct your head exactly toward red dot, and Touch anywhere");
		enterStageDelayed(Stage.FirstShot, 5000);
	}

	private void enterFirstShotStage() {
		Log.v(VividCamera.TAG, "enterFirstShotStage");
		removeDark();
		hideInstruction();
		placeRedCircle(Position.Right);
		mCurrentStage = Stage.FirstShot;
	}

	private void enterInstructionStage2() {
		Log.v(VividCamera.TAG, "enterInstructionStage2");
		mCurrentStage = Stage.Instruction2;
		hideRedCircle();
		showInstruction("One more time");
		enterStageDelayed(Stage.SecondShot, 1000);
	}

	private void enterSecondShotStage() {
		Log.v(VividCamera.TAG, "enterSecondShotStage");
		removeDark();
		hideInstruction();
		placeRedCircle(Position.Left);
		mCurrentStage = Stage.SecondShot;
	}

	private void enterCompletedStage() {
		Log.v(VividCamera.TAG, "enterCompletedStage");
		hideRedCircle();
		mCurrentStage = Stage.Completed;
		tryEnterShowResult();
	}

	private void enterShowResultStage() {
		Bitmap combined = ImageProcessor.combineBitmap(mBitmap1, mBitmap2);
		ImageProcessor.saveBitmapToFile(combined, getOutputMediaFile(MEDIA_TYPE_IMAGE));
	}

	private void showInstruction(String text) {
		makeDark();
		mInstructionTextView.setAlpha(1.0f);
		mInstructionTextView.setText(text);
	}

	private void hideInstruction() {
		mInstructionTextView.setAlpha(0.0f);
	}

	private void hideRedCircle() {
		mRedCircle.setAlpha(0.0f);
	}

	private void placeRedCircle(Position position) {
		mRedCircle.setAlpha(1.0f);
		FrameLayout.LayoutParams params = (LayoutParams) mRedCircle.getLayoutParams();
		if (position == Position.Left) {
			params.gravity = android.view.Gravity.CENTER_VERTICAL | android.view.Gravity.LEFT;
		} else {
			params.gravity = android.view.Gravity.CENTER_VERTICAL | android.view.Gravity.RIGHT;
		}
		mRedCircle.setLayoutParams(params);
	}

	private void onClickTakeScreenshotButton(View v) {
		// takeScreenshot();
	}

	private void restartPreview() {
		// mCamera.stopPreview();
		makeDark();
		try {
			Thread.sleep(10);
		} catch (Exception e) {} finally {
			// removeDark();
		}
		// setPreviewCallback();
		// mCamera.startPreview();
	}

	private boolean isCameraBusy() {
		return isShutterCallbackBusy || isJpegCallbackBusy || isDataProcessingBusy;
	}

	private void setCameraBusy() {
		isShutterCallbackBusy = true;
		isJpegCallbackBusy = true;
		isDataProcessingBusy = true;
	}

	private void makeDark() {
		// don't understand why this function call is needed, but it needs to be
		// added in Galaxy S3
		adjustDarkScreenSize();
		mDarkScreen.setAlpha(0.8f);
	}

	private void removeDark() {
		mDarkScreen.setAlpha(0.0f);
	}

	private void takePicture() {
		if (isCameraBusy()) {
			Log.e(VividCamera.TAG, "Camera busy");
			return;
		} else {
			setCameraBusy();
		}
		// makeDark();
		mCamera.takePicture(mShutterCallback, null, mPictureCallback);
		try {
			Thread.sleep(300);
			while (true) {
				try {
					mCamera.startPreview();
					break;
				} catch (Exception e) {
					e.printStackTrace();
					Thread.sleep(200);
					// Log.w(VividCamera.TAG, "Waiting loop");
				}
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		} finally {}
		isDataProcessingBusy = false;
		Log.w(VividCamera.TAG, "Picture taken");
	}

	private void takeScreenshot() {
		shouldCapturePreview = true;
	}

	private ShutterCallback mShutterCallback = new ShutterCallback() {
		public void onShutter() {
			// Log.w(VividCamera.TAG, "ShutterCallback");
			isShutterCallbackBusy = false;
		}
	};
	private PictureCallback mPictureCallback = new PictureCallback() {
		@Override
		public void onPictureTaken(byte[] data, Camera camera) {
			if (exifImageRotation < 0) {
				Log.e(VividCamera.TAG, "exifImageRotation : " + exifImageRotation);
				File pictureFile = getOutputMediaFile(MEDIA_TYPE_IMAGE);
				if (pictureFile == null) {
					Log.d(VividCamera.TAG, "Error creating media file, check storage permission");
					return;
				}
				try {
					FileOutputStream fos = new FileOutputStream(pictureFile);
					fos.write(data);
					fos.close();
				} catch (FileNotFoundException e) {
					Log.d(VividCamera.TAG, "File not found: " + e.getMessage());
				} catch (IOException e) {
					Log.d(VividCamera.TAG, "Error accessing file: " + e.getMessage());
				}
				exifImageRotation = calculateExifImageRotation(pictureFile);
				pictureFile.delete();
			}
			isJpegCallbackBusy = false;
			if (mBitmap1 == null) {
				// mPictureFileName1 = pictureFile.getAbsolutePath();
				mBitmap1 = getBitmapFromJpegData(data);
			} else if (mBitmap1 != null && mBitmap2 == null) {
				// mPictureFileName2 = pictureFile.getAbsolutePath();
				mBitmap2 = getBitmapFromJpegData(data);
			} else { // serious error
				Log.e(VividCamera.TAG, "Picture callback error");
				finish();
			}
			// Log.w(VividCamera.TAG, "PictureCallback finished");
		}
	};

	private Bitmap getBitmapFromJpegData(byte[] data) {
		return ImageProcessor.rotateBitmap(BitmapFactory.decodeByteArray(data, 0, data.length), exifImageRotation);
	}

	private int calculateExifImageRotation(File pictureFile) {
		String fileName = pictureFile.getAbsolutePath();
		int exifOrientation = 0;
		try {
			exifOrientation = new ExifInterface(fileName).getAttributeInt(ExifInterface.TAG_ORIENTATION,
					ExifInterface.ORIENTATION_NORMAL);
		} catch (IOException e) {
			e.printStackTrace();
		}
		Log.e(VividCamera.TAG, "calculated exif rotation = " + exifToDegrees(exifOrientation));
		return exifToDegrees(exifOrientation);
	}

	private static int exifToDegrees(int exifOrientation) {
		if (exifOrientation == ExifInterface.ORIENTATION_ROTATE_90) {
			return 90;
		} else if (exifOrientation == ExifInterface.ORIENTATION_ROTATE_180) {
			return 180;
		} else if (exifOrientation == ExifInterface.ORIENTATION_ROTATE_270) {
			return 270;
		}
		return 0;
	}

	/** Create a file Uri for saving an image or video */
	private static Uri getOutputMediaFileUri(int type) {
		return Uri.fromFile(getOutputMediaFile(type));
	}

	/** Create a File for saving an image or video */
	private static File getOutputMediaFile(int type) {
		// To be safe, you should check that the SDCard is mounted
		// using Environment.getExternalStorageState() before doing this.
		// Log.e(VividCamera.TAG, "External storage: " +
		// Environment.getExternalStorageState());
		File mediaStorageDir = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES),
				"MyCameraApp");
		// This location works best if you want the created images to be shared
		// between applications and persist after your app has been uninstalled.
		// Create the storage directory if it does not exist
		if (!mediaStorageDir.exists()) {
			if (!mediaStorageDir.mkdirs()) {
				Log.d(VividCamera.TAG, "failed to create directory");
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
		// Log.w(VividCamera.TAG, "get outputfile " + mediaStorageDir.getPath() +
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
				// Log.v(VividCamera.TAG, "Available picture: " + size.width + " " +
				// size.height + " ratio: " + getRatio(size));
			}
		}
		// for (int[] fps : params.getSupportedPreviewFpsRange()) {
		// Log.v(VividCamera.TAG, "Supported FPS: " + fps[0] + " " + fps[1]);
		// }
		for (Size size : sortSizeList(params.getSupportedPreviewSizes())) {
			if (size.width > 1000) {
				// Log.v(VividCamera.TAG, "Available preview: " + size.width + " " +
				// size.height + " ratio: " + getRatio(size));
			}
		}
		Size[] pictureAndPreviewSize = this.getBestPictureAndPreviewSizes(params.getSupportedPictureSizes(),
				params.getSupportedPreviewSizes());
		if (pictureAndPreviewSize != null) {
			params.setPictureSize(pictureAndPreviewSize[0].width, pictureAndPreviewSize[0].height);
			params.setPreviewSize(pictureAndPreviewSize[1].width, pictureAndPreviewSize[1].height);
		}
		mCamera.setParameters(params);
		Log.v(VividCamera.TAG, "Set picture size to: " + params.getPictureSize().width + " "
				+ params.getPictureSize().height);
		Log.v(VividCamera.TAG, "Set preview size to: " + params.getPreviewSize().width + " "
				+ params.getPreviewSize().height);
	}

	public void setPictureOrientation() {
		int orientation = 0; // natural position
		android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
		android.hardware.Camera.getCameraInfo(cameraIdToUse, info);
		int rotation = 0;
		if (info.facing == CameraInfo.CAMERA_FACING_FRONT) {
			rotation = (info.orientation - orientation + 360) % 360;
		} else { // back-facing camera
			rotation = (info.orientation + orientation) % 360;
		}
		Camera.Parameters params = mCamera.getParameters();
		params.setRotation(rotation);
		mCamera.setParameters(params);
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

	private void adjustFrameSize() {
		Camera.Size previewSize = mCamera.getParameters().getPreviewSize();
		Camera.Size pictureSize = mCamera.getParameters().getPictureSize();
		// Log.v(VividCamera.TAG, "screen size is " + realWidth + ", " +
		// realHeight);
		// Log.v(VividCamera.TAG, "framelayout size is " + preview.getWidth() + ", "
		// + preview.getHeight());
		// Log.v(VividCamera.TAG, "picture size was " + pictureSize.width + ", " +
		// pictureSize.height);
		// Log.v(VividCamera.TAG, "preview size was " + previewSize.width + ", " +
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
		Log.v(VividCamera.TAG, "result mPreview size is " + new_width + ", " + new_height + " ratio is " + ratio);
		mFrame.setLayoutParams(new LinearLayout.LayoutParams(new_width, new_height));
	}

	private void adjustDarkScreenSize() {
		ViewGroup.LayoutParams params = mDarkScreen.getLayoutParams();
		params.width = mFrame.getLayoutParams().width;
		params.height = mFrame.getLayoutParams().height;
		// Log.v(VividCamera.TAG, "adjust dark screen " + params.width + ", " +
		// params.height);
		mDarkScreen.setLayoutParams(params);
	}

	// google's solution for setting appropriate rotation
	private void setCameraDisplayOrientation(Activity activity, int cameraId, android.hardware.Camera camera) {
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
			// Log.v(VividCamera.TAG, "Preview callback!");
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
			Log.e(VividCamera.TAG, "Preview captured!");
			shouldCapturePreview = false;
		}
	};

	public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
		Log.v(VividCamera.TAG, "TextureView ready");
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
