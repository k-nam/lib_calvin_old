package com.excelsior.prototype.photoselector;

import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;

import com.excelsior.prototype.R;
import com.excelsior.prototype.photoselector.GridAdapter.CellType;
import com.excelsior.prototype.widget.ListCell;

public class GridItem implements ListCell {
	private String mTitle;
	
	GridItem(String title) {
		mTitle = title;
	}
	
	@Override
	public int getViewType() {
		CellType.TEXT.ordinal();
		return 0;
	}

	@Override
	public View getView(LayoutInflater inflater, View convertView) {
		View view;
		if (convertView == null) {
			view = (View)inflater.inflate(R.layout.photo_selector__grid_item, null);
		} else {
			view = convertView;
		}
		TextView textView = (TextView)view.findViewById(R.id.photo_selector_grid_item_text); 
		textView.setText(mTitle);
		return view;
	}

}
