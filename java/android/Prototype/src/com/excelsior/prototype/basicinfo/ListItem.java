package com.excelsior.prototype.basicinfo;


import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;

import com.excelsior.prototype.R;
import com.excelsior.prototype.basicinfo.ListAdapter.BasicInfoCellType;
import com.excelsior.prototype.widget.ListCell;

public class ListItem implements ListCell {
	String mTitle;
	String mContent;
	public ListItem(String title, String content) {
		mTitle = title;
		mContent = content;
	}
	public int getViewType() {
		return BasicInfoCellType.ITEM.ordinal();
	}

	public View getView(LayoutInflater inflater, View convertView) {
		View view;
		if (convertView == null) {
			view = (View)inflater.inflate(R.layout.basic_info__list_item, null);
		} else {
			view = convertView;
		}
		TextView title = (TextView)view.findViewById(R.id.basic_info__item_title); 
		TextView content = (TextView)view.findViewById(R.id.basic_info__item_content);
		title.setText(mTitle);
		content.setText(mContent);
		return view;
	}
}
