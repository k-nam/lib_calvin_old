package com.excelsior.prototype.maintab;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

import android.app.ActionBar;
import android.app.ActionBar.Tab;
import android.app.FragmentTransaction;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.content.res.Configuration;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.NotificationCompat;
import android.support.v4.view.ViewPager;
import android.util.Base64;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.Toast;

import com.excelsior.prototype.R;
import com.excelsior.prototype.basicinfo.BasicInfo;
import com.excelsior.prototype.photoselector.PhotoSelector;
import com.excelsior.prototype.testset.TestSet;
import com.excelsior.prototype.vividcamera.VividCamera;
import com.excelsior.prototype.wholikesme.WhoLikesMe;

public class MainTab extends FragmentActivity {
	// test
	String lifeCycleTag = "Life cycle: ";
	int notificationID = 1;
	int loginTestRequestCode = 0;
	int infoTestRequestCode = 1;
	// fragment for each tabs
	BasicInfo basicInfo;
	TestSet testSet;
	PhotoSelector photoSelector;
	WhoLikesMe whoLikesMe;
	VividCamera vividCamera;
	PagerAdapter fragmentPagerAdapter;
	ViewPager viewPager;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		setContentView(R.layout.main_tab);
		basicInfo = new BasicInfo();
		testSet = new TestSet();
		photoSelector = new PhotoSelector();
		whoLikesMe = new WhoLikesMe();
		vividCamera = new VividCamera();
		fragmentPagerAdapter = new PagerAdapter(getSupportFragmentManager(), Arrays.asList(vividCamera, basicInfo, testSet,
				photoSelector, whoLikesMe));
		viewPager = (ViewPager) findViewById(R.id.main_tab_view_pager);
		viewPager.setAdapter(fragmentPagerAdapter);
		viewPager.setOnPageChangeListener(new ViewPager.SimpleOnPageChangeListener() {
			public void onPageSelected(int pageIndex) {
				getActionBar().setSelectedNavigationItem(pageIndex);
			}
		});
		final ActionBar actionBar = getActionBar();
		actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);
		ActionBar.TabListener listener = new ActionBar.TabListener() {
			public void onTabReselected(Tab tab, FragmentTransaction ft) {
			}

			public void onTabSelected(Tab tab, FragmentTransaction ft) {
				viewPager.setCurrentItem(tab.getPosition());
			}

			public void onTabUnselected(Tab tab, FragmentTransaction ft) {
			}
		};
		Tab basicInfoTab = actionBar.newTab();
		Tab testSetTab = actionBar.newTab();
		Tab photoSelectorTab = actionBar.newTab();
		Tab whoLikesMeTab = actionBar.newTab();
		Tab vividCameraTab = actionBar.newTab();
		basicInfoTab.setText("Basic info");
		testSetTab.setText("Test");
		photoSelectorTab.setText("Photo");
		whoLikesMeTab.setText("WhoLikesMe");
		vividCameraTab.setText("VividCamera");
		// basicInfoTab.setIcon(R.drawable.icon_basic_info_tab);
		// testSetTab.setIcon(R.drawable.icon_test_set_tab);
		basicInfoTab.setTabListener(listener);
		testSetTab.setTabListener(listener);
		photoSelectorTab.setTabListener(listener);
		whoLikesMeTab.setTabListener(listener);
		vividCameraTab.setTabListener(listener);
		actionBar.addTab(vividCameraTab);
		actionBar.addTab(basicInfoTab);
		actionBar.addTab(testSetTab);
		actionBar.addTab(photoSelectorTab);
		actionBar.addTab(whoLikesMeTab);
		// finding out hash key on this machine: needed for facebook login
		PackageInfo info;
		try {
			info = getPackageManager().getPackageInfo("com.excelsior.prototype", PackageManager.GET_SIGNATURES);
			for (Signature signature : info.signatures) {
				MessageDigest md;
				md = MessageDigest.getInstance("SHA");
				md.update(signature.toByteArray());
				String something = new String(Base64.encode(md.digest(), 0));
				// String something = new String(Base64.encodeBytes(md.digest()));
				Log.e("hash key", something);
			}
		} catch (NameNotFoundException e1) {
			Log.e("hash key", e1.toString());
		} catch (NoSuchAlgorithmException e) {
			Log.e("hash key", e.toString());
		} catch (Exception e) {
			Log.e("hash key", e.toString());
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main_tab, menu);
		return true;
	}

	public void onClickAddCellButton(View v) {
		basicInfo.addCell();
	}

	public void onClickShowNotificationButton(View v) {
		Intent intent = new Intent(this, Test01Notification.class);
		intent.putExtra("notificationId", notificationID);
		PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, intent, 0);
		NotificationCompat.Builder builder = new NotificationCompat.Builder(this).setSmallIcon(R.drawable.ic_launcher)
				.setContentTitle("Systemt alarm").setContentText("Kill him at 3 PM")
				.setVibrate(new long[] { 100, 250, 100, 500 }).setContentIntent(pendingIntent);
		NotificationManager nm = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
		nm.notify(notificationID, builder.build());
	}

	public void onClickLoginTestButton(View v) {
		Intent intent = new Intent("com.excelsior.prototype.LoginTest");
		startActivityForResult(intent, loginTestRequestCode);
	}

	public void onClickIntentTestButton(View v) {
		Intent intent = new Intent("com.excelsior.prototype.IntentTest");
		intent.putExtra("extra1", "This is a string extra");
		intent.putExtra("extra2", 177);
		Bundle bundle = new Bundle();
		bundle.putString("string extra", "This is a string extra in bundle");
		bundle.putInt("int extra", 151);
		intent.putExtras(bundle);
		intent.setData(Uri.parse("http://www.naver.com"));
		startActivityForResult(Intent.createChooser(intent, "Choose for intent test activity"), infoTestRequestCode);
	}

	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		if (requestCode == loginTestRequestCode) {
			if (resultCode == RESULT_OK) {
				Toast.makeText(this, "Back to the main activity!", Toast.LENGTH_SHORT).show();
				Toast.makeText(this, "Login data: " + data.getData().toString(), Toast.LENGTH_SHORT).show();
			}
		}
	}

	public void onClickFragmentTestButton(View v) {
		Intent intent = new Intent("com.excelsior.prototype.FragmentTest");
		startActivity(intent);
	}

	public void onClickBuiltInAppsTestButton(View v) {
		Intent intent = new Intent("com.excelsior.prototype.BuiltInAppsTest");
		startActivity(intent);
	}

	public void onClickNavigationDrawerTestButton(View v) {
		Intent intent = new Intent("com.excelsior.prototype.NavigationDrawerTest");
		startActivity(intent);
	}

	public void onClickFacebookTestButton(View v) {
		// Intent intent = new Intent("com.excelsior.prototype.FacebookTest");
		// startActivity(intent);
	}

	@Override
	public void onRestoreInstanceState(Bundle savedInstance) {
		super.onRestoreInstanceState(savedInstance);
	}

	@Override
	public void onRestart() {
		Log.d(lifeCycleTag, "onRestart");
		super.onRestart();
	}

	@Override
	public void onStart() {
		Log.d(lifeCycleTag, "onStart");
		super.onStart();
	}

	@Override
	public void onResume() {
		Log.d(lifeCycleTag, "onResume");
		super.onResume();
	}

	@Override
	public void onSaveInstanceState(Bundle instanceStateToSave) {
		super.onSaveInstanceState(instanceStateToSave);
	}

	@Override
	public void onPause() {
		Log.d(lifeCycleTag, "onPause");
		super.onPause();
	}

	@Override
	public void onStop() {
		Log.d(lifeCycleTag, "onStop");
		super.onStop();
	}

	@Override
	public void onDestroy() {
		Log.d(lifeCycleTag, "onDestroy");
		super.onDestroy();
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {} else {}
		if (newConfig.keyboardHidden == Configuration.KEYBOARDHIDDEN_NO) {}
	}
}
