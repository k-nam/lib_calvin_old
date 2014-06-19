package com.excelsior.prototype.testset;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Toast;

import com.excelsior.prototype.R;

public class IntentTest extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.intent_test);
		Uri url = getIntent().getData();
		WebView webView = (WebView) findViewById(R.id.intent_test_web_view);
		webView.setWebViewClient(new Callback());
		webView.loadUrl(url.toString());
	}

	private class Callback extends WebViewClient {
		@Override
		public boolean shouldOverrideUrlLoading(WebView webView, String url) {
			return false;
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.intent_test, menu);
		return true;
	}

	public void onClickShowPassedInfo(View v) {
		Toast.makeText(this, "extra1: " + getIntent().getStringExtra("extra1"), Toast.LENGTH_SHORT).show();
		Toast.makeText(this, "extra2: " + Integer.toString(getIntent().getIntExtra("extra2", 0)), Toast.LENGTH_SHORT)
				.show();
		Bundle bundle = getIntent().getExtras();
		Toast.makeText(this, "string extra: " + bundle.getString("string extra"), Toast.LENGTH_SHORT).show();
		Toast.makeText(this, "int extra: " + Integer.toString(bundle.getInt("int extra")), Toast.LENGTH_SHORT).show();
	}

	public void onClickGoBack(View v) {
		Intent intent = new Intent();
		intent.putExtra("int extra", 166);
		intent.setData(Uri.parse("Intent passed back to main activity"));
		setResult(RESULT_OK, intent);
		finish();
	}
}
