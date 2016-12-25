package com.excelsior.prototype.testset;

import android.app.Activity;
import android.os.Bundle;
import android.support.v4.app.ActionBarDrawerToggle;
import android.support.v4.widget.DrawerLayout;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

import com.excelsior.prototype.R;

public class NavigationDrawerTest extends Activity {
	DrawerLayout mDrawerLayout;
	ActionBarDrawerToggle mToggle;
	View mDrawerView;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.navigation_drawer_test);
		mDrawerLayout = (DrawerLayout) findViewById(R.id.navigation_drawer_test__drawer_layout);
		mDrawerView = mDrawerLayout.findViewById(R.id.navigation_drawer_test__right_drawer);
		mToggle = new ActionBarDrawerToggle(this, mDrawerLayout, R.drawable.ic_drawer, R.string.hello_world,
				R.string.hello_world);
		mDrawerLayout.setDrawerListener(mToggle);
		// getActionBar().setDisplayHomeAsUpEnabled(true); // shows up button
		getActionBar().setHomeButtonEnabled(true);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.navigation_drawer_test, menu);
		return true;
	}

	public void onPostCreate(Bundle savedInstanceState) {
		super.onPostCreate(savedInstanceState);
		mToggle.syncState();
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
			case R.id.navigation_drawer_test__drawer_button:
				if (mDrawerLayout.isDrawerOpen(mDrawerView)) {
					mDrawerLayout.closeDrawer(mDrawerView);
				} else {
					mDrawerLayout.openDrawer(mDrawerView);
				}
				return true;
			default:
				return super.onOptionsItemSelected(item);
		}
		// use action bar title as drawer button
		// if (mToggle.onOptionsItemSelected(item)) {
		// return true;
		// }
	}
}
