package com.excelsior.prototype.testset;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.Menu;

import com.excelsior.prototype.R;


public class FragmentTest extends FragmentActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.fragment_test);
		/*
		FragmentManager fragmentManager = getFragmentManager();
		FragmentTransaction fragmentTransaction = fragmentManager.beginTransaction();
		WindowManager windowManager = getWindowManager();
		Display display = windowManager.getDefaultDisplay();
		Fragment fragmentToDisplay = null;
		if (display.getRotation() == Surface.ROTATION_0 || 
				display.getRotation() == Surface.ROTATION_180) { // original orientation 
			fragmentToDisplay = new Test01Fragment();
		} else {
			fragmentToDisplay = new Test02Fragment();
		}
		fragmentTransaction.replace(android.R.id.content, fragmentToDisplay);
		fragmentTransaction.commit();*/
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.fragment_test, menu);
		return true;
	}

}
