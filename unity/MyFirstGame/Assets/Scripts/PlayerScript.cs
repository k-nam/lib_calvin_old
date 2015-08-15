﻿using UnityEngine;
using System.Collections;

public class PlayerScript : MonoBehaviour {
	
	private Rigidbody rb;
	public float speed;

	// Use this for initialization
	void Start () {
		rb = GetComponent<Rigidbody> ();
	}
	
	
	void FixedUpdate() {
		float moveHorizontal = Input.GetAxis ("Horizontal");
		float moveVertical = Input.GetAxis ("Vertical");
		
		rb.AddForce (new Vector3(moveHorizontal, 0.0f, moveVertical ) * speed);
	}
}
