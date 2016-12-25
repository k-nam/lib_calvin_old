package com.excelsior.prototype.photoselector;

import java.util.ArrayList;
import java.util.List;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.GridView;

import com.excelsior.prototype.R;
import com.excelsior.prototype.widget.ListCell;

public class PhotoSelector extends Fragment {
	private List<ListCell> gridCells;

	public PhotoSelector() {
		gridCells = new ArrayList<ListCell>();
		for (int i = 0; i < 100; i++) {
			gridCells.add(new GridItem(Integer.toString(i)));
		}
	}

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

	}

	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.photo_selector, container, false);
	}

	public void onStart() {
		super.onStart();
		GridAdapter adapter = new GridAdapter(getActivity(), gridCells);
		GridView gridView = (GridView) getView().findViewById(R.id.photo_selector_grid_view);
		gridView.setAdapter(adapter);
	}
}
