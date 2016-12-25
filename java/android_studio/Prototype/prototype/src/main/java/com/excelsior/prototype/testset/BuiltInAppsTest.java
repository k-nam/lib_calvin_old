package com.excelsior.prototype.testset;


import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;

import com.excelsior.prototype.R;

public class BuiltInAppsTest extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.built_in_apps_test);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.built_in_apps_test, menu);
		return true;
	}

	public void onClickOpenBrowser(View v) {
		Intent intent = new Intent(android.content.Intent.ACTION_VIEW, Uri.parse("http://www.naver.com"));
		startActivity(Intent.createChooser(intent, "internet"));
	}

	public void onClickOpenMap(View v) {
		Intent intent = new Intent(android.content.Intent.ACTION_VIEW, Uri.parse("geo:37.8, -122.4"));
		startActivity(intent);
	}
}
