package com.excelsior.prototype.widget;

import android.view.LayoutInflater;
import android.view.View;

public interface ListCell {
	public int getViewType();
	public View getView(LayoutInflater inflater, View convertView);
}
