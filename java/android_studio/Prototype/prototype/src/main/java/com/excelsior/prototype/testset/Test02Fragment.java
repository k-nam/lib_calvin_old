package com.excelsior.prototype.testset;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.excelsior.prototype.R;

public class Test02Fragment extends Fragment {
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.fragment_test__test_02_fragment, container, false);
	}

	public void onStart() {
		super.onStart();
		Button showDataButton = (Button)getActivity().findViewById(R.id.test_02_fragment_show_data_button);
		showDataButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				// TODO Auto-generated method stub
				final EditText editText = (EditText) getActivity().findViewById(R.id.test_01_fragment_edit_text);
				Toast.makeText(getActivity(), editText.getText().toString(), Toast.LENGTH_SHORT).show();
			}
		});
	}
}
