using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DestroyByContact : MonoBehaviour {
	public GameObject explosion;
	public GameObject playerExplosion;
	public GameController controller;

	void Start() {
		GameObject controllerObject = GameObject.FindWithTag ("GameController");
		if (controllerObject) {
			controller = controllerObject.GetComponent<GameController> ();
		} else {
			Debug.Log ("Could not find GameController object");
		}
	}

	void OnTriggerEnter(Collider other)
	{
		//Debug.Log ("Other:" + other.name);
		if (other.tag == "Boundary") {
			return;
		} 
		if (other.tag == "Asteroid") {
			return;
		} 
		Destroy (gameObject);
		Instantiate (explosion, transform.position, transform.rotation);
		Destroy (other.gameObject);
		if (other.tag == "Player") {
			Instantiate (playerExplosion, other.transform.position, other.transform.rotation);
		} else { // with bolt
			controller.AddScore(1);
		}	
	}
}

