package com.excelsior.prototype.wholikesme;

import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;

import com.excelsior.prototype.R;
import com.excelsior.prototype.wholikesme.ListAdapter.WhoLikesMeCellType;
import com.excelsior.prototype.widget.ListCell;

public class ListItem implements ListCell {
	String mUserName;
	int mNumLikes;

	public ListItem(String title, int numLikes) {
		mUserName = title;
		mNumLikes = numLikes;
	}

	public int getViewType() {
		return WhoLikesMeCellType.FRIEND.ordinal();
	}

	public View getView(LayoutInflater inflater, View convertView) {
		View view;
		if (convertView == null) {
			view = inflater.inflate(R.layout.who_likes_me__list_item, null);
		} else {
			view = convertView;
		}
		TextView friendName = (TextView) view.findViewById(R.id.who_likes_me__friend_name);
		friendName.setText(mUserName);
		TextView numLikes = (TextView) view.findViewById(R.id.who_likes_me__num_likes);
		numLikes.setText(Integer.toString(mNumLikes));
		return view;
	}
}
