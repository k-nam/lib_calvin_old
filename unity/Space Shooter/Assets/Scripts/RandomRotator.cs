using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RandomRotator : MonoBehaviour {
	public float tumble;

	void Start()
	{
		Rigidbody body = GetComponent<Rigidbody> ();
		body.angularVelocity = Random.insideUnitSphere * 5;
	}
}
