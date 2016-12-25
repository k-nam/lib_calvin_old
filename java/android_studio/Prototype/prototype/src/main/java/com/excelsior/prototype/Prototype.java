package com.excelsior.prototype;

import android.app.Application;
import android.content.res.Configuration;

public class Prototype extends Application {
	public static Prototype getInstance() {
		return singleton;
	}

	@Override
	public final void onCreate() {
		super.onCreate();
		singleton = this;
	}

	@Override
	public final void onLowMemory() {
		super.onLowMemory();
	}

	@Override
	public final void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
	}

	private static Prototype singleton;
}

















