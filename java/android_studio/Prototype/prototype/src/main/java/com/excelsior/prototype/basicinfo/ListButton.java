package com.excelsior.prototype.basicinfo;

import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;

import com.excelsior.prototype.R;
import com.excelsior.prototype.basicinfo.ListAdapter.BasicInfoCellType;
import com.excelsior.prototype.widget.ListCell;

public class ListButton implements ListCell {
	private String mTitle;

	public ListButton(String title) {
		mTitle = title;
	}

	public int getViewType() {
		return BasicInfoCellType.BUTTON.ordinal();
	}

	public View getView(LayoutInflater inflater, View convertView) {
		View view;
		if (convertView == null) {
			view = inflater.inflate(R.layout.basic_info__list_button, null);
		} else {
			view = convertView;
		}
		Button button = (Button)view.findViewById(R.id.basic_info__add_cell_button);
		button.setText(mTitle);
		return view;
	}
}
