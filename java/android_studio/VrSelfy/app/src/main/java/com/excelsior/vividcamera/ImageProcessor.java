package com.excelsior.vividcamera;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.util.Log;

public class ImageProcessor {
	public void Concatenate() {
	}

	// take 1.33 ratio crop of each part and combine to make 1.5 ratio result
	public static Bitmap cropAndCombineBitmap(Bitmap leftHalf, Bitmap rightHalf) {
		int width = leftHalf.getWidth();
		int croppedHeight = (int) (width * 1.333);
		int originalHeight = leftHalf.getHeight();
		int upperLetterBoxHeight = (originalHeight - croppedHeight) / 2;
		Bitmap leftCrop = Bitmap.createBitmap(leftHalf, 0, upperLetterBoxHeight, width, croppedHeight);
		Bitmap rightCrop = Bitmap.createBitmap(rightHalf, 0, upperLetterBoxHeight, width, croppedHeight);
		return combineBitmap(leftCrop, rightCrop);
	}

	private static Bitmap combineBitmap(Bitmap leftHalf, Bitmap rightHalf) {
		Bitmap result = null;
		int width = 0;
		int height = 0;
		if (leftHalf == null || rightHalf == null) {
			Log.e(Main.TAG, "combineImage null");
		}
		if (leftHalf.getWidth() > rightHalf.getWidth()) {
			width = leftHalf.getWidth() + rightHalf.getWidth();
			height = leftHalf.getHeight();
		} else {
			width = rightHalf.getWidth() + rightHalf.getWidth();
			height = leftHalf.getHeight();
		}
		if (leftHalf.getHeight() != rightHalf.getHeight()) {
			// input error
			Log.e(Main.TAG, "combineImage error");
			return null;
		}
		result = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(result);
		canvas.drawBitmap(leftHalf, 0f, 0f, null);
		canvas.drawBitmap(rightHalf, leftHalf.getWidth(), 0f, null);
		return result;
	}

	public static Bitmap rotateBitmap(Bitmap source, int rotation) {
		Matrix matrix = new Matrix();
		if (rotation != 0f) {
			matrix.preRotate(rotation);
		}
		return Bitmap.createBitmap(source, 0, 0, source.getWidth(), source.getHeight(), matrix, true);
	}

	public static void saveBitmapToFile(Bitmap bitmap, File file) {
		try {
			FileOutputStream fos = new FileOutputStream(file);
			bitmap.compress(CompressFormat.JPEG, 100, fos);
		} catch (IOException e) {
			Log.e(Main.TAG, "save bitmap error");
		}
	}
}
