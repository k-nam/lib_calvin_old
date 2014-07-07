package com.excelsior.prototype.testset;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Toast;

import com.excelsior.prototype.R;
import com.excelsior.prototype.maintab.MainTab;

public class TestSet extends Fragment implements OnClickListener {
	MainTab mainTabActivity;
	CharSequence[] dialogItems = { "Apple", "Samsung", "LG" };
	boolean[] dialogItemsCheckingStatus = new boolean[dialogItems.length];
	int infoTestRequestCode = 1;

	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		mainTabActivity = (MainTab) getActivity();
		View view = inflater.inflate(R.layout.test_set, container, false);
		setOnClickListenersForButtons(view);
		return view;
	}

	private void setOnClickListenersForButtons(View view) {
		Button b = null;
		b = (Button) view.findViewById(R.id.test_set__simple_dialog_button);
		b.setOnClickListener(this);
		b = (Button) view.findViewById(R.id.test_set__progress_dialog_button);
		b.setOnClickListener(this);
		b = (Button) view.findViewById(R.id.test_set__complex_progress_dialog_button);
		b.setOnClickListener(this);
		b = (Button) view.findViewById(R.id.test_set__open_intent_test_button);
		b.setOnClickListener(this);
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
			case R.id.test_set__simple_dialog_button:
				onClickSimpleDialogButton(v);
				break;
			case R.id.test_set__progress_dialog_button:
				onClickProgressDialogButton(v);
				break;
			case R.id.test_set__complex_progress_dialog_button:
				onClickComplexProgressDialogButton(v);
				break;
			case R.id.test_set__open_intent_test_button:
				onClickOpenIntentTestButton(v);
				break;
			default:
		}
	}

	private class MySimpleDialog extends DialogFragment {
		public Dialog onCreateDialog(Bundle savedInstanceState) {
			Builder builder = new AlertDialog.Builder(mainTabActivity);
			builder.setIcon(R.drawable.ic_launcher);
			builder.setTitle("Simple Dialog");
			builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {
					Toast.makeText(mainTabActivity, "OK clicked", Toast.LENGTH_SHORT).show();
				}
			});
			builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {
					Toast.makeText(mainTabActivity, "Cancel clicked", Toast.LENGTH_SHORT).show();
				}
			});
			builder.setMultiChoiceItems(dialogItems, dialogItemsCheckingStatus,
					new DialogInterface.OnMultiChoiceClickListener() {
						public void onClick(DialogInterface dialog, int which, boolean checked) {
							Toast.makeText(mainTabActivity, dialogItems[which] + ": " + (checked ? "checked" : "unchecked"),
									Toast.LENGTH_SHORT).show();
						}
					});
			return builder.create();
		}
	}

	private class MyProgressDialog extends DialogFragment {
		public Dialog onCreateDialog(Bundle savedInstanceState) {
			ProgressDialog dialog = new ProgressDialog(mainTabActivity);
			dialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
			dialog.setTitle("Processing...");
			dialog.setMessage("Please wait");
			return dialog;
		}
	}

	private class MyComplexProgressDialog extends DialogFragment {
		public Dialog onCreateDialog(Bundle savedInstance) {
			final ProgressDialog progressDialog = new ProgressDialog(mainTabActivity);
			progressDialog.setIcon(R.drawable.ic_launcher);
			progressDialog.setTitle("Processing...");
			progressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
			progressDialog.setButton(DialogInterface.BUTTON_POSITIVE, "OK", new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {
					// Toast.makeText(mainTabActivity, "OK clicked",
					// Toast.LENGTH_SHORT).show();
				}
			});
			progressDialog.setButton(DialogInterface.BUTTON_NEGATIVE, "Cancel", new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {
					Toast.makeText(mainTabActivity, "Cancel clicked", Toast.LENGTH_SHORT).show();
				}
			});
			progressDialog.setProgress(0);
			new Thread(new Runnable() {
				public void run() {
					try {
						for (int i = 0; i < 100; i++) {
							progressDialog.incrementProgressBy(1);
							Thread.sleep(30);
						}
						dismiss();
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}).start();
			return progressDialog;
		}
	}

	public void onClickSimpleDialogButton(View v) {
		new MySimpleDialog().show(getFragmentManager(), "aa");
	}

	public void onClickProgressDialogButton(View v) {
		final DialogFragment dialog = new MyProgressDialog();
		dialog.show(getFragmentManager(), "aa");
		new Thread(new Runnable() {
			public void run() {
				try {
					Thread.sleep(2000);
					dialog.dismiss();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}).start();
	}

	public void onClickComplexProgressDialogButton(View v) {
		new MyComplexProgressDialog().show(getFragmentManager(), "aa");
	}

	public void onClickOpenIntentTestButton(View v) {
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
		if (requestCode == infoTestRequestCode) {
			if (resultCode == Activity.RESULT_OK) {
				Toast.makeText(mainTabActivity, "Back to the main activity!", Toast.LENGTH_SHORT).show();
				Toast.makeText(mainTabActivity, data.getData().toString(), Toast.LENGTH_SHORT).show();
				Toast.makeText(mainTabActivity, Integer.toString(data.getIntExtra("int extra", 0)), Toast.LENGTH_SHORT).show();
			}
		}
	}
}
