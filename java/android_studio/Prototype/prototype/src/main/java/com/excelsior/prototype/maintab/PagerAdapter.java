package com.excelsior.prototype.maintab;

import java.util.List;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

public class PagerAdapter extends FragmentPagerAdapter {
	List<Fragment> mFragments;

	public PagerAdapter(FragmentManager fm, List<Fragment> arrayList) {
		super(fm);
		mFragments = arrayList;
	}

	@Override
	public Fragment getItem(int tabIndex) {
		return mFragments.get(tabIndex);
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub2w
		return mFragments.size();
	}

	public CharSequence getPageTitle(int tabIndex) {
		if (tabIndex == 0) {
			return "Basic Info";
		} else if (tabIndex == 1) {
			return "Test Set";
		} else if (tabIndex == 2) {
			return "Photo";
		} else if (tabIndex == 3) {
			return "Facebook";
		} else if (tabIndex == 4) {
			return "VividCamera23213";
		} else {
			return "error";
		}
	}
}
