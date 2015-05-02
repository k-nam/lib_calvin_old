package com.excelsior.comfydrive;

import android.hardware.SensorEvent;

public class Anaylizer {
	private final float nano = 1000000000.0f;
	private float interval = 0;
	private final float gravityConstant = 9.8f;
	private Vector gravity = null;
	private Vector forward = null;
	private Vector updown = null;
	private Vector sideways = null;
	private Vector previousGravity = null;
	private Vector previousForward = null;
	private Vector previousUpdown = null;
	private Vector previousSideways = null;
	private float gravityAlpha = 0.9f;
	private float forwardAlpha = 0.8f;
	private float previousTimestamp = 0;

	private class Vector {
		public final float x;
		public final float y;
		public final float z;

		public Vector(float x, float y, float z) {
			this.x = x;
			this.y = y;
			this.z = z;
		}

		public float length() {
			return (float)Math.sqrt(x * x + y * y + z * z);
		}

		public float getInnterProductWith(Vector vector) {
			return x * vector.x + y * vector.y + z * vector.z;
		}

		public Vector getNormarlPartTo(Vector vector) {
			return subtract(getParallelPartTo(vector));
		}

		public Vector getParallelPartTo(Vector vector) {
			return vector.multiply(this.getInnterProductWith(vector) / vector.getInnterProductWith(vector));
		}

		private Vector multiply(float k) {
			return new Vector(x * k, y * k, z * k);
		}

		private Vector subtract(Vector vector) {
			return new Vector(x - vector.x, y - vector.y, z - vector.z);
		}

		private Vector add(Vector vector) {
			return new Vector(x + vector.x, y + vector.y, z + vector.z);
		}
	}

	private Vector add(Vector vector1, Vector vector2) {
		return vector1.add(vector2);
	}

	private Vector subtract(Vector vector1, Vector vector2) {
		return vector1.subtract(vector2);
	}

	private Vector multiply(Vector vector, float k) {
		return vector.multiply(k);
	}

	public void refresh(SensorEvent event) {
		Vector currentAccel = new Vector(event.values[0], event.values[1], event.values[2]);
		if (gravity == null) {
			gravity = currentAccel;
			forward = new Vector(0, 0, 0);
			updown = new Vector(0, 0, 0);
			sideways = new Vector(0, 0, 0);
			return;
		} else {
			saveToPrevious();
			// adjust gravity
			Vector newGravity = add(multiply(gravity, gravityAlpha), multiply(currentAccel, (1 - gravityAlpha)));
			Vector plane = currentAccel.getNormarlPartTo(newGravity);
			Vector newUpdown = currentAccel.getParallelPartTo(newGravity);
			// adjust forward
			Vector newForward = add(multiply(forward, forwardAlpha), multiply(plane, (1 - forwardAlpha)));
			Vector newSideways = subtract(plane, newForward);
			// refresh current values
			interval = (event.timestamp - previousTimestamp) / nano;
			gravity = newGravity;
			forward = newForward;
			sideways = newSideways;
			updown = newUpdown;
		}
		normalizeGravity();
		previousTimestamp = event.timestamp;
	}

	private void saveToPrevious() {
		previousGravity = gravity;
		previousForward = forward;
		previousUpdown = updown;
		previousSideways = sideways;
	}

	private void normalizeGravity() {
		// the magnitude of gravity is constant on earth
		// the residual goes into updown
		Vector normalizedGravity = gravity.multiply(gravityConstant / gravity.length());
		Vector residual = gravity.subtract(normalizedGravity);
		gravity = normalizedGravity;
		updown = updown.add(residual);
	}

	public float getForwardAccel() {
		return forward.length();
	}

	public float getForwardShock() {
		return (forward.length() - previousForward.length()) / interval;
	}

	public float getUpdownAccel() {
		return updown.length();
	}

	public float getUpdownShock() {
		return (updown.length() - previousUpdown.length()) / interval;
	}

	public float getSidewaysAccel() {
		return sideways.length();
	}

	public float getSidewaysShock() {
		return (sideways.length() - previousSideways.length()) / interval;
	}
}
