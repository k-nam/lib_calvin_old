package com.excelsior.prototype.basicinfo;

import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;

import com.excelsior.prototype.R;
import com.excelsior.prototype.basicinfo.ListAdapter.BasicInfoCellType;
import com.excelsior.prototype.widget.ListCell;

public class ListHeader implements ListCell {
	private String mTitle;

	public ListHeader(String title) {
		mTitle = title;
	}

	public int getViewType() {
		return BasicInfoCellType.HEADER.ordinal();
	}

	public View getView(LayoutInflater inflater, View convertView) {
		View view;
		if (convertView == null) {
			view = inflater.inflate(R.layout.basic_info__list_header, null);
		} else {
			view = convertView;
		}
		TextView textView = (TextView)view.findViewById(R.id.basic_info__header);
		textView.setText(mTitle);
		return view;
	}

}
