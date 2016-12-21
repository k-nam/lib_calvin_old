package com.excelsior.prototype.testset;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.EditText;

import com.excelsior.prototype.R;

public class LoginTest extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.login_test);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.login, menu);
		return true;
	}

	public void onClickLogin(View v) {
		Intent data = new Intent();
		EditText loginInfo = (EditText) findViewById(R.id.login_test_edit_text);
		data.setData(Uri.parse(loginInfo.getText().toString()));
		setResult(RESULT_OK, data);
		Log.e("calvin", "before finish");
		finish();
		Log.e("calvin", "after finish");
	}
}
