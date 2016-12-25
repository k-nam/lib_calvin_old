package com.excelsior.prototype.basicinfo;

import java.util.ArrayList;
import java.util.List;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;

import com.excelsior.prototype.R;
import com.excelsior.prototype.widget.ListCell;

public class BasicInfo extends Fragment {
	List<ListCell> listCells;
	
	public BasicInfo() {
		listCells = new ArrayList<ListCell>();
		listCells.add(new ListHeader("Attack")); 
		listCells.add(new ListItem("Damage", "650"));
		listCells.add(new ListItem("Speed", "203"));
		listCells.add(new ListItem("Critical", "27.4%"));
		listCells.add(new ListHeader("Defense"));
		listCells.add(new ListItem("Life", "2091"));
		listCells.add(new ListItem("Armor", "132"));
		listCells.add(new ListItem("Evade", "18%"));
		listCells.add(new ListHeader("Magic"));
		listCells.add(new ListItem("Mana", "320"));
		listCells.add(new ListItem("Resistance", "22%"));
		listCells.add(new ListButton("Click to add new cell"));		
	}
	
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		View view = inflater.inflate(R.layout.basic_info, container, false);
		return view;
	}

	public void onStart() {
		super.onStart();
		ListAdapter adapter = new ListAdapter(getActivity(), listCells);
		ListView listView = (ListView)getView().findViewById(R.id.basic_info__list);
		listView.setAdapter(adapter);
	}
	
	public void addCell() {
		ListItem newItem = new ListItem("Additional cell", "???");
		// modify data
		listCells.add(listCells.size() - 1, newItem);
		// modify UI
		ListView listView = (ListView)getView().findViewById(R.id.basic_info__list);
		ListAdapter adapter = (ListAdapter)listView.getAdapter();
		adapter.notifyDataSetChanged();
	}

}
