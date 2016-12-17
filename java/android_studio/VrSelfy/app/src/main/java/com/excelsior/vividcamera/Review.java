package com.excelsior.vividcamera;

import java.io.File;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.widget.ImageView;

import com.excelsior.vividcamera.R;

public class Review extends Activity {
	private ImageView mImageView;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.vividcamera__review);
		mImageView = (ImageView) findViewById(R.id.vividcamera__review_image);
		mImageView.setImageURI(Uri.fromFile(new File(getReviewImagePath())));
	}

	private String getReviewImagePath() {
		Intent intent = getIntent();
		return intent.getStringExtra(Main.RESULT_FILE_PATH);
	}
}
