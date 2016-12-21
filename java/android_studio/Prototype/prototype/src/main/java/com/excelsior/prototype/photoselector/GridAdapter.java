package com.excelsior.prototype.photoselector;

import java.util.List;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;

import com.excelsior.prototype.widget.ListCell;

public class GridAdapter extends ArrayAdapter<ListCell> {
	private LayoutInflater mInflator;
	public enum CellType {
		TEXT
	}
	public GridAdapter(Context context, List<ListCell> objects) {
		super(context, 0, objects);
		mInflator = LayoutInflater.from(context);
	}

	public int getViewTypeCount() {
		return CellType.values().length;
	}
	
	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		return getItem(position).getView(mInflator, convertView);
	}
}
