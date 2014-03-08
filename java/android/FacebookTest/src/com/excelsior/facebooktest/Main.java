package com.excelsior.facebooktest;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.util.Base64;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.TextView;

import com.facebook.Session;
import com.facebook.SessionState;

public class Main extends FragmentActivity {

	CharSequence faceBookTag = "excelsior.facebook";
	TextView mTextView;

	private Session.StatusCallback statusCallback = new SessionStatusCallback();

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		mTextView = (TextView) findViewById(R.id.facebook_test__welcome_text);
		mTextView.setText("onCreated");
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	public void onClickSeeLoginStatusButton(View v) {
		Session session = Session.getActiveSession();
		if (session == null) {
			mTextView.setText("session null");
			Session.openActiveSession(this, true, statusCallback);
		} else if (session.isOpened()) {
			mTextView.setText("session is open");
		} else if (!session.isOpened() && !session.isClosed()) {
			mTextView.setText("session not open yet");
			if (session.getState() == SessionState.CREATED_TOKEN_LOADED) {
				mTextView.setText("session CREATED_TOKEN_LOADED");
			}
			session.openForRead(new Session.OpenRequest(this).setPermissions(Arrays.asList("basic_info")).setCallback(
					statusCallback));
			// session.close();
		} else if (session.isClosed()) {
			mTextView.setText("session closed");
			Session.openActiveSession(this, true, statusCallback);
		} else {
			mTextView.setText("????????");
		}
	}

	public void onClickCloseSessionButton(View v) {
		Session session = Session.getActiveSession();
		if (session != null) {
			Session.getActiveSession().close();
		}
	}

	private class SessionStatusCallback implements Session.StatusCallback {
		@Override
		public void call(Session session, SessionState state, Exception exception) {
			try {
				PackageInfo info = getPackageManager().getPackageInfo("com.excelsior.prototype",
						PackageManager.GET_SIGNATURES);
				for (Signature signature : info.signatures) {
					MessageDigest md = MessageDigest.getInstance("SHA");
					md.update(signature.toByteArray());
					Log.d("KeyHash:", Base64.encodeToString(md.digest(), Base64.DEFAULT));
				}
			} catch (NameNotFoundException e) {

			} catch (NoSuchAlgorithmException e) {

			}
			
			if (state == SessionState.OPENED_TOKEN_UPDATED) {
				mTextView.setText("SessionState OPENED_TOKEN_UPDATED");
			} else if (state == SessionState.CLOSED_LOGIN_FAILED) {
				mTextView.setText("SessionState CLOSED_LOGIN_FAILED");
			} else if (state == SessionState.CREATED) {
				mTextView.setText("SessionState CREATED");
			} else if (state == SessionState.CLOSED_LOGIN_FAILED) {
				mTextView.setText("SessionState CLOSED_LOGIN_FAILED");
			} else if (state == SessionState.OPENED_TOKEN_UPDATED) {
				mTextView.setText("SessionState OPENED_TOKEN_UPDATED");
			} else if (state == SessionState.OPENING) {
				mTextView.setText("SessionState OPENING");
			} else {
				mTextView.setText("SessionState ?");
			}

		}

	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		Session.getActiveSession().onActivityResult(this, requestCode, resultCode, data);
	}

}
