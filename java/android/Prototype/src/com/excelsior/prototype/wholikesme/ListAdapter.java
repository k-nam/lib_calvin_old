package com.excelsior.prototype.wholikesme;

import java.util.List;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;

import com.excelsior.prototype.widget.ListCell;

public class ListAdapter extends ArrayAdapter<ListCell> {

	public ListAdapter(Context context, List<ListCell> objects) {
		super(context, 0, objects);
		mInflater = LayoutInflater.from(context);
	}

	private LayoutInflater mInflater;

	public enum WhoLikesMeCellType {
		FRIEND
	}

	public int getViewTypeCount() {
		return WhoLikesMeCellType.values().length;
	}

	public int getItemViewType(int cellIndex) {
		return getItem(cellIndex).getViewType();
	}

	public View getView(int cellIndex, View convertView, ViewGroup parent) {
		return getItem(cellIndex).getView(mInflater, convertView);
	}
}
